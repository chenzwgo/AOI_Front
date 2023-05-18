#ifndef LIBSVM_INTERFACE_H_
#define LIBSVM_INTERFACE_H_

#include "dv.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#ifdef _WIN32
#ifdef LIBSVM_EXPORTS
#define LIBSVM_DLL __declspec(dllexport)
#else
#define LIBSVM_DLL __declspec(dllimport)
#endif
#else
#define LIBSVM_DLL
#endif

    typedef void *PredictorHandle;

    LIBSVM_DLL int SvmTrain(const char *sample_path, const char *model_path);

    LIBSVM_DLL int SvmLoadModel(const char *model_path, PredictorHandle *handle);

    LIBSVM_DLL int SvmRunTestSample(const char *sample_path, PredictorHandle handle);

    LIBSVM_DLL int SvmPredict_DV(PredictorHandle handle, dv::DefectAttr *defectAttr);

    LIBSVM_DLL int SvmPredict(PredictorHandle handle, const double *data, int size);

    LIBSVM_DLL int SvmFreeModel(PredictorHandle *handle);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // LIBLINEAR_INTERFACE_H_
