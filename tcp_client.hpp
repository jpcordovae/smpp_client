#ifndef _TCP_CLIENT_HPP_
#define _TCP_CLIENT_HPP_

#include <deque>
#include <queue>
#include <vector>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

#include "smpp_defs.hpp"

#ifdef POSIX
#include <termios.h>
#endif

using boost::asio::ip::tcp;
using namespace std;

#define MAX_READ_LENGHT 2048

class tcp_client
{
public:

   enum enStatus { CONNECTED, CONNECTING, DISCONNECTED };
   
   enum { max_read_length = 2048 };


   tcp_client(boost::asio::io_service &_io_service, tcp::resolver::iterator endpoint_iterator)
      : m_io_service(_io_service),
	socket_(_io_service)
      {
         connect_start(endpoint_iterator);
      }

   tcp_client(boost::asio::io_service &_io_service)
      : m_io_service(_io_service),
	socket_(_io_service)
      {
      }
   
   void foo(void)
      {
	 while(1)
	 {
	    sleep(1);
	 }
      }
   
   tcp_client(void)
      : m_io_service(tcp_io_service),
        socket_(m_io_service)
      {
      }
   
   void connect(const char *host, const char *port)
      {

	 m_io_service.post(boost::bind(&tcp_client::do_connect,this,host,port));
      }
   
   void write(const char msg) // pass the write data to the do_write function via the io service in the other thread
      {
         m_io_service.post(boost::bind(&tcp_client::do_write, this, msg));
      }

   void write(const boost::uint8_t *_buffer, size_t _lenght)
      {
	 buffertype_ptr buffer_ptr(new buffertype());
	 buffer_ptr->assign(_buffer,_buffer+_lenght);
	 m_io_service.post(boost::bind(&tcp_client::do_write_vct, this, buffer_ptr));
      }
   
   void close() // call the do_close function via the io service in the other thread
      {
         m_io_service.post(boost::bind(&tcp_client::do_close, this));
      }

   enStatus get_tcp_status(void)
      {
	 boost::mutex::scoped_lock l(status_mutex);
	 return status;
      }

   bool rx_queue_empty(void)
      {
	 boost::mutex::scoped_lock l(m_rx_queue_mutex);
	 return m_rx_queue.empty();
      }

   buffertype_ptr get_rx_message(void)
      {
	 boost::mutex::scoped_lock l(m_rx_queue_mutex);
	 return m_rx_queue.front();
      }

   void pop_rx_queue(void)
      {
	 boost::mutex::scoped_lock l(m_rx_queue_mutex);
	 m_rx_queue.pop();
      }
   
private:

   void do_connect(const char *host, const char *port)
      {
	 if(get_tcp_status()==CONNECTED || get_tcp_status()==CONNECTING)
	 {
	    std::cout << "trying to connect with status CONNECTED or CONNECTING" << std::endl;
	    do_close();
	 }
	 tcp::resolver::query query(host,port);
	 tcp::resolver resolver(m_io_service);
	 tcp::resolver::iterator iterator = resolver.resolve(query);
	 //m_io_service.post(boost::bind(&tcp_client::connect_start,this,iterator));
	 connect_start(iterator);
      }
   
   void set_tcp_status(enStatus _status)
      {
	 boost::mutex::scoped_lock l(status_mutex);
	 status = _status;
      }

   void connect_start(tcp::resolver::iterator endpoint_iterator)
      { // asynchronously connect a socket to the specified remote endpoint and call connect_complete when it completes or fails
	 std::cout << "connecting" << std::endl;
	 tcp::endpoint endpoint = *endpoint_iterator;
	 set_tcp_status(CONNECTING);
         socket_.async_connect(endpoint,
                               boost::bind(&tcp_client::connect_complete,
                                           this,
                                           boost::asio::placeholders::error,
                                           ++endpoint_iterator));
      }

   void connect_complete(const boost::system::error_code& error, tcp::resolver::iterator endpoint_iterator)
      { // the connection to the server has now completed or failed and returned an error
         if (!error) // success, so start waiting for read data
	 {
	    set_tcp_status(CONNECTED);
	    std::cout << "connected" << std::endl;
            read_start();
	 }
         else if (endpoint_iterator != tcp::resolver::iterator())
         { // failed, so wait for another connection event
            socket_.close();
	    set_tcp_status(DISCONNECTED);
            connect_start(endpoint_iterator);
         }
      }

