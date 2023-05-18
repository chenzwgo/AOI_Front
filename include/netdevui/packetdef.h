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
#define PACKET_FUN_INQUIRE_PACKET   "inquire_packet"
#define PACKET_FUN_SEND_PRODUCT_RESULT           "send_product_result"
#define PACKET_FUN_SEND_PRODUCT_DATA_RESULT           "send_product_data_result"
#define PACKET_FUN_SEND_PRODUCT_SN           "send_product_sn"
#define PACKET_FUN_SEND_ALARM           "send_alarm"

#define PACKET_FUN_SYS_START        "sys_start"
#define PACKET_FUN_SYS_STOP         "sys_stop"
#define PACKET_FUN_SYS_RESET        "sys_reset"
#define PACKET_FUN_SYS_PAUSE         "sys_pause"

#define PACKET_FUN_QUERY_SYS_STATUS        "query_sys_status"
#define PACKET_FUN_QUERY_PRODUCT_SN        "query_product_sn"

#define PACKET_STATUS_OK				    "OK"
#define PACKET_STATUS_ERROR				    "Error"
#define PACKET_STATUS_TIMEOUT               "Timeout"
#define PACKET_STATUS_FUN_NOEXIST           "funnoexist"


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
