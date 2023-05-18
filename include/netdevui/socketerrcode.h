#ifndef __ERROR_CODE_H__
#define __ERROR_CODE_H__

typedef int ERROR_CODE;
#define SOCKET_ERR	-2000


enum EN_SOKECT_ERR_CODE
{
    ERR_OK = 0,
    ERR_THREAD_START = -1000,
    ERR_SOCKET = SOCKET_ERR,
    ERR_SOCKET_CREATE,
    ERR_SOCKET_BIND,
    ERR_SOCKET_LISTEN,
    ERR_SOCKET_RECV_TIMEOUT,
    ERR_SOCKET_RECV_ERROR,
    ERR_SOCKET_SEND_TIMEOUT,
    ERR_SOCKET_SEND_ERROR,
    ERR_SOCKET_CONNECT_ERROR,
    ERR_SOCKET_NOT_REGISTER_MSG,
    ERR_SOCKET_SERVER_EXIT,
    ERR_SOCKET_SERVER_INIT,
    ERR_PARSER_JSON,
    ERR_PARSER_JSON_KEY_TARGET,
    ERR_PARSER_JSON_KEY_TYPE,
    ERR_PARSER_JSON_KEY_FUN,
    ERR_PARSER_JSON_KEY_SN,
    ERR_PARSER_JSON_KEY_STATUS,
    ERR_PARSER_JSON_KEY_BODY,
	ERR_PACKET_NULL,
    ERR_PACKET_ACK_TIMEOUT,
    ERR_PACKET_ACK_ERROR,
    ERR_PACKET_ACK_BODY_ERROR,
};

static const char* errorCodeToStr(int iCode)
{
    switch (iCode)
    {
        case ERR_OK:                        return "OK";
        case ERR_THREAD_START:              return "ERR_THREAD_START";
        case ERR_SOCKET_CREATE:             return "ERR_SOCKET_CREATE";
        case ERR_SOCKET_BIND:               return "ERR_SOCKET_BIND";
        case ERR_SOCKET_LISTEN:             return "ERR_SOCKET_RECV_TIMEOUT";
        case ERR_SOCKET_RECV_ERROR:         return "ERR_SOCKET_RECV_ERROR";
        case ERR_SOCKET_SEND_TIMEOUT:       return "ERR_SOCKET_SEND_TIMEOUT";
        case ERR_SOCKET_SEND_ERROR:         return "ERR_SOCKET_SEND_ERROR";
        case ERR_SOCKET_NOT_REGISTER_MSG:   return "ERR_SOCKET_NOT_REGISTER_MSG";
        case ERR_PARSER_JSON:               return "ERR_PARSER_JSON";
        case ERR_PARSER_JSON_KEY_TARGET:    return "ERR_PARSER_JSON_KEY_TARGET";
        case ERR_PARSER_JSON_KEY_TYPE:      return "ERR_PARSER_JSON_KEY_TYPE";
        case ERR_PARSER_JSON_KEY_FUN:       return "ERR_PARSER_JSON_KEY_FUN";
        case ERR_PARSER_JSON_KEY_SN:        return "ERR_PARSER_JSON_KEY_SN";
        case ERR_PARSER_JSON_KEY_STATUS:    return "ERR_PARSER_JSON_KEY_STATUS";
        case ERR_PARSER_JSON_KEY_BODY:      return "ERR_PARSER_JSON_KEY_BODY";
        default:                            return "UnKonw";
    }
    return "UnKonw";
}

#endif