   void read_start(void)
      { // Start an asynchronous read and call read_complete when it completes or fails
	 std::cout << "read start" << std::endl;
         socket_.async_read_some(boost::asio::buffer(read_msg_, MAX_READ_LENGHT),
                                 boost::bind(&tcp_client::read_complete,
                                             this,
                                             boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred));
      }

   void read_complete(const boost::system::error_code& error, size_t bytes_transferred)
      { // the asynchronous read operation has now completed or failed and returned an error
	 if (!error)
         {
	    //dump_buffer((boost::uint8_t*)read_msg_,bytes_transferred);
	    buffertype_ptr _ptr = buffertype_ptr(new buffertype(read_msg_,read_msg_+bytes_transferred));

	    {
	       boost::mutex::scoped_lock l(m_rx_queue_mutex);
	       //dump_buffer(_ptr->data(),_ptr->size());
	       m_rx_queue.push(_ptr);
	    }

	    read_start(); // start waiting for another asynchronous read again
         }

	 if (error == boost::asio::error::eof)
	 {
	    printf("connection closed cleanly by peer\n");
	    do_close();
	    return;
	 } else if (error) {
	    throw boost::system::system_error(error); // Some other error.  if( ! error ) 
	 }
	 
      }

   void do_write(const char msg)
      { // callback to handle write call from outside this class
         bool write_in_progress = !write_msgs_.empty(); // is there anything currently being written?
         write_msgs_.push_back(msg); // store in write buffer
         if (!write_in_progress) // if nothing is currently being written, then start
            write_start();
      }
   
   void write_start(void)
      { // Start an asynchronous write and call write_complete when it completes or fails
         boost::asio::async_write(socket_,
                                  boost::asio::buffer(&write_msgs_.front(), 1),
                                  boost::bind(&tcp_client::write_complete,
                                              this,
                                              boost::asio::placeholders::error));
      }

   void write_complete(const boost::system::error_code& error)
      { // the asynchronous read operation has now completed or failed and returned an error
         if (!error)
         { // write completed, so send next write data
            write_msgs_.pop_front(); // remove the completed data
            if (!write_msgs_.empty()) // if there is anthing left to be written
               write_start(); // then start sending the next item in the buffer
         }
         else
	 {
            do_close();
	 }
      }
   
   void do_write_vct(buffertype_ptr _buffer_ptr)
      {
	 bool write_in_progress = !m_write_queue.empty();
	 {
	    boost::mutex::scoped_lock l(m_write_queue_mutex);
	    m_write_queue.push(_buffer_ptr);
	 }
	 if(!write_in_progress)
	 {
	    write_start_vct();
	 }
      }

   void write_start_vct(void)
      {
	 buffertype_ptr _ptr;
	 {
	    boost::mutex::scoped_lock l(m_write_queue_mutex);
	    _ptr = m_write_queue.front();
	 }
	 boost::asio::async_write(socket_,
				  boost::asio::buffer(_ptr->data(),_ptr->size()),
				  boost::bind(&tcp_client::write_complete_vct,
					      this,
					      boost::asio::placeholders::error));
      }

   void write_complete_vct(const boost::system::error_code &error)
      {
	 if(!error)
	 {
	    {
	       boost::mutex::scoped_lock l(m_write_queue_mutex);
	       m_write_queue.pop();
	    }
	    if(!m_write_queue.empty())
	    {
	       write_start_vct();
	    }
	 }else
	 {
	    do_close();
	 }
      }
   
   void do_close()
      {
         std::cout << "closing connection" << std::endl;
	 status = DISCONNECTED;
         socket_.close();
      }

private:
   boost::thread m_io_service_thread;
   boost::asio::io_service tcp_io_service; // encapsulated IO service
   boost::asio::io_service& m_io_service; // the main IO service that runs this connection
   boost::mutex status_mutex;
   enStatus status;
   tcp::socket socket_; // the socket this instance is connected to
   char read_msg_[MAX_READ_LENGHT]; // data read from the socket
   deque<char> write_msgs_; // buffered write data
   std::queue<buffertype_ptr> m_write_queue;
   boost::mutex m_write_queue_mutex;
   std::queue<buffertype_ptr> m_rx_queue;
   boost::mutex m_rx_queue_mutex;
};

#endif
