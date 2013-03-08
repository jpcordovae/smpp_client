#include "smpp_defs.hpp"
#include "smpp_client.hpp"

void on_connect(void)
{

}

void on_bind(void)
{

}

int main(int argc, char* argv[])
{
#ifdef POSIX
   termios stored_settings;
   tcgeattr(0,&stored_settings);
   termios new_settings = stored_settings;
   new_settings.c_lflags &= (~ICANON);
   new_settings.c_lflags &= (~ISIG);
   tcseattr(0,TCSANOW,&new_settings);
#endif

   
  try
  {
    if (argc < 2)
    {
      std::cerr << "Usage: smpp_client <host> <port>\n";
      return 1;
    }

    set_up_command_status_error_string_codes();

    boost::asio::io_service _io_service;
    
    smpp_client c(_io_service);

    boost::thread t(boost::bind(&boost::asio::io_service::run,&_io_service));
    
    c.connect(argv[1],argv[2]);
    
    while(c.get_tcp_status()!=tcp_client::CONNECTED)
    {
       sleep(1);
    }
    
    /*c.bind_transmitter("GK_TRX_MOBILEID",
		       "z6ik8tdj",
		       "BulkRD",
		       0x34,
		       0x00,
		       0x00,
		       "");*/

    //c.bind_transmitter("smppclient1","password","",0x034,0x00,0x00,"");    
    //c.bind_receiver("smppclient1","password","",0x034,0x00,0x00,"");
    c.bind_transceiver("smppclient1","password","",0x034,0x00,0x00,"");
    
    while(1)
    {
       char ch;
       cin.get(ch);
       if(ch == 3 || c.get_tcp_status()==tcp_client::DISCONNECTED)
       {
	  break;
       }
       c.write(ch);
    }
    
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
