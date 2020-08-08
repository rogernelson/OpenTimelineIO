#pragma once

#include "anyDictionary.h"
#include "errorStatus.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif
    struct FreezeFrame;
    typedef struct FreezeFrame FreezeFrame;

    FreezeFrame* FreezeFrame_create(const char* name, AnyDictionary* metadata);
    double       FreezeFrame_time_scalar(FreezeFrame* self);
    void FreezeFrame_set_time_scalar(FreezeFrame* self, double time_scalar);
    char* FreezeFrame_effect_name(FreezeFrame* self);
    void
                   FreezeFrame_set_effect_name(FreezeFrame* self, const char* effect_name);
    char*    FreezeFrame_name(FreezeFrame* self);
    void           FreezeFrame_set_name(FreezeFrame* self, const char* name);
    AnyDictionary* FreezeFrame_metadata(FreezeFrame* self);
    bool           FreezeFrame_to_json_file(
                 FreezeFrame*     self,
                 const char*      file_name,
                 OTIOErrorStatus* error_status,
                 int              indent);
    char* FreezeFrame_to_json_string(
        FreezeFrame* self, OTIOErrorStatus* error_status, int indent);
    bool
    FreezeFrame_is_equivalent_to(FreezeFrame* self, OTIOSerializableObject* other);
    FreezeFrame*
                FreezeFrame_clone(FreezeFrame* self, OTIOErrorStatus* error_status);
    char* FreezeFrame_schema_name(FreezeFrame* self);
    int         FreezeFrame_schema_version(FreezeFrame* self);
#ifdef __cplusplus
}
#endif
