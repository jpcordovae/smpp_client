#ifndef __SMPP_DEFS_HPP__
#define __SMPP_DEFS_HPP__

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <deque>
#include <iostream>
#include <vector>
#include <endian.h>

//namespace smpp_v34 {

typedef boost::uint8_t OCTET;
typedef boost::uint32_t INTEGER;

#define MESSAGE_MODE_FORWARD_AND_STORAGE 0x00
#define MESSAGE_MODE_DATAGRAM            0x01
#define MESSAGE_MODE_TRANSACTION_MODE    0x02

// smpp command_id set
#define GENERIC_NACK           0x80000000
#define BIND_RECEIVER          0x00000001
#define BIND_RECEIVER_RESP     0x80000001
#define BIND_TRANSMITTER       0x00000002
#define BIND_TRANSMITTER_RESP  0x80000002
#define QUERY_SM               0x00000003
#define QUERY_SM_RESP          0x80000003
#define SUBMIT_SM              0x00000004
#define SUBMIT_SM_RESP         0x80000004
#define DELIVER_SM             0x00000005
#define DELIVER_SM_RESP        0x80000005
#define UNBIND                 0x00000006
#define UNBIND_RESP            0x80000006
#define REPLACE_SM             0x00000007
#define REPLACE_SM_RESP        0x80000007
#define CANCEL_SM              0x00000008
#define CANCEL_SM_RESP         0x80000008
#define BIND_TRANSCEIVER       0x00000009
#define BIND_TRANSCEIVER_RESP  0x80000009
#define OUTBIND                0x0000000B
#define ENQUIRE_LINK           0x00000015
#define ENQUIRE_LINK_RESP      0x80000015
#define SUBMIT_MULTI           0x00000021
#define SUBMIT_MULTI_RESP      0x80000021
#define ALERT_NOTIFICATION     0x00000102
#define DATA_SM                0x00000103
#define DATA_SM_RESP           0x80000103

// error codes in command_status
#define ESME_ROK               0x00000000 // no error
#define ESME_RINVMSGLEN        0x00000001 // message lenght is invalid
#define ESME_RINVCMDLEN        0x00000002 // command lenght is invalid
#define ESME_RINVCMDID         0x00000003 // invalid command ID
#define ESME_RINVBNDSTS        0x00000004 // incorrect BIND status for given command
#define ESME_RALYBND           0x00000005 // ESME alreaddy in bound state
#define ESME_RINVPRTFLG        0x00000006 // invalid priority flag
#define ESME_RINVREGDLVFLG     0x00000007 // invalid registered priority flag
#define ESME_RSYSERR           0x00000008 // system error
// 0x00000009 reserved
#define ESME_INVSRCADR         0x0000000A // invalid source address
#define ESME_RINVDSTADR        0x0000000B // invalid dest address
#define ESME_RINVMSGID         0x0000000C // message ID is invalid
#define ESME_RBINDFAIL         0x0000000D // bind failed
#define ESME_RINVPASWD         0x0000000E // invalid password
#define ESME_RINVSYSID         0x0000000F // invalid system ID
// 0x00000010 reserved
#define ESME_RCANCELFAIL       0x00000011 // cancel SM failed
// 0x00000012 reserved
#define ESME_RREPLACEFAIL      0x00000013 // replace SM failed
#define ESME_RMSGQFULL         0x00000014 // message queue full
#define ESME_RINVSERTYP        0x00000015 // invalid service type
// 16 to 32 reserved !
#define ESME_RINVNUMDESTS      0x00000033 // invalid number of destinations
#define ESME_RINVDLNAME        0x00000034 // invalid distribution list name
// 35 to 3F reserved
#define ESME_RINVDESTFLAG      0x00000040 // destination flag is invalid (submit_multi)
// 0x00000041 reserved
#define ESME_RINVSUBREP        0x00000042 // invalid 'submit with replace' request (i.e. submit_sm with replace_if_present_flag set)
#define ESME_RINVESMCLASS      0x00000043 // invalid esm_class field data
#define ESME_RCNTSUBDL         0x00000044 // cannot submit to ditribution list
#define ESME_RSUBMITFAIL       0x00000045 // submit_sm or submit_multi failed
// 46 to 47 reserved
#define ESME_RINVSRCTON        0x00000048 // invalid source address TON
#define ESME_RINVSRCNPI        0x00000049 // invalid source address NPI
#define ESME_RINVDSTTON        0x00000050 // invalid destination address TON
#define ESME_RINVDSTNPI        0x00000051 // invalid destination address NPI
// 0x00000052 reserved
#define ESME_RINVSYSTYP        0x00000053 // invalid system_type field
#define ESME_RINVREPFLAG       0x00000054 // invalid replace_if_present flag
#define ESME_RINVNUMMSGS       0x00000055 // invalid number of messages
// 56 and 57 reserved
#define ESME_RTHROTTLED        0x00000058 // Throttling error (ESME has exceeded allowed message limits
// 59 and 60 reserved
#define ESME_RINVSCHED         0x00000061 // Invalid shcedule delivery time
#define ESME_RINVEXPIRITY      0x00000062 // invalid message validity period (Expiry time)
#define ESME_RINVDFTMSGID      0x00000063 // predefined message invalid or not found
#define ESME_RX_T_APPN         0x00000064 // ESME receiver temporary app error code
#define ESME_RX_P_APPN         0x00000065 // ESME receiver permanent app error code
#define ESME_RX_R_APPN         0x00000066 // ESME receiver reject message error code
#define ESME_RQUERYFAIL        0x00000067 // query_sm request failed
// 68 to BF reserved
#define ESME_RINVOPTPARSTREAM  0x000000C0 // error in the optional parto of the PDU body
#define ESME_ROPTPARNOTALLWD   0x000000C1 // optional parameter nor allowed
#define ESME_RINVPARLEN        0x000000C2 // invalid parameter lenght
#define ESME_RMISSINGOPTPARAM  0x000000C3 // expected optional parameter missing
#define ESME_RINVOPTPARAMVAL   0x000000C4 // invalid optional parameter value
// C5 to FD reserved
#define ESME_RDELIVERYFAILURE  0x000000FE // deliver failure (used for data_sm_resp)
#define ESME_RUNKNOWNERR       0x000000FF // unknown error
// 100 to 3FF reserved for SMPP extension
// 400 to 4FF reserved for SMSC vendor specific errors
// 500 to 0xFFFFFFFF reserved

