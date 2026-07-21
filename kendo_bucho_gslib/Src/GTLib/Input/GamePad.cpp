#include "GamePad.h"
#include <cmath>

#pragma comment(lib, "xinput.lib")

// コンストラクタ
GamePad::GamePad(DWORD player_index) : player_index_{ player_index } {
}

// デストラクタ
GamePad::~GamePad() {
    // 振動を停止する
    set_vibration(0.0f, 0.0f);
}

// 毎フレーム呼び出してパッド状態を更新する
void GamePad::update() {
    // 前フレームのボタン状態を保存
    previous_buttons_ = current_buttons_;
    // パッドの状態を取得
    XINPUT_STATE state{};
    DWORD result = XInputGetState(player_index_, &state);
    connected_ = (result == ERROR_SUCCESS);
    if (connected_) {
        // ボタン状態を更新
        current_buttons_ = state.Gamepad.wButtons;
        // 左スティック（デッドゾーン適用）
        left_stick_x_ = apply_deadzone(state.Gamepad.sThumbLX,
            XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
        left_stick_y_ = apply_deadzone(state.Gamepad.sThumbLY,
            XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
        // 右スティック（デッドゾーン適用）
        right_stick_x_ = apply_deadzone(state.Gamepad.sThumbRX,
            XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
        right_stick_y_ = apply_deadzone(state.Gamepad.sThumbRY,
            XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
        // トリガー（デッドゾーン適用、0.0～1.0に正規化）
        left_trigger_ = (state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
            ? (float)(state.Gamepad.bLeftTrigger) / 255.0f : 0.0f;
        right_trigger_ = (state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
            ? (float)(state.Gamepad.bRightTrigger) / 255.0f : 0.0f;
    }
    else {
        // 未接続の場合はリセット
        current_buttons_ = 0;
        left_stick_x_ = left_stick_y_ = 0.0f;
        right_stick_x_ = right_stick_y_ = 0.0f;
        left_trigger_ = right_trigger_ = 0.0f;
    }
}

// パッドが接続されているか
bool GamePad::is_connected() const {
    return connected_;
}

// ボタンが押されている間trueを返す
bool GamePad::get_button(WORD button) const {
    return (current_buttons_ & button) != 0;
}

// ボタンが押された瞬間のみtrueを返す
bool GamePad::get_button_down(WORD button) const {
    return (current_buttons_ & button) != 0 && (previous_buttons_ & button) == 0;
}

// ボタンが離された瞬間のみtrueを返す
bool GamePad::get_button_up(WORD button) const {
    return (current_buttons_ & button) == 0 && (previous_buttons_ & button) != 0;
}

// 左スティックの値を取得
float GamePad::get_left_stick_x() const {
    return left_stick_x_;
}
float GamePad::get_left_stick_y() const {
    return left_stick_y_;
}

// 右スティックの値を取得
float GamePad::get_right_stick_x() const {
    return right_stick_x_;
}
float GamePad::get_right_stick_y() const {
    return right_stick_y_;
}

// トリガーの値を取得
float GamePad::get_left_trigger() const {
    return left_trigger_;
}
float GamePad::get_right_trigger() const {
    return right_trigger_;
}

// 振動を設定（0.0～1.0）
void GamePad::set_vibration(float left_motor, float right_motor) {
    XINPUT_VIBRATION vibration{};
    vibration.wLeftMotorSpeed = (WORD)(left_motor * 65535.0f);
    vibration.wRightMotorSpeed = (WORD)(right_motor * 65535.0f);
    XInputSetState(player_index_, &vibration);
}

// スティックのデッドゾーンを適用する(-1.0～1.0に正規化)
float GamePad::apply_deadzone(SHORT value, SHORT deadzone) {
    if (value > deadzone) {
        return (float)(value - deadzone) / (32767.0f - deadzone);
    }
    else if (value < -deadzone) {
        return (float)(value + deadzone) / (32767.0f - deadzone);
    }
    return 0.0f;
}