#ifndef __SMPP_PDU__H__
#define __SMPP_PDU__H__

#include "smpp_defs.hpp"

class smpp_pdu
{
public:
   smpp_pdu():pdu(NULL)
      {}
   
   smpp_pdu(butffertype_ptr _ptr):pdu(NULL)
      {
	 pdu = _ptr->data();
	 pdu->header = _ptr->data(); // begin of the struct
	 pdu->body = _ptr->data()+16; // begin of the body
      }
   
   INTEGER get_command_lenght(void)
      {
	 return pdu->header->command_lenght;
      }
   
   INTEGER get_command_id(void)
      {
	 return pdu->header->command_id;
      }
   
   INTEGER get_command_status(void)
      {
	 return pdu->header->command_status;
      }
   
   INTEGER get_sequence_number(void)
      {
	 return pdu->heade->sequence_number;
      }
   
   PDU *pdu;
   
};

#endif
