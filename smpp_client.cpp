#include "smpp_client.hpp"

void smpp_client::bind_transmitter(const char *system_id, const char *password,
				   const char *system_type, int interface_version,
				   int addr_ton, int addr_npi, const char *address_range)
{
   std::cout << "binding transmitter..." << std::endl;
   if(get_smpp_status()!=SC_BIND_DISCONNECTED)
   {
      std::cerr << "bind_transmitter fail, already connected or connecting. You must unbind first." << std::endl;
      return;
   }
   set_smpp_status(SC_BIND_CONNECTING); 
   
   size_t body_lenght = strlen(system_id) + 1 +
                        strlen(password) + 1 +
                        strlen(system_type) + 1 +  3  +
                        strlen(address_range) + 1;
   
   size_t total_size = body_lenght + 16;
   
   boost::uint8_t *buffer = new boost::uint8_t[total_size];
   bzero(buffer,total_size);

   buffer[3] = (boost::uint8_t)total_size;
   buffer[7] = (boost::uint8_t)BIND_TRANSMITTER; //command_id
   buffer[11] = 0x00; // command_status
   size_t sequence_number = reserve_stack_place();
   buffer[15] = (boost::uint8_t)sequence_number; // sequence_number
   
   int index = 16;
   memcpy( buffer + index, system_id, strlen(system_id) + 1 );
   index += strlen(system_id) + 1;
   memcpy( buffer + index, password, strlen(password) + 1 );
   index += strlen(password) + 1;
   memcpy( buffer + index, system_type, strlen(system_type) + 1 );
   index += strlen(system_type) + 1;
   memcpy( buffer + index, &interface_version,1);
   index++;
   memcpy( buffer + index, &addr_ton, 1);
   index++;
   memcpy( buffer + index, &addr_npi, 1);
   index++;
   memcpy( buffer + index, address_range, strlen(address_range) + 1 );
   //dump_buffer(buffer,total_size);
   write(buffer,total_size);
   
   { // this is for the scoped_lock
      boost::mutex::scoped_lock(stack_mutex);
      m_sended_message_stack[sequence_number]->status = SS_SENDED;
   }
   
   delete[] buffer;
}

void smpp_client::bind_receiver(void)
{
   if(get_smpp_status()!=SC_BIND_DISCONNECTED)
   {
      std::cerr << "bind_receiver fail, already connected or connecting. You must unbind first." << std::endl;
      return;
   }

   set_smpp_status(SC_BIND_CONNECTING);
   
}

void smpp_client::bind_transceiver(void)
{
   if(get_smpp_status()!=SC_BIND_DISCONNECTED)
   {
      std::cerr << "bind_transceiver fail, already connected or connecting. You must unbind first." << std::endl;
      return;
   }

   set_smpp_status(SC_BIND_CONNECTING);
   
}

void smpp_client::unbind(void)
{
   if(get_smpp_status()!=SC_BIND_DISCONNECTED)
   {
      std::cerr << "unbind fail, already unbinded." << std::endl;
      return;
   }

   set_smpp_status(SC_BIND_DISCONNECTED);
}

void smpp_client::rx_handle(buffertype_ptr _bt_ptr)
{
   //dump_buffer(_bt_ptr->data(),_bt_ptr->size());
   boost::uint32_t tmp;
#if __BYTE_ORDER == __LITTLE_ENDIAN
   tmp = __bswap_32(*(((boost::uint32_t *)_bt_ptr->data())+1));
#endif
   
   switch(tmp)
   {
   case GENERIC_NACK:
      break;
   case BIND_RECEIVER_RESP:
      break;
   case BIND_TRANSMITTER_RESP:
      std::cout << "bind transmitter response" << std::endl;
      bind_transmitter_resp(_bt_ptr);
      break;
   case QUERY_SM_RESP:
      break;
   case SUBMIT_SM_RESP:
      break;
   case DELIVER_SM_RESP:
      break;
   case UNBIND_RESP:
      break;
   case REPLACE_SM_RESP:
      break;
   case CANCEL_SM_RESP:
      break;
   case BIND_TRANSCEIVER_RESP:
      std::cout << "bind transceiver response" << std::endl;
      bind_transceiver_response(_bt_ptr);
      break;
   case ENQUIRE_LINK_RESP:
      break;
   case SUBMIT_MULTI_RESP:
      break;
   case DATA_SM_RESP:
      break;
   default:
      break;
   }
}

void smpp_client::bind_receiver_resp(buffertype_ptr _ptr)
{
}

void smpp_client::bind_transmitter_resp(buffertype_ptr _ptr)
{
   
}

void smpp_client::query_sm_resp(buffertype_ptr _ptr)
{}

void smpp_client::submit_sm_resp(buffertype_ptr _ptr)
{}

void smpp_client::deliver_sm_resp(buffertype_ptr _ptr)
{}

void smpp_client::unbind_resp(buffertype_ptr _ptr)
{}

void smpp_client::replace_sm_resp(buffertype_ptr _ptr)
{}

void smpp_client::cancel_sm_resp(buffertype_ptr _ptr)
{}

void smpp_client::bind_transceiver_resp(buffertype_ptr _ptr)
{}

void smpp_client::enquire_link_resp(buffertype_ptr _ptr)
{}

void smpp_client::submit_multi_resp(buffertype_ptr _ptr)
{}

void smpp_client::data_sm_resp(buffertype_ptr _ptr)
{}


