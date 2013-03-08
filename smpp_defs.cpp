#include "smpp_defs.hpp"

boost::mutex command_status_error_strings_mutex;
std::map<boost::uint32_t,std::string> command_status_error_strings;

void dump_buffer(boost::uint8_t *_buffer, size_t _size)
{
   for (int k=0;k<_size;k++)
   {
      if( !(k%4) && k>1 )
      {
	 printf("\n");
      }
      
      printf("0x%.2X \'%-1c\'     ",_buffer[k],_buffer[k]);
   }
   printf("\n");
}

// swap byte ordening for little endian architectures
void swap_pdu_header(buffertype_ptr _bt_ptr)
{
   *(((boost::uint32_t *)_bt_ptr->data()))   = __bswap_32(*(((boost::uint32_t *)_bt_ptr->data())));
   *(((boost::uint32_t *)_bt_ptr->data())+1) = __bswap_32(*(((boost::uint32_t *)_bt_ptr->data())+1));
   *(((boost::uint32_t *)_bt_ptr->data())+2) = __bswap_32(*(((boost::uint32_t *)_bt_ptr->data())+2));
   *(((boost::uint32_t *)_bt_ptr->data())+3) = __bswap_32(*(((boost::uint32_t *)_bt_ptr->data())+3));
}

void set_up_command_status_error_string_codes(void)
{
   std::cout << "setting up command status error strings...";
// error codes in command_status
   command_status_error_strings[ESME_ROK] = "no error";
   command_status_error_strings[ESME_RINVMSGLEN] = "message lenght is invalid";
   command_status_error_strings[ESME_RINVCMDLEN] = "command lenght is invalid";
   command_status_error_strings[ESME_RINVCMDID] = "invalid command ID";
   command_status_error_strings[ESME_RINVBNDSTS] = "incorrect BIND status for given command";
   command_status_error_strings[ESME_RALYBND] = "ESME alreaddy in bound stateE";
   command_status_error_strings[ESME_RINVPRTFLG] ="invalid priority flagE";
   command_status_error_strings[ESME_RINVREGDLVFLG] = "invalid registered priority flag";
   command_status_error_strings[ESME_RSYSERR] = "system error";
// 0x00000009 reserved
   command_status_error_strings[ESME_INVSRCADR] = "invalid source address";
   command_status_error_strings[ESME_RINVDSTADR] = "invalid dest address";
   command_status_error_strings[ESME_RINVMSGID] = "message ID is invalid";
   command_status_error_strings[ESME_RBINDFAIL] = "bind failed";
   command_status_error_strings[ESME_RINVPASWD] = "invalid password";
   command_status_error_strings[ESME_RINVSYSID] = "invalid system ID";
// 0x00000010 reserved
   command_status_error_strings[ESME_RCANCELFAIL] = "cancel SM failed";
// 0x00000012 reserved
   command_status_error_strings[ESME_RREPLACEFAIL] = "replace SM failed";
   command_status_error_strings[ESME_RMSGQFULL] = "message queue full";
   command_status_error_strings[ESME_RINVSERTYP] = "invalid service type";
// 16 to 32 reserved !
   command_status_error_strings[ESME_RINVNUMDESTS] = "invalid number of destinations";
   command_status_error_strings[ESME_RINVDLNAME] = "invalid distribution list name";
// 35 to 3F reserved
   command_status_error_strings[ESME_RINVDESTFLAG] = "destination flag is invalid (submit_multi)";
// 0x00000041 reserved
   command_status_error_strings[ESME_RINVSUBREP] = "invalid 'submit with replace' request (i.e. submit_sm with replace_if_present_flag set)";
   command_status_error_strings[ESME_RINVESMCLASS] = "invalid esm_class field data";
   command_status_error_strings[ESME_RCNTSUBDL] = "cannot submit to ditribution list";
   command_status_error_strings[ESME_RSUBMITFAIL] = "submit_sm or submit_multi failed";
// 46 to 47 reserved
   command_status_error_strings[ESME_RINVSRCTON] = "invalid source address TON";
   command_status_error_strings[ESME_RINVSRCNPI] = "invalid source address NPI";
   command_status_error_strings[ESME_RINVDSTTON] = "invalid destination address TON";
   command_status_error_strings[ESME_RINVDSTNPI] = "invalid destination address NPI";
// 0x00000052 reserved
   command_status_error_strings[ESME_RINVSYSTYP] = "invalid system_type field";
   command_status_error_strings[ESME_RINVREPFLAG] = "invalid replace_if_present flag";
   command_status_error_strings[ESME_RINVNUMMSGS] = "invalid number of messages";
// 56 and 57 reserved
   command_status_error_strings[ESME_RTHROTTLED] = "Throttling error (ESME has exceeded allowed message limits";
// 59 and 60 reserved
   command_status_error_strings[ESME_RINVSCHED] = "Invalid shcedule delivery time";
   command_status_error_strings[ESME_RINVEXPIRITY] = "invalid message validity period (Expiry time)";
   command_status_error_strings[ESME_RINVDFTMSGID] = "predefined message invalid or not found";
   command_status_error_strings[ESME_RX_T_APPN] = "ESME receiver temporary app error code";
   command_status_error_strings[ESME_RX_P_APPN] = "ESME receiver permanent app error code";
   command_status_error_strings[ESME_RX_R_APPN] = "ESME receiver reject message error code";
   command_status_error_strings[ESME_RQUERYFAIL] = "query_sm request failed";
// 68 to BF reserved
   command_status_error_strings[ESME_RINVOPTPARSTREAM] = "error in the optional parto of the PDU body";
   command_status_error_strings[ESME_ROPTPARNOTALLWD] = "optional parameter nor allowed";
   command_status_error_strings[ESME_RINVPARLEN] = "invalid parameter lenght";
   command_status_error_strings[ESME_RMISSINGOPTPARAM] = "expected optional parameter missing";
   command_status_error_strings[ESME_RINVOPTPARAMVAL] = "invalid optional parameter value";
// C5 to FD reserved
   command_status_error_strings[ESME_RDELIVERYFAILURE] = "deliver failure (used for data_sm_resp)";
   command_status_error_strings[ESME_RUNKNOWNERR] = "unknown error";
   std::cout << "OK" << std::endl;
}
