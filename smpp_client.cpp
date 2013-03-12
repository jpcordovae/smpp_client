#include "smpp_client.hpp"

void smpp_client::bind_transmitter(const char *system_id,
				   const char *password,
				   const char *system_type,
				   INTEGER interface_version,
				   INTEGER addr_ton,
				   INTEGER addr_npi,
				   const char *address_range)
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

void smpp_client::bind_receiver(const char *system_id,
				const char *password,
				const char *system_type,
				INTEGER interface_version,
				INTEGER addr_ton,
				INTEGER addr_npi,
				const char *address_range)
{
   if(get_smpp_status()!=SC_BIND_DISCONNECTED)
   {
      std::cerr << "bind_receiver fail, already connected or connecting. You must unbind first." << std::endl;
      return;
   }

   set_smpp_status(SC_BIND_CONNECTING);

   std::cout << "bind receiver" << std::endl;
   set_smpp_status(SC_BIND_CONNECTING);
   size_t body_lenght = 3 + strlen(system_id) + 1 + strlen(password) + 1 + strlen(system_type) + 1 + strlen(address_range) + 1;
   
   size_t total_size = body_lenght + 16;
   
   boost::uint8_t *buffer = new boost::uint8_t[total_size];
   bzero(buffer,total_size);

   buffer[3] = (boost::uint8_t)total_size;
   buffer[7] = (boost::uint8_t)BIND_RECEIVER; //command_id
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

void smpp_client::bind_transceiver(const char *system_id,
				   const char *password,
				   const char *system_type,
				   INTEGER interface_version,
				   INTEGER addr_ton,
				   INTEGER addr_npi,
				   const char *address_range)
{
   if(get_smpp_status()!=SC_BIND_DISCONNECTED)
   {
      std::cerr << "bind_transceiver fail, already connected or connecting. You must unbind first." << std::endl;
      return;
   }
   std::cout << "dinb transceiver" << std::endl;
   set_smpp_status(SC_BIND_CONNECTING);
   size_t body_lenght = 3 + strlen(system_id) + 1 + strlen(password) + 1 + strlen(system_type) + 1 + strlen(address_range) + 1;
   
   size_t total_size = body_lenght + 16;
   
   boost::uint8_t *buffer = new boost::uint8_t[total_size];
   bzero(buffer,total_size);

   buffer[3] = (boost::uint8_t)total_size;
   buffer[7] = (boost::uint8_t)BIND_TRANSCEIVER; //command_id
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

void smpp_client::unbind(void)
{
   std::cout << "unbinding method" << std::endl;
   if(get_smpp_status()==SC_BIND_CONNECTING)
   {
      std::cerr << "unbind command fail, smpp status is SC_BIND_CONNECTING, you must finish binding handshake, or close TCP connection with tcp_close() method." << std::endl;
      return;
   }
   
   if(get_smpp_status()==SC_BIND_DISCONNECTED)
   {
      std::cerr << "unbind command fail, smpp status is SC_BIND_DISCONNECTED." << std::endl;
      return;
   }

   PDU_HEADER  *pdu_header = new PDU_HEADER;
   bzero(pdu_header,sizeof(PDU_HEADER));
   pdu_header->command_lenght = 16;
   pdu_header->command_id = UNBIND;
   pdu_header->command_status = 0x00000000;
   if((pdu_header->sequence_number=reserve_stack_place()) < 0)
   {
      std::cerr << "cannot get sequence number to send unbind message, try to close socket witn tcp_close() function." << std::endl;
      delete pdu_header;
      return;
   }
   
   buffertype_ptr buffer_ptr(new buffertype());
   buffer_ptr->assign((boost::uint8_t *)pdu_header,(boost::uint8_t *)pdu_header+sizeof(PDU_HEADER));
   
   delete pdu_header;
#if __BYTE_ORDER == __LITTLE_ENDIAN
   swap_pdu_header(buffer_ptr);
#endif
   //dump_buffer(buffer_ptr->data(),buffer_ptr->size());
   write(buffer_ptr);
   
   { // this is for the scoped_lock
      boost::mutex::scoped_lock(stack_mutex);
      m_sended_message_stack[pdu_header->sequence_number]->status = SS_SENDED;
   }
}

void smpp_client::rx_handle(buffertype_ptr _bt_ptr)
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
   swap_pdu_header(_bt_ptr);
#endif
   free_stack_place(*(((boost::uint32_t *)_bt_ptr->data())+3)); // sequence_number
   switch(*(((boost::uint32_t *)_bt_ptr->data())+1)) // command_id
   {
   case GENERIC_NACK:
      std::cout << "generic nack response" << std::endl;
      break;
   case BIND_RECEIVER_RESP:
      std::cout << "bind receiver response" << std::endl;
      bind_receiver_resp(_bt_ptr);
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
      std::cout << "unbind response" << std::endl;
      unbind_resp(_bt_ptr);
      break;
   case REPLACE_SM_RESP:
      break;
   case CANCEL_SM_RESP:
      break;
   case BIND_TRANSCEIVER_RESP:
      std::cout << "bind transceiver response" << std::endl;
      bind_transceiver_resp(_bt_ptr);
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

   void send_message(char *service_type,
		     INTEGER source_addr_ton,
		     INTEGER source_addr_npi,
		     char *source_addr,
		     INTEGER dest_addr_ton,
		     INTEGER dest_addr_npi,
		     char *destination_addr,
		     INTEGER esm_class,
		     INTEGER protocol_id,
		     INTEGER priority_flag,
		     char *schedule_delivery_time,
		     char *validity_period,
		     INTEGER registered_delivery,
		     INTEGER replace_if_present_flag,
		     INTEGER data_coding,
		     INTEGER sm_default_msg_id,
		     INTEGER sm_lenght,
		     char *short_message) // 0 to 254 octet string
   {
      size_t total_lenght = sizeof(INTEGER) * 12 + 6 +
	 strlen(service_type) +
	 strlen(source_addr) +
	 strlen(destination_addr) +
	 strlen(schedule_delivery_time) +
	 strlen(validity_period) +
	 strlen(short_message) + 16;

      // check sizes of strings
      if(strlen(service_type)>=6) { std::cerr << "service_type in send message can't be bigger then 5 characters" << std::endl; return;}
      if(strlen(source_addr)>=21) { std::cerr << "source_addr in send message can't be bigger then 20 characters" << std::endl; return;}
      if(strlen(destination_addr)>=21) { std::cerr << "destination_addr in send message can't be bigger then 20 characters" << std::endl; return;}
      if(strlen(schedule_delivery_time)>=17) { std::cerr << "schedule_delivery_time in send message can't be bigger then 16 characters" << std::endl; return;}
      if(strlen(validity_period)>=17) { std::cerr << "delivery_period in send message can't be bigger then 16 characters" << std::endl; return;}
      if(strlen(short_message)>=254) { std::cerr << "shot_message in send message can't be bigger then 253 characters" << std::endl; return;}

      buffertype_ptr bt_ptr = buffertype_ptr(new buffertype);
      
      PDU_HEADER *pdu_header = (PDU_HEADER *)bt_ptr->data();
      pdu_header->command_lenght = total_lenght;
      pdu_header->command_id = SUBMIT_SM;
      pdu_header->command_status = 0x00000000;
      if((pdu_header->sequence_number=reserve_stack_place()) < 0)
      {
	 std::cerr << "cannot get sequence number to send unbind message, try to close socket witn tcp_close() function." << std::endl;
	 return;
      }
      
      
   }


void smpp_client::bind_receiver_resp(buffertype_ptr _bt_ptr)
{
      if(*(((boost::uint32_t *)_bt_ptr->data())+2)!=ESME_ROK)
   {
      set_smpp_status(SC_BIND_DISCONNECTED);
      boost::mutex::scoped_lock l(command_status_error_strings_mutex);
      std::cout << "bind transmitter response error" << command_status_error_strings[*(((boost::uint32_t *)_bt_ptr->data())+2)] << std::endl;
      // if has been an error, the system_id is not returned, so we must return now
      return;
   }
   set_smpp_status(SC_BIND_CONNECTED);
   std::cout << "binded as receiver with SMSC identifier as " << (char*)(((boost::uint32_t *)_bt_ptr->data())+4)  << std::endl;

}

void smpp_client::bind_transmitter_resp(buffertype_ptr _bt_ptr)
{
   if(*(((boost::uint32_t *)_bt_ptr->data())+2)!=ESME_ROK)
   {
      set_smpp_status(SC_BIND_DISCONNECTED);
      boost::mutex::scoped_lock l(command_status_error_strings_mutex);
      std::cout << "bind transmitter response error" << command_status_error_strings[*(((boost::uint32_t *)_bt_ptr->data())+2)] << std::endl;
      // if has been an error, the system_id is not returned, so we must return now
      return;
   }
   set_smpp_status(SC_BIND_CONNECTED);
   std::cout << "binded as transmitter with SMSC identifier as " << (char*)(((boost::uint32_t *)_bt_ptr->data())+4)  << std::endl;
   
}

void smpp_client::query_sm_resp(buffertype_ptr _ptr)
{
}

void smpp_client::submit_sm_resp(buffertype_ptr _ptr)
{
}

void smpp_client::deliver_sm_resp(buffertype_ptr _ptr)
{
}

void smpp_client::unbind_resp(buffertype_ptr _bt_ptr)
{
   if(*(((boost::uint32_t *)_bt_ptr->data())+2)!=ESME_ROK)
   {
      //set_smpp_status(SC_BIND_DISCONNECTED);
      boost::mutex::scoped_lock l(command_status_error_strings_mutex);
      std::cout << "unbind response error: " << command_status_error_strings[*(((boost::uint32_t *)_bt_ptr->data())+2)] << std::endl;
      // if has been an error, the system_id is not returned, so we must return now
      return;
   }
   set_smpp_status(SC_BIND_DISCONNECTED);
   std::cout << "unbinded succesfully" << (char*)(((boost::uint32_t *)_bt_ptr->data())+4)  << std::endl;
   
}

void smpp_client::replace_sm_resp(buffertype_ptr _ptr)
{
}

void smpp_client::cancel_sm_resp(buffertype_ptr _ptr)
{
}

void smpp_client::bind_transceiver_resp(buffertype_ptr _bt_ptr)
{
   if(*(((boost::uint32_t *)_bt_ptr->data())+2)!=ESME_ROK)
   {
      set_smpp_status(SC_BIND_DISCONNECTED);
      boost::mutex::scoped_lock l(command_status_error_strings_mutex);
      std::cout << "bind transmitter response error" << command_status_error_strings[*(((boost::uint32_t *)_bt_ptr->data())+2)] << std::endl;
      // if has been an error, the system_id is not returned, so we must return now
      return;
   }
   set_smpp_status(SC_BIND_CONNECTED);
   std::cout << "binded as tranceiver with SMSC identifier as " << (char*)(((boost::uint32_t *)_bt_ptr->data())+4)  << std::endl;

   
}

void smpp_client::enquire_link_resp(buffertype_ptr _ptr)
{
}

void smpp_client::submit_multi_resp(buffertype_ptr _ptr)
{
}

void smpp_client::data_sm_resp(buffertype_ptr _ptr)
{
}