#define INTERFACE_VERSION 0x34

// TON values
#define TON_UNKNOWN            0x00
#define TON_INTERNATIONAL      0x01
#define TON_NATIONAL           0x02
#define TON_NETWORK_SPECIFIC   0x03
#define TON_SUBSCRIBER_NUMBER  0x04
#define TON_ALPHANUMERIC       0x05
#define TON_ABBREVIATED        0x06

// NPI values
#define NPI_UNKNOWN            0x00
#define NPI_ISDN               0x01
#define NPI_DATA               0x03
#define NPI_TELEX              0x04
#define NPI_LAND_MOBILE        0x05
#define NPI_NATIONAL           0x08
#define NPI_PRIVATE            0x09
#define NPI_ERMES              0x0A
#define NPI_INTERNET           0x0E
#define NPI_WAP                0x12

// service_type parameters
#define ST_NULL                ""        // null
#define ST_CMT                 "CMT"     // cellular messaging
#define ST_CPT                 "CPT"     // cellular paging
#define ST_VMN                 "VMN"     // voice mail notification
#define ST_VMA                 "VMA"     // voice mail alerting
#define ST_WAP                 "WAP"     // wireless application protocol
#define ST_USSD                "USSD"    // unstructured dupplementary services data

// *******************
// esm_class
// The esm_class parameter is used to indicate special message
// attributes associated with the short message.
// The esm_class parameter is encoded in submit_sm, submmit_multi and data_sm (ESME->SMSC) PDU
#define ES_ESM_CLASS_MESSAGING_MODE_DEFAULT           0x00   // store and forward
#define ES_ESM_CLASS_MESSAGING_MODE_DATAGRAM          0x01   // datagram mode
#define ES_ESM_CLASS_MESSAGING_MODE_FORWARD           0x02   // forward (i.e. Transaction) mode
#define ES_ESM_CLASS_MESSAGING_MODE_STORE_AND_FORWARD 0x03   // Store and Forward mode. Use to select Store and Forward mode if Default SMSC Mode is non Store and Forward
#define ES_ESM_CLASS_MESSAGE_TYPE_DEFAULT             0x00   // default message type
#define ES_ESM_CLASS_MESSAGE_TYPE_WEDA                0x08   // Short Message contains ESME Delivery Acknowledgment
#define ES_ESM_CLASS_MESSAGE_TYPE_WEMUA               0x10   // Short Message contains ESME Manual/User Acknowlegment
#define ES_ESM_CLASS_GSM_FEATURE_NONE                 0x00   // no specific feature
#define ES_ESM_CLASS_GSM_FEATURE_UDHI                 0x10   // UDHI Indicator (only relevant for MT short messages)
#define ES_ESM_CLASS_GSM_FEATURE_SRP                  0x20   // Set Reply Path (onlo relevant for GSM network)
#define ES_ESM_CLASS_GSM_FEATURE_SUARP                0x30   // Set UDHO and Reply Path (only relevant for GSM network)
// for deliver_sm and data_sm (SMSC->ESME)PDU
//#define SE_ESM_CLASS_MESSAGE_MODE // not applicable for SMSC-> ESME, ignore bits 0 and 1
#define SE_ESM_CLASS_MESSAGE_TYPE_DEFAULT            0x00   // default message type
#define SE_ESM_CLASS_MESSAGE_TYPE_SMCSDR             0x04   // Short Message contains SMSC Delivery Receipt
#define SE_ESM_CLASS_MESSAGE_TYPE_SMCSDAR            0x08   // Short Message contains SME Delivery Acknowledgment
//#define SE_ESM_CLASS_MESSAGE_TYPE_DEFAULT          0x09   // reserved
#define SE_ESM_CLASS_MESSAGE_TYPE_SMCSMUA            0x10   // Short Message contains SME Manual/User Acknowledgment
//#define SE_ESM_CLASS_MESSAGE_TYPE_DEFAULT          0x14   // reserved
#define SE_ESM_CLASS_MESSAGE_TYPE_SMCCA              0x18   // Short Message contains Conversation Abort (Koream CDMA)
//#define SE_ESM_CLASS_MESSAGE_TYPE_DEFAULT          0x19   // reserved
#define SE_ESM_CLASS_MESSAGE_TYPE_SMCIDN             0x20   // Short Message contains Intermediate Delivery Notification
#define SE_ESM_CLASS_GSM_FEATURE_NONE                0x00   // no specific features selected
#define SE_ESM_CLASS_GSM_FEATURE_UDHI                0x40   // UDHI indicator set
#define SE_ESM_CLASS_GSM_FEATURE_REPLY               0x80   // Reply Path
#define SE_ESM_CLASS_GSM_FEATURE_UDHI_REPLY          0xC0   // UDHO and Reply Path

//#define ESM_CLASS(MESSAGING_MODE,MESSAGE_TYPE,GSM_FEATURE) ((MESSAGING_MODE) | (MESSAGE_TYPE) | (GSM_FEATURE))

//******************
// priority_flag
#define PRIORITY_FLAG_LOWEST  0x00 // level 0, lowest priority
#define PRIORITY_FLAG_NORMAL  0x01 // level 1 priority
#define PRIORITY_FLAG_HIGH    0x02 // level 2 priority
#define PRIORITY_FLAG_HIGHEST 0x03 // level 3, highest priority

//*****************
// schedule_priority_time

//*****************
// validity_period

//*****************
// registered_delivery
// SMSC Delivery Receipt (bits 0 and 1)
#define RD_SDR_NSDRR    0x00 // No SMSC Delivery Receipt requested
#define RD_SDR_SDRRF    0x01 // SMSC Delivery Recepit requested where final delivery outcome is delivery success or failure
#define RD_SDR_SDRRS    0x10 // SMSC Delivery Receipt requested where the final delivery outcome is delivery failure
//#define RD_SDR_         0x11 // reserved
// SME Originated Acknowledgement (bits 3 and 4)
#define RD_SOA_NRSAR    0x00 // No recipient SME Acknowldgement requested (default)
#define RD_SOA_SDAR     0x04 // SME Delivery Acknowledgement requested
#define RD_SOA_SMUAR    0x08 // SME Manual/User Acknowledgement requested
#define RD_SOA_BDMUAR   0x09 // Both DFelivery and Manual/User Acknowledgement requested
// Intermediate Notifications (bit 5)
#define RD_IN_NINR      0x00 // No Intermediate notification requested (default)
#define RD_IN_INR       0x10 // Intermediate notification requested

//****************
// replace_if_present_flag
// is used to reques the SMSC to replace a previously submitted mesage, that is still pending delivery. Must have same service_type then the message to be replaced
#define RIPF_NOT_REPLACE    0x00 // Don't replace
#define RIPF_REPLACE        0x01 // replace

struct PDU_HEADER {
   INTEGER command_lenght;
   INTEGER command_id;
   INTEGER command_status;
   INTEGER sequence_number;
};

struct PDU_OPTIONAL_PARAMETER {
   INTEGER tag;
   INTEGER lenght;
   OCTET *value;
};

struct PDU_BODY {
   OCTET *mandatory_parameters;
   OCTET *optional_parameters;
};

struct PDU {
   PDU_HEADER *header;
   PDU_BODY *body;
};

//enum enOperations { BIND, UNBIND, GNERIC_NACK, SUBMIT_SM, SUBMIT_MULTI, DELIVER_SM, DATA_SM, QUERY_SM, CANCEL_SM, REPLACE_SM, ENQUIRE_LINK, ALERT_NOTIFICATION };

struct bind_transmitter_mandatory_parameters {
   OCTET *system_id; // max 16 octet string
   OCTET *password; // max 9 octet string
   OCTET *system_type; // max 13 octet string
   INTEGER interface_version; // indicates the version of the SMPP protocol supported by ESME
   INTEGER addr_ton; // Indicates Type of Number of the ESME address. if not known set to NULL
   INTEGER addr_npi; // Numbering plan indicator for ESME address. If not known set to NULL
   OCTET *address_range; // The ESME address, if not known, set to NULL.
};

void dump_buffer(boost::uint8_t *_buffer, size_t _size);
//}

typedef std::vector<boost::uint8_t> buffertype;
typedef boost::shared_ptr<buffertype> buffertype_ptr;


#endif //END __SMPP_DEFS_HPP__
