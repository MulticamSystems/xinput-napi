/**
 * XInput stub for non-Windows platforms
 * Returns empty/false values - Xbox controllers use HID on macOS/Linux
 */
#include <napi.h>

Napi::Object GetControllerState(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::Object obj = Napi::Object::New(env);
    obj.Set("connected", Napi::Boolean::New(env, false));
    return obj;
}

Napi::Array GetConnectedControllers(const Napi::CallbackInfo& info) {
    return Napi::Array::New(info.Env());
}

Napi::Boolean SetVibration(const Napi::CallbackInfo& info) {
    return Napi::Boolean::New(info.Env(), false);
}

Napi::Boolean IsAvailable(const Napi::CallbackInfo& info) {
    return Napi::Boolean::New(info.Env(), false);
}

Napi::Object GetBatteryInfo(const Napi::CallbackInfo& info) {
    return Napi::Object::New(info.Env());
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("getControllerState", Napi::Function::New(env, GetControllerState));
    exports.Set("getConnectedControllers", Napi::Function::New(env, GetConnectedControllers));
    exports.Set("setVibration", Napi::Function::New(env, SetVibration));
    exports.Set("isAvailable", Napi::Function::New(env, IsAvailable));
    exports.Set("getBatteryInfo", Napi::Function::New(env, GetBatteryInfo));
    return exports;
}

NODE_API_MODULE(xinput, Init)

