#ifndef GAME_PAD_H_
#define GAME_PAD_H_

#include <windows.h>
#include <Xinput.h>

// ゲームパッドクラス（XInput）
class GamePad {
public:
    // ボタン定数
    static constexpr WORD BUTTON_DPAD_UP = XINPUT_GAMEPAD_DPAD_UP;
    static constexpr WORD BUTTON_DPAD_DOWN = XINPUT_GAMEPAD_DPAD_DOWN;
    static constexpr WORD BUTTON_DPAD_LEFT = XINPUT_GAMEPAD_DPAD_LEFT;
    static constexpr WORD BUTTON_DPAD_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT;
    static constexpr WORD BUTTON_START = XINPUT_GAMEPAD_START;
    static constexpr WORD BUTTON_BACK = XINPUT_GAMEPAD_BACK;
    static constexpr WORD BUTTON_LEFT_THUMB = XINPUT_GAMEPAD_LEFT_THUMB;
    static constexpr WORD BUTTON_RIGHT_THUMB = XINPUT_GAMEPAD_RIGHT_THUMB;
    static constexpr WORD BUTTON_LEFT_SHOULDER = XINPUT_GAMEPAD_LEFT_SHOULDER;
    static constexpr WORD BUTTON_RIGHT_SHOULDER = XINPUT_GAMEPAD_RIGHT_SHOULDER;
    static constexpr WORD BUTTON_A = XINPUT_GAMEPAD_A;
    static constexpr WORD BUTTON_B = XINPUT_GAMEPAD_B;
    static constexpr WORD BUTTON_X = XINPUT_GAMEPAD_X;
    static constexpr WORD BUTTON_Y = XINPUT_GAMEPAD_Y;

    // コンストラクタ（player_index: 0～3）
    GamePad(DWORD player_index = 0);
    // デストラクタ
    ~GamePad();

    // 毎フレーム呼び出してパッド状態を更新する
    void update();
    // パッドが接続されているか
    bool is_connected() const;
    // ボタンが押されている間trueを返す
    bool get_button(WORD button) const;
    // ボタンが押された瞬間のみtrueを返す
    bool get_button_down(WORD button) const;
    // ボタンが離された瞬間のみtrueを返す
    bool get_button_up(WORD button) const;
    // 左スティックの値を取得（-1.0～1.0）
    float get_left_stick_x() const;
    float get_left_stick_y() const;
    // 右スティックの値を取得（-1.0～1.0）
    float get_right_stick_x() const;
    float get_right_stick_y() const;
    // トリガーの値を取得（0.0～1.0）
    float get_left_trigger() const;
    float get_right_trigger() const;
    // 振動を設定（0.0～1.0）
    void set_vibration(float left_motor, float right_motor);

private:
    // スティックのデッドゾーンを適用する
    static float apply_deadzone(SHORT value, SHORT deadzone);

    // プレイヤーインデックス
    DWORD player_index_{};
    // パッドが接続されているか
    bool connected_{};
    // ボタン状態のダブルバッファ
    WORD current_buttons_{};
    WORD previous_buttons_{};
    // スティック値
    float left_stick_x_{};
    float left_stick_y_{};
    float right_stick_x_{};
    float right_stick_y_{};
    // トリガー値
    float left_trigger_{};
    float right_trigger_{};
};

#endif