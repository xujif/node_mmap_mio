#include <napi.h>
#include "mio.hpp"

Napi::Object InitAll(Napi::Env env, Napi::Object exports)
{
    MioInit(env, exports);
    return exports;
}
NODE_API_MODULE(addon, InitAll)