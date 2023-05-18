#ifndef C_LOGG_H_
#define C_LOGG_H_

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
#define DEEP_VISION_EXTERN_C extern "C"
#else
#define DEEP_VISION_EXTERN_C
#endif // __cplusplus

#ifdef _WIN32
#ifdef LOG_LIB_EXPORTS
#define DV_LOG_DLL DEEP_VISION_EXTERN_C __declspec(dllexport)
#else
#define DV_LOG_DLL DEEP_VISION_EXTERN_C __declspec(dllimport)
#endif
#else
#define DV_LOG_DLL DEEP_VISION_EXTERN_C
#endif // _WIN32

enum log_error_code {
    LogErrorNone       = 0,
    LogErrorInitFailed = 1
};

enum log_rank_t {
    LOG_DEBUG   = 1,
    LOG_INFO    = 2,
    LOG_WARNING = 4,
    LOG_ERROR   = 8
};

// Init log file
// level                  The log level to be logged, default:0xe, INFO|WARNING|ERROR message will be logged.
//                                You can log DEBUG and ERROR logs using "LOG_DEBUG|LOG_ERROR"
// LogPath                Log storage directory, default: ./log
//                                Or enter a specific directory in the following format: E:/log_path
// LogNamePrefix          prefix of log file name, default:nullptr
DV_LOG_DLL int initLogger(int level = 0xe, const char* LogPath = nullptr, const char* LogNamePrefix = nullptr);

// Close log file
DV_LOG_DLL void closeLogger();

DV_LOG_DLL void logStart(log_rank_t rank, const char* info, 
                long int line = 0,
                const char* file = nullptr, 
                const char* func = nullptr, 
                bool simple = true);


// Record logs with different output streams according to different levels
#define LOGGER(log_rank, info)   \
    logStart(log_rank, info, __LINE__, __FILE__, __FUNCTION__, false)

#define LOGGER_SIMP(log_rank, info)   \
    logStart(log_rank, info)

#endif  // C_LOGG_H_
