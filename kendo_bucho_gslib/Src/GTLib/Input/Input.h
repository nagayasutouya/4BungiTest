#ifndef INPUT_H_
#define INPUT_H_

#include "KeyBoard.h"
#include "Mouse.h"
#include "GamePad.h"

// Inputシステム
namespace Input {
    // -----------------------------------------------------
    // システム制御
    // -----------------------------------------------------
    // Inputシステムの初期化
    void initialize(HWND hwnd);
    // Inputシステムの終了処理
    void finalize();
    // 毎フレーム呼び出して入力状態を更新する
    void update();

    // -----------------------------------------------------
    // キーボード
    // -----------------------------------------------------
    // キーが押されているか（押している間true）
    bool get_key(UINT key_code);
    // キーが押された瞬間（押した瞬間のみtrue）
    bool get_key_down(UINT key_code);
    // キーが離された瞬間（離した瞬間のみtrue）
    bool get_key_up(UINT key_code);

    // -----------------------------------------------------
    // マウス
    // -----------------------------------------------------
    // マウスボタンが押されているか
    bool get_mouse_button(UINT button);
    // マウスボタンが押された瞬間
    bool get_mouse_button_down(UINT button);
    // マウスボタンが離された瞬間
    bool get_mouse_button_up(UINT button);
    // マウスの移動量を取得
    LONG get_mouse_delta_x();
    LONG get_mouse_delta_y();
    // マウスホイールの回転量を取得
    LONG get_mouse_wheel();
    // マウスカーソルのウィンドウ内座標を取得
    LONG get_mouse_position_x();
    LONG get_mouse_position_y();
    // マウス位置の設定
    void set_mouse_position(LONG x, LONG y);
    // マウスカーソルの表示
    void show_mouse_cursor();
    // マウスカーソルの非表示
    void hide_mouse_cursor();
    // マウスカーソルの表示状態を取得
    bool is_mouse_cursor_visible();

    // -----------------------------------------------------
    // ゲームパッド
    // -----------------------------------------------------
    // ゲームパッドが接続されているか
    bool is_gamepad_connected(DWORD player_index = 0);
    // ゲームパッドのボタンが押されているか
    bool get_gamepad_button(WORD button, DWORD player_index = 0);
    // ゲームパッドのボタンが押された瞬間
    bool get_gamepad_button_down(WORD button, DWORD player_index = 0);
    // ゲームパッドのボタンが離された瞬間
    bool get_gamepad_button_up(WORD button, DWORD player_index = 0);
    // 左スティックの値を取得（-1.0～1.0）
    float get_gamepad_left_stick_x(DWORD player_index = 0);
    float get_gamepad_left_stick_y(DWORD player_index = 0);
    // 右スティックの値を取得（-1.0～1.0）
    float get_gamepad_right_stick_x(DWORD player_index = 0);
    float get_gamepad_right_stick_y(DWORD player_index = 0);
    // トリガーの値を取得（0.0～1.0）
    float get_gamepad_left_trigger(DWORD player_index = 0);
    float get_gamepad_right_trigger(DWORD player_index = 0);
    // 振動の設定（0.0～1.0）
    void set_gamepad_vibration(float left_motor, float right_motor, DWORD player_index = 0);

    // -----------------------------------------------------
    // 個別インスタンスへのアクセス
    // -----------------------------------------------------
    // キーボードインスタンスの取得
    KeyBoard* get_keyboard();
    // マウスインスタンスの取得
    Mouse* get_mouse();
    // ゲームパッドインスタンスを取得
    GamePad* get_gamepad(DWORD player_index = 0);

} // namespace Input

#endif 

