#include "Input.h"
#include <memory>
#include <array>

namespace {
    // 最大ゲームパッド数
    static constexpr DWORD MAX_GAMEPADS = 4;

    // キーボードインスタンス
    std::unique_ptr<KeyBoard> s_keyboard;
    // マウスインスタンス
    std::unique_ptr<Mouse> s_mouse;
    // ゲームパッドインスタンス（最大4個）
    std::array<std::unique_ptr<GamePad>, MAX_GAMEPADS> s_gamepads;
}

// Inputシステムの初期化
void Input::initialize(HWND hwnd) {
    // キーボードの作成
    s_keyboard = std::make_unique<KeyBoard>(hwnd);
    // マウスの作成
    s_mouse = std::make_unique<Mouse>(hwnd);
    // ゲームパッドの作成（4プレイヤー分）
    for (DWORD i = 0; i < MAX_GAMEPADS; ++i) {
        s_gamepads[i] = std::make_unique<GamePad>(i);
    }
}

// Inputシステムの終了処理
void Input::finalize() {
    // ゲームパッドの破棄
    for (auto& pad : s_gamepads) {
        pad.reset();
    }
    // マウスの破棄
    s_mouse.reset();
    // キーボードの破棄
    s_keyboard.reset();
}

// 毎フレーム呼び出して入力状態を更新する
void Input::update() {
    // キーボードの更新
    s_keyboard->update();
    // マウスの更新
    s_mouse->update();
    // ゲームパッドの更新
    for (auto& pad : s_gamepads) {
        if (pad) {
            pad->update();
        }
    }
}

// キーが押されているか
bool Input::get_key(UINT key_code) {
    return s_keyboard->get_key(key_code);
}

// キーが押された瞬間
bool Input::get_key_down(UINT key_code) {
    return s_keyboard->get_key_down(key_code);
}

// キーが離された瞬間
bool Input::get_key_up(UINT key_code) {
    return s_keyboard->get_key_up(key_code);
}

// マウスボタンが押されているか
bool Input::get_mouse_button(UINT button) {
    return s_mouse->get_button(button);
}

// マウスボタンが押された瞬間
bool Input::get_mouse_button_down(UINT button) {
    return s_mouse->get_button_down(button);
}

// マウスボタンが離された瞬間
bool Input::get_mouse_button_up(UINT button) {
    return s_mouse->get_button_up(button);
}

// マウスの移動量を取得
LONG Input::get_mouse_delta_x() {
    return s_mouse->get_delta_x();
}

// マウスの移動量を取得
LONG Input::get_mouse_delta_y() {
    return s_mouse->get_delta_y();
}

// マウスホイールの回転量を取得
LONG Input::get_mouse_wheel() {
    return s_mouse->get_wheel();
}

// マウスカーソルのウィンドウ内座標を取得
LONG Input::get_mouse_position_x() {
    return s_mouse->get_position_x();
}

// マウスカーソルのウィンドウ内座標を取得
LONG Input::get_mouse_position_y() {
    return s_mouse->get_position_y();
}

// マウス位置の設定
void Input::set_mouse_position(LONG x, LONG y) {
    s_mouse->set_position(x, y);
}


// マウスカーソルの表示
void Input::show_mouse_cursor() {
    s_mouse->show_cursor();
}

// マウスカーソルの非表示
void Input::hide_mouse_cursor() {
    s_mouse->hide_cursor();
}

// マウスカーソルの表示状態を取得
bool Input::is_mouse_cursor_visible() {
    return s_mouse->is_cursor_visible();
}

// ゲームパッドが接続されているか
bool Input::is_gamepad_connected(DWORD player_index) {
    GamePad* pad = get_gamepad(player_index);
    if (pad) return pad->is_connected();
    return false;
}

// ゲームパッドのボタンが押されているか
bool Input::get_gamepad_button(WORD button, DWORD player_index) {
    GamePad* pad = get_gamepad(player_index);
    if (pad) return pad->get_button(button);
    return false;
}

// ゲームパッドのボタンが押された瞬間
bool Input::get_gamepad_button_down(WORD button, DWORD player_index) {
    GamePad* pad = get_gamepad(player_index);
    if (pad) return pad->get_button_down(button);
    return false;
}

// ゲームパッドのボタンが離された瞬間
bool Input::get_gamepad_button_up(WORD button, DWORD player_index) {
    GamePad* pad = get_gamepad(player_index);
    if (pad) return pad->get_button_up(button);
    return false;
}

// 左スティックの値を取得
float Input::get_gamepad_left_stick_x(DWORD player_index) {
    GamePad* pad = get_gamepad(player_index);
    if (pad) return pad->get_left_stick_x();
    return 0.0f;
}

// 左スティックの値を取得
float Input::get_gamepad_left_stick_y(DWORD player_index) {
    GamePad* pad = get_gamepad(player_index);
    if (pad) return pad->get_left_stick_y();
    return 0.0f;
}

// 右スティックの値を取得
float Input::get_gamepad_right_stick_x(DWORD player_index) {
    GamePad* pad = get_gamepad(player_index);
    if (pad) return pad->get_right_stick_x();
    return 0.0f;
}

// 右スティックの値を取得
float Input::get_gamepad_right_stick_y(DWORD player_index) {
    GamePad* pad = get_gamepad(player_index);
    if (pad) return pad->get_right_stick_y();
    return 0.0f;
}

// 左トリガーの値を取得
float Input::get_gamepad_left_trigger(DWORD player_index) {
    GamePad* pad = get_gamepad(player_index);
    if (pad) return pad->get_left_trigger();
    return 0.0f;
}

// 右トリガーの値を取得
float Input::get_gamepad_right_trigger(DWORD player_index) {
    GamePad* pad = get_gamepad(player_index);
    if (pad) return pad->get_right_trigger();
    return 0.0f;
}

// 振動の設定
void Input::set_gamepad_vibration(float left_motor, float right_motor, DWORD player_index) {
    GamePad* pad = get_gamepad(player_index);
    if (pad) pad->set_vibration(left_motor, right_motor);
}

// キーボードインスタンスの取得
KeyBoard* Input::get_keyboard() {
    return s_keyboard.get();
}

// マウスインスタンスの取得
Mouse* Input::get_mouse() {
    return s_mouse.get();
}

// ゲームパッドインスタンスの取得
GamePad* Input::get_gamepad(DWORD player_index) {
    if (player_index < MAX_GAMEPADS) {
        return s_gamepads[player_index].get();
    }
    return nullptr;
}

