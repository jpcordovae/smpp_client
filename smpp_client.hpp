#ifndef _SMPP_CLIENT_HPP_
#define _SMPP_CLIENT_HPP_

#include "smpp_defs.hpp"
#include "tcp_client.hpp"

#define MAX_STACK_SIZE 1024

//using namespace smpp_v34;

class smpp_client : public tcp_client
{
public:

   enum enSMPPStatus { SC_BIND_CONNECTED, SC_BIND_DISCONNECTED, SC_BIND_CONNECTING };

   enum enStackStatus { SS_FREE, SS_RESERVED, SS_SENDED };

   struct stack_payload {
      stack_payload():status(SS_FREE)
	 {}
      enStackStatus status;
      buffertype_ptr buffer;
   };

   typedef boost::shared_ptr<stack_payload> stack_payload_ptr;

   smpp_client(boost::asio::io_service &_io_service)
      :tcp_client(_io_service),
       m_smpp_status(SC_BIND_DISCONNECTED),
       rx_thread_exit(false)
      {
	 m_windowing = 10;
	 initialize_stack();
	 m_rx_thread = boost::thread(boost::bind(&smpp_client::rx_loop,this));
      }
   
   smpp_client(void)
      :m_smpp_status(SC_BIND_DISCONNECTED),
       rx_thread_exit(false)
      {
	 m_windowing = 10;
	 initialize_stack();
      }
   
   ~smpp_client()
      {
	 rx_thread_exit = true;
      };

   enSMPPStatus get_smpp_status(void)
      {
         boost::mutex::scoped_lock l(m_smpp_status_mutex);
         return m_smpp_status;
      }
   
   void bind_transmitter(const char *system_id, const char *password, const char *system_type, int interface_version, int addr_ton, int addr_npi, const char *address_range);
   void bind_receiver(void);
   void bind_transceiver(void);
   void unbind(void);
   void rx_handle(buffertype_ptr _bt_ptr);
   
private:

   void initialize_stack(void)
      {
	 // fill and maintain a normal 
	 for(size_t k=0; k < MAX_STACK_SIZE; k++)
	 {
	    m_sended_message_stack[k] = stack_payload_ptr(new stack_payload);
	 }
      }

   void rx_loop(void)
      {
	 while(!rx_thread_exit)
	 {
	    usleep(10000); // sleep 10ms
	    while(!rx_queue_empty())
	    {
	       rx_handle(get_rx_message());
	       pop_rx_queue();
	    }
	 }
	 
      }
   
   void set_smpp_status(enSMPPStatus _smpp_status)
      {
         boost::mutex::scoped_lock l(m_smpp_status_mutex);
         m_smpp_status = _smpp_status;
      }

   int reserve_stack_place(void)
      {
	 try
	 {
	    {// for scoped_lock
	       boost::mutex::scoped_lock l(stack_mutex);
	       for( int k=1; k < m_sended_message_stack.size() && k < m_windowing; k++ )
	       {
		  if( m_sended_message_stack[k]->status==SS_FREE )
		  {
		     m_sended_message_stack[k]->status==SS_RESERVED;
		     return k;
		  }
	       }
	    }
	    return -1; // no free space for a new message
	 }catch(std::exception &e)
	 {
	    std::cerr << e.what() << std::endl;
	 }
      }

   void bind_receiver_resp(buffertype_ptr _ptr);
   void bind_transmitter_resp(buffertype_ptr _ptr);
   void query_sm_resp(buffertype_ptr _ptr);
   void submit_sm_resp(buffertype_ptr _ptr);
   void deliver_sm_resp(buffertype_ptr _ptr);
   void unbind_resp(buffertype_ptr _ptr);
   void replace_sm_resp(buffertype_ptr _ptr);
   void cancel_sm_resp(buffertype_ptr _ptr);
   void bind_transceiver_resp(buffertype_ptr _ptr);
   void enquire_link_resp(buffertype_ptr _ptr);
   void submit_multi_resp(buffertype_ptr _ptr);
   void data_sm_resp(buffertype_ptr _ptr);

   bool rx_thread_exit;
   boost::thread m_rx_thread;
   enSMPPStatus m_smpp_status;
   boost::mutex m_smpp_status_mutex;
   std::map<int,stack_payload_ptr> m_sended_message_stack;
   std::queue<buffertype_ptr> m_pending_messages_queue;
   boost::mutex stack_mutex;
   size_t m_windowing;
};

#endif
