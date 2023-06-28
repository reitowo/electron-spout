// It is sad, that we are inevitably map data from CPU to GPU, as electron offscreen rendering
// relies heavily on CPU canvas and bitmap.

#include <node_api.h>
#include "spout_output.h"

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    SpoutOutput::Init(env, exports);
    return exports;
}

NODE_API_MODULE(addon, Init)
