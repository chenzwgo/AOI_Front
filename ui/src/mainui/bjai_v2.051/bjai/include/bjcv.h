#ifndef BJCVRUNNER_H__
#define BJCVRUNNER_H__
#include "bjcv_common.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef _WIN32
#ifdef DLL_IMPLEMENT
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif
#else
#define DLL_API
#endif //  _WIN32

    namespace bjcv {
        DLL_API ErrorCode init_model(const char* model_dir, ModelHandle* model_handle_ptr);
        DLL_API ErrorCode inspect(Image* imgs_ptr, int img_num, BJCVResults* results_ptr, ModelHandle model_handle);
        DLL_API ErrorCode uninit_model(ModelHandle* model_handle);
    }

#ifdef __cplusplus
}
#endif

#endif
