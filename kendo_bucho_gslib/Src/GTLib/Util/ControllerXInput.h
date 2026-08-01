#ifndef CONTROLLER_XINPUT_H_
#define CONTROLLER_XINPUT_H_

#include <Windows.h>
#include <Xinput.h>

#pragma comment(lib, "Xinput.lib")
static const float stic_dec_{ 32767.0f };

class ControllerXInput{
public:
    ControllerXInput(int id = 0);
    bool update();// 入力の更新
    bool is_connected() const;//コントローラー繋がっているか
    // ボタン状態
    bool is_pressed(WORD button) const;// 押されている間
    bool is_trigger_down(WORD button) const;// 押された瞬間
    bool is_trigger_up(WORD button) const;// 離した瞬間
    WORD current_bottuns_state()const {
        WORD result = state.Gamepad.wButtons;
        return result;
    };
    // スティック
    float get_left_stick_x(float dead_zone = 0.1f)const;
    float get_left_stick_y(float dead_zone = 0.1f)const;
    float get_right_stick_x(float dead_zone = 0.1f)const;
    float get_right_stick_y(float dead_zone = 0.1f)const;
    float get_left_stick_x_down(float dead_zone = 0.1f)const;
    float get_left_stick_y_down(float dead_zone = 0.1f)const;
    float get_right_stick_x_down(float dead_zone = 0.1f)const;
    float get_right_stick_y_down(float dead_zone = 0.1f)const;
    // トリガー
    BYTE get_left_trigger()  const { return state.Gamepad.bLeftTrigger; }
    BYTE get_right_trigger() const { return state.Gamepad.bRightTrigger; }
private:
    int controllerId;
    XINPUT_STATE state{};
    XINPUT_STATE prevState{};
    bool connected = false;
};

#endif