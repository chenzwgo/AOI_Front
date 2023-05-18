#ifndef _PACKET_DEF_H_
#define _PACKET_DEF_H_
#include <string>
#include <map>
using namespace std;

#define PACKET_END_CH           "#"
#define PACKET_KEY_TARGET       "packet_target"
#define PACKET_KEY_TYPE         "packet_type"
#define PACKET_KEY_FUN          "packet_fun"
#define PACKET_KEY_SN           "packet_sn"
#define PACKET_KEY_STATUS       "packet_status"
#define PACKET_KEY_BODY         "packet_body"


#define PACKET_TARGET_SERVER      "Server"
#define PACKET_TARGET_PC1         "PC1"
#define PACKET_TARGET_PC2         "PC2"
#define PACKET_TARGET_PC3         "PC3"

#define PACKET_TYPE_REQUEST      "request"
#define PACKET_TYPE_ACK          "ack"
#define PACKET_TYPE_INQUIRE      "inquire"



#define PACKET_FUN_REGISTER         "register"
#define PACKET_FUN_SET_CONFIG       "set_config"
#define PACKET_FUN_GET_CONFIG       "get_config"
#define PACKET_FUN_HEARTBEAT        "heartbeat"
#define PACKET_FUN_GET_IO           "get_io_status"
#define PACKET_FUN_SET_IO           "set_io_status"
#define PACKET_FUN_GET_BATCH_IO     "get_batch_io_status"
#define PACKET_FUN_INQUIRE_PACKET   "inquire_packet"
#define PACKET_FUN_SEND_PRODUCT_RESULT           "send_product_result"

#define PACKET_FUN_SYS_START        "sys_start"
#define PACKET_FUN_SYS_STOP         "sys_stop"
#define PACKET_FUN_SYS_RESET        "sys_reset"


#define PACKET_STATUS_OK				    "OK"
#define PACKET_STATUS_ERROR				    "Error"
#define PACKET_STATUS_TIMEOUT               "Timeout"
#define PACKET_STATUS_FUN_NOEXIST           "funnoexist"

#define PACKET_FUN_STATUS                   "status"

//#define PACKET_STATUS_FUN_GET_PICK_INFO_2_1     "GET_PICK_INFO_2,1"
//#define PACKET_STATUS_FUN_GET_PICK_STATUS_2_1   "GET_PICK_STATUS_2,1"  /*查询1#取载板位的状态*/
//#define PACKET_STATUS_FUN_GET_PIACE_STATUS_2_1  "GET_PIACE_STATUS_2,1"  /*查询1#放载板位的状态*/
//#define PACKET_STATUS_FUN_SET_PICK_STATUS_2_1   "SET_PICK_STATUS_2,1"  /*设置1#取载板位的状态*/
//#define PACKET_STATUS_FUN_SET_PIACE_STATUS_2_1  "SET_PIACE_STATUS_2,1"  /*设置1#放载板位的状态*/
//
//#define PACKET_STATUS_FUN_GET_PICK_INFO_2_2     "GET_PICK_INFO_2,2"
//#define PACKET_STATUS_FUN_GET_PICK_STATUS_2_2   "GET_PICK_STATUS_2,2"
//#define PACKET_STATUS_FUN_GET_PIACE_STATUS_2_2  "GET_PIACE_STATUS_2,2"
//#define PACKET_STATUS_FUN_SET_PICK_STATUS_2_2   "SET_PICK_STATUS_2,2"
//#define PACKET_STATUS_FUN_SET_PIACE_STATUS_2_2  "SET_PIACE_STATUS_2,2"
//
//#define PACKET_STATUS_FUN_GET_PICK_INFO_3_3     "GET_PICK_INFO_3,3"
//#define PACKET_STATUS_FUN_GET_PICK_STATUS_3_3   "GET_PICK_STATUS_3,3"
//#define PACKET_STATUS_FUN_GET_PIACE_STATUS_3_3  "GET_PIACE_STATUS_3,3"
//#define PACKET_STATUS_FUN_SET_PICK_STATUS_3_3   "SET_PICK_STATUS_3,3"
//#define PACKET_STATUS_FUN_SET_PIACE_STATUS_3_3  "SET_PIACE_STATUS_3,3"
//
//#define PACKET_STATUS_FUN_GET_PICK_INFO_3_4     "GET_PICK_INFO_3,4"
//#define PACKET_STATUS_FUN_GET_PICK_STATUS_3_4   "GET_PICK_STATUS_3,4"
//#define PACKET_STATUS_FUN_GET_PIACE_STATUS_3_4  "GET_PIACE_STATUS_3,4"
//#define PACKET_STATUS_FUN_SET_PICK_STATUS_3_4   "SET_PICK_STATUS_3,4"
//#define PACKET_STATUS_FUN_SET_PIACE_STATUS_3_4  "SET_PIACE_STATUS_3,4"


#define PACKET_STRING   "\
                        {\
                            \"packet_target\": \"%s\",\
                            \"packet_type\": \"%s\",\
                            \"packet_fun\": \"%s\",\
                            \"packet_sn\": \"%s\",\
                            \"packet_status\": \"%s\",\
                            \"packet_body\": {%s} \
                        }\
                        "



typedef struct stPacket
{
    string target;
    string type;
    string fun;
    string sn;
    string status;
    string body;
	map<string, string> mapBody;
}stPacket;


#endif
