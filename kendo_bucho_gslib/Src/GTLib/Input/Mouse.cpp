#include "Mouse.h"
#include <commctrl.h>

#pragma comment(lib, "comctl32.lib")

// サブクラスID（KeyBoardと異なる値にする）
static constexpr UINT_PTR SUBCLASS_ID = 2;

// コンストラクタ
Mouse::Mouse(HWND hwnd) : hwnd_(hwnd) {
    // RawInputデバイス登録（マウス）
    RAWINPUTDEVICE rid{};
    rid.usUsagePage = 0x01;  // Generic Desktop Controls
    rid.usUsage = 0x02;      // Mouse
    rid.dwFlags = RIDEV_INPUTSINK;
    rid.hwndTarget = hwnd_;
    RegisterRawInputDevices(&rid, 1, sizeof(rid));
    // ウィンドウサブクラス化
    SetWindowSubclass(hwnd_, subclass_proc, SUBCLASS_ID, reinterpret_cast<DWORD_PTR>(this));
}

// デストラクタ
Mouse::~Mouse() {
    // カーソルが非表示の場合は表示に戻す
    if (!cursor_visible_) {
        show_cursor();
    }
    // サブクラス解除
    RemoveWindowSubclass(hwnd_, subclass_proc, SUBCLASS_ID);
}

// 毎フレーム呼び出してマウス状態を更新する
void Mouse::update() {
    // ボタン状態のダブルバッファ更新
    previous_buttons_ = current_buttons_;
    current_buttons_ = buttons_;
    // 移動量を反映してRawInputバッファリセット
    current_delta_x_ = delta_x_;
    current_delta_y_ = delta_y_;
    delta_x_ = 0;
    delta_y_ = 0;
    // ホイール回転量を反映してリセット
    current_wheel_ = wheel_;
    wheel_ = 0;
    // カーソル位置取得（ウィンドウ上の座標に変換）
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(hwnd_, &pt);
    position_x_ = pt.x;
    position_y_ = pt.y;
}

// ボタンが押されているとtrue返す
bool Mouse::get_button(UINT button) const {
    return current_buttons_[button];
}

// ボタンが押された瞬間のみtrue返す
bool Mouse::get_button_down(UINT button) const {
    return current_buttons_[button] && !previous_buttons_[button];
}

// ボタンが離された瞬間のみtrue返す
bool Mouse::get_button_up(UINT button) const {
    return !current_buttons_[button] && previous_buttons_[button];
}

// フレーム間のX移動量を取得
LONG Mouse::get_delta_x() const {
    return current_delta_x_;
}

// フレーム間のY移動量を取得
LONG Mouse::get_delta_y() const {
    return current_delta_y_;
}

// フレーム間のホイール回転量を取得
LONG Mouse::get_wheel() const {
    return current_wheel_;
}

// ウィンドウX座標取得
LONG Mouse::get_position_x() const {
    return position_x_;
}

// ウィンドウY座標取得
LONG Mouse::get_position_y() const {
    return position_y_;
}

// マウスカーソル位置の設定
void Mouse::set_position(LONG x, LONG y) {
    POINT pt = { x, y };
    ClientToScreen(hwnd_, &pt);
    SetCursorPos(pt.x, pt.y);
    position_x_ = x;
    position_y_ = y;
}

// マウスカーソルの表示
void Mouse::show_cursor() {
    if (!cursor_visible_) {
        cursor_visible_ = true;
        // ShowCursorは内部カウンタ方式なので、0以上になるまで呼び出す
        while (ShowCursor(TRUE) < 0) {}
    }
}

// マウスカーソルの非表示
void Mouse::hide_cursor() {
    if (cursor_visible_) {
        cursor_visible_ = false;
        // ShowCursorは内部カウンタ方式なので、0未満になるまで呼び出す
        while (ShowCursor(FALSE) >= 0) {}
    }
}

// マウスカーソルの表示状態を取得
bool Mouse::is_cursor_visible() const {
    return cursor_visible_;
}

// サブクラス用ウィンドウプロシージャ
LRESULT CALLBACK Mouse::subclass_proc(
    HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
    UINT_PTR id, DWORD_PTR ref_data
) {
    Mouse* self = reinterpret_cast<Mouse*>(ref_data);
    if (msg == WM_INPUT) {
        self->process_raw_input(reinterpret_cast<HRAWINPUT>(lp));
    }
    // 次のサブクラスまたは元のウィンドウプロシージャにメッセージ転送
    return DefSubclassProc(hwnd, msg, wp, lp);
}

// RawInputデータ処理
void Mouse::process_raw_input(HRAWINPUT raw_input) {
    UINT size = 0;
    // RawInputデータのサイズ取得
    GetRawInputData(
        raw_input, RID_INPUT,
        nullptr, &size, sizeof(RAWINPUTHEADER)
    );
    // RawInputデータのサイズチェック
    if (size == 0) return;

    // メンバ変数のバッファサイズを調整（必要なら拡張）
    if (raw_input_buffer_.size() < size) {
        raw_input_buffer_.resize(size);
    }

    // RawInputデータ取得
    if (GetRawInputData(
        raw_input, RID_INPUT,
        raw_input_buffer_.data(), &size, sizeof(RAWINPUTHEADER)) != size) {
        return;
    }
    // RawInputデータ処理
    RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(raw_input_buffer_.data());
    if (raw->header.dwType == RIM_TYPEMOUSE) {
        USHORT flags = raw->data.mouse.usButtonFlags;
        // ボタン状態の更新
        if (flags & RI_MOUSE_LEFT_BUTTON_DOWN)   buttons_[LEFT] = true;
        if (flags & RI_MOUSE_LEFT_BUTTON_UP)     buttons_[LEFT] = false;
        if (flags & RI_MOUSE_RIGHT_BUTTON_DOWN)  buttons_[RIGHT] = true;
        if (flags & RI_MOUSE_RIGHT_BUTTON_UP)    buttons_[RIGHT] = false;
        if (flags & RI_MOUSE_MIDDLE_BUTTON_DOWN) buttons_[MIDDLE] = true;
        if (flags & RI_MOUSE_MIDDLE_BUTTON_UP)   buttons_[MIDDLE] = false;
        // 移動量の累積
        delta_x_ += raw->data.mouse.lLastX;
        delta_y_ += raw->data.mouse.lLastY;
        // ホイール回転量の累積
        if (flags & RI_MOUSE_WHEEL) {
            wheel_ += static_cast<SHORT>(raw->data.mouse.usButtonData);
        }
    }
}

