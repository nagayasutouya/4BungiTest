#include "ControllerXInput.h"
#include <iomanip>

ControllerXInput::ControllerXInput(int id): controllerId(id){}

bool ControllerXInput::update(){
    prevState = state;
    XINPUT_STATE newState{};
    DWORD result = XInputGetState(controllerId, &newState);
    if (result == ERROR_SUCCESS){
        state = newState;
        connected = true;
    }
    else connected = false;
    return connected;
}

bool ControllerXInput::is_connected() const{
    return connected;
}

// 押されている間
bool ControllerXInput::is_pressed(WORD button) const{
    return (state.Gamepad.wButtons & button) != 0;
}

// 押された瞬間
bool ControllerXInput::is_trigger_down(WORD button) const{
    bool now = (state.Gamepad.wButtons & button) != 0;
    bool prev = (prevState.Gamepad.wButtons & button) != 0;
    return now && !prev;
}

// 離した瞬間
bool ControllerXInput::is_trigger_up(WORD button) const{
    bool now = (state.Gamepad.wButtons & button) != 0;
    bool prev = (prevState.Gamepad.wButtons & button) != 0;
    return !now && prev;
}
float ControllerXInput::get_left_stick_x_down(float dead_zone) const {
    bool now = fabs(state.Gamepad.sThumbLX) >= dead_zone;
    bool prev = fabs(prevState.Gamepad.sThumbLX) >= dead_zone;
    if (now && !prev)return (state.Gamepad.sThumbLX >= dead_zone) ? 1.0f : -1.0f;
    return 0.0f;
}
float ControllerXInput::get_left_stick_y_down(float dead_zone) const {
    bool now = fabs(state.Gamepad.sThumbLY) >= dead_zone;
    bool prev = fabs(prevState.Gamepad.sThumbLY) >= dead_zone;
    if (now && !prev)return (state.Gamepad.sThumbLY >= dead_zone) ? 1.0f : -1.0f;
    return 0.0f;
}
float ControllerXInput::get_right_stick_x_down(float dead_zone) const {
    bool now = fabs(state.Gamepad.sThumbRX) >= dead_zone;
    bool prev = fabs(prevState.Gamepad.sThumbRX) >= dead_zone;
    if (now && !prev)return (state.Gamepad.sThumbRX >= dead_zone) ? 1.0f : -1.0f;
    return 0.0f;
}
float ControllerXInput::get_right_stick_y_down(float dead_zone) const {
    bool now = fabs(state.Gamepad.sThumbRY) >= dead_zone;
    bool prev = fabs(prevState.Gamepad.sThumbRY) >= dead_zone;
    if (now && !prev)return (state.Gamepad.sThumbRY >= dead_zone) ? 1.0f : -1.0f;
    return 0.0f;
}

//スティック入力
float ControllerXInput::get_left_stick_x(float dead_zone) const {
    float input = static_cast<float>(state.Gamepad.sThumbLX) / stic_dec_;
    return (fabs(input) >= dead_zone) ? input : 0.0f;
}
float ControllerXInput::get_left_stick_y(float dead_zone) const {
    float input = static_cast<float>(state.Gamepad.sThumbLY) / stic_dec_;
    return (fabs(input) >= dead_zone) ? input : 0.0f;
}
float ControllerXInput::get_right_stick_x(float dead_zone) const {
    float input = static_cast<float>(state.Gamepad.sThumbRX) / stic_dec_;
    return (fabs(input) >= dead_zone) ? input : 0.0f;
}
float ControllerXInput::get_right_stick_y(float dead_zone) const {
    float input = static_cast<float>(state.Gamepad.sThumbRY) / stic_dec_;
    return (fabs(input) >= dead_zone) ? input : 0.0f;
}