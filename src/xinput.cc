/**
 * XInput native binding for Node.js/Electron (Windows)
 * Provides access to Xbox controllers via Microsoft's XInput API
 */
#include <napi.h>
#include <windows.h>
#include <xinput.h>

#define MAX_CONTROLLERS 4

/**
 * Get the state of a specific controller (0-3)
 * Returns { connected: bool, gamepad?: { buttons, triggers, sticks, packetNumber } }
 */
Napi::Object GetControllerState(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "Controller index (0-3) required").ThrowAsJavaScriptException();
        return Napi::Object::New(env);
    }
    
    int controllerIndex = info[0].As<Napi::Number>().Int32Value();
    if (controllerIndex < 0 || controllerIndex >= MAX_CONTROLLERS) {
        Napi::RangeError::New(env, "Controller index must be 0-3").ThrowAsJavaScriptException();
        return Napi::Object::New(env);
    }
    
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));
    
    DWORD result = XInputGetState(controllerIndex, &state);
    
    Napi::Object obj = Napi::Object::New(env);
    obj.Set("connected", Napi::Boolean::New(env, result == ERROR_SUCCESS));
    
    if (result == ERROR_SUCCESS) {
        Napi::Object gamepad = Napi::Object::New(env);
        
        // Buttons (bitmask decoded to object)
        WORD buttons = state.Gamepad.wButtons;
        Napi::Object btns = Napi::Object::New(env);
        btns.Set("dpadUp", Napi::Boolean::New(env, (buttons & XINPUT_GAMEPAD_DPAD_UP) != 0));
        btns.Set("dpadDown", Napi::Boolean::New(env, (buttons & XINPUT_GAMEPAD_DPAD_DOWN) != 0));
        btns.Set("dpadLeft", Napi::Boolean::New(env, (buttons & XINPUT_GAMEPAD_DPAD_LEFT) != 0));
        btns.Set("dpadRight", Napi::Boolean::New(env, (buttons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0));
        btns.Set("start", Napi::Boolean::New(env, (buttons & XINPUT_GAMEPAD_START) != 0));
        btns.Set("back", Napi::Boolean::New(env, (buttons & XINPUT_GAMEPAD_BACK) != 0));
        btns.Set("leftThumb", Napi::Boolean::New(env, (buttons & XINPUT_GAMEPAD_LEFT_THUMB) != 0));
        btns.Set("rightThumb", Napi::Boolean::New(env, (buttons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0));
        btns.Set("leftShoulder", Napi::Boolean::New(env, (buttons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0));
        btns.Set("rightShoulder", Napi::Boolean::New(env, (buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0));
        btns.Set("a", Napi::Boolean::New(env, (buttons & XINPUT_GAMEPAD_A) != 0));
        btns.Set("b", Napi::Boolean::New(env, (buttons & XINPUT_GAMEPAD_B) != 0));
        btns.Set("x", Napi::Boolean::New(env, (buttons & XINPUT_GAMEPAD_X) != 0));
        btns.Set("y", Napi::Boolean::New(env, (buttons & XINPUT_GAMEPAD_Y) != 0));
        gamepad.Set("buttons", btns);
        
        // Triggers (0-255 normalized to 0.0-1.0)
        gamepad.Set("leftTrigger", Napi::Number::New(env, state.Gamepad.bLeftTrigger / 255.0));
        gamepad.Set("rightTrigger", Napi::Number::New(env, state.Gamepad.bRightTrigger / 255.0));
        
        // Thumbsticks (-32768 to 32767 normalized to -1.0 to 1.0)
        Napi::Object leftStick = Napi::Object::New(env);
        leftStick.Set("x", Napi::Number::New(env, state.Gamepad.sThumbLX / 32767.0));
        leftStick.Set("y", Napi::Number::New(env, state.Gamepad.sThumbLY / 32767.0));
        gamepad.Set("leftStick", leftStick);
        
        Napi::Object rightStick = Napi::Object::New(env);
        rightStick.Set("x", Napi::Number::New(env, state.Gamepad.sThumbRX / 32767.0));
        rightStick.Set("y", Napi::Number::New(env, state.Gamepad.sThumbRY / 32767.0));
        gamepad.Set("rightStick", rightStick);
        
        // Packet number (for change detection)
        gamepad.Set("packetNumber", Napi::Number::New(env, state.dwPacketNumber));
        
        obj.Set("gamepad", gamepad);
    }
    
    return obj;
}

/**
 * Get indices of all connected controllers
 * Returns number[] (e.g., [0, 1] if controllers 0 and 1 are connected)
 */
Napi::Array GetConnectedControllers(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::Array result = Napi::Array::New(env);
    
    uint32_t index = 0;
    for (int i = 0; i < MAX_CONTROLLERS; i++) {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));
        
        if (XInputGetState(i, &state) == ERROR_SUCCESS) {
            result.Set(index++, Napi::Number::New(env, i));
        }
    }
    
    return result;
}

/**
 * Set controller vibration
 * setVibration(index, leftMotor, rightMotor) - motors are 0.0-1.0
 */
Napi::Boolean SetVibration(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Controller index, left motor, right motor required").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }
    
    int controllerIndex = info[0].As<Napi::Number>().Int32Value();
    double leftMotor = info[1].As<Napi::Number>().DoubleValue();
    double rightMotor = info[2].As<Napi::Number>().DoubleValue();
    
    if (controllerIndex < 0 || controllerIndex >= MAX_CONTROLLERS) {
        return Napi::Boolean::New(env, false);
    }
    
    XINPUT_VIBRATION vibration;
    ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
    vibration.wLeftMotorSpeed = (WORD)(leftMotor * 65535);
    vibration.wRightMotorSpeed = (WORD)(rightMotor * 65535);
    
    DWORD result = XInputSetState(controllerIndex, &vibration);
    return Napi::Boolean::New(env, result == ERROR_SUCCESS);
}

/**
 * Check if XInput is available (always true on Windows Vista+)
 */
Napi::Boolean IsAvailable(const Napi::CallbackInfo& info) {
    return Napi::Boolean::New(info.Env(), true);
}

/**
 * Get battery information for a controller
 */
Napi::Object GetBatteryInfo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "Controller index (0-3) required").ThrowAsJavaScriptException();
        return Napi::Object::New(env);
    }
    
    int controllerIndex = info[0].As<Napi::Number>().Int32Value();
    
    Napi::Object obj = Napi::Object::New(env);
    
    XINPUT_BATTERY_INFORMATION battery;
    DWORD result = XInputGetBatteryInformation(controllerIndex, BATTERY_DEVTYPE_GAMEPAD, &battery);
    
    if (result == ERROR_SUCCESS) {
        const char* typeStr = "Unknown";
        switch (battery.BatteryType) {
            case BATTERY_TYPE_DISCONNECTED: typeStr = "Disconnected"; break;
            case BATTERY_TYPE_WIRED: typeStr = "Wired"; break;
            case BATTERY_TYPE_ALKALINE: typeStr = "Alkaline"; break;
            case BATTERY_TYPE_NIMH: typeStr = "NiMH"; break;
            case BATTERY_TYPE_UNKNOWN: typeStr = "Unknown"; break;
        }
        obj.Set("type", Napi::String::New(env, typeStr));
        
        const char* levelStr = "Unknown";
        switch (battery.BatteryLevel) {
            case BATTERY_LEVEL_EMPTY: levelStr = "Empty"; break;
            case BATTERY_LEVEL_LOW: levelStr = "Low"; break;
            case BATTERY_LEVEL_MEDIUM: levelStr = "Medium"; break;
            case BATTERY_LEVEL_FULL: levelStr = "Full"; break;
        }
        obj.Set("level", Napi::String::New(env, levelStr));
    }
    
    return obj;
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

