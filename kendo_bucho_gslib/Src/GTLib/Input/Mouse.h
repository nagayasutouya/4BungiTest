#ifndef MOUSE_H_
#define MOUSE_H_

#include <windows.h>
#include <array>
#include <vector>

// マウスクラス
class Mouse {
public:
    // ボタン定数
    static constexpr UINT LEFT = 0;
    static constexpr UINT RIGHT = 1;
    static constexpr UINT MIDDLE = 2;
    static constexpr UINT NUM_BUTTONS = 3;

    // コンストラクタ
    Mouse(HWND hwnd);
    // デストラクタ
    ~Mouse();

    // 毎フレーム呼び出してマウス状態を更新する
    void update();
    // ボタンが押されているとtrue返す
    bool get_button(UINT button) const;
    // ボタンが押された瞬間のみtrue返す
    bool get_button_down(UINT button) const;
    // ボタンが離された瞬間のみtrue返す
    bool get_button_up(UINT button) const;
    // フレーム間のマウス移動量を取得
    LONG get_delta_x() const;
    LONG get_delta_y() const;
    // フレーム間のホイール回転量を取得
    LONG get_wheel() const;
    // ウィンドウのマウスカーソル位置取得
    LONG get_position_x() const;
    LONG get_position_y() const;

    // マウスカーソル位置の設定
    void set_position(LONG x, LONG y);

    // マウスカーソルの表示
    void show_cursor();
    // マウスカーソルの非表示
    void hide_cursor();
    // マウスカーソルの表示状態を取得
    bool is_cursor_visible() const;

    // コピー禁止
    Mouse(const Mouse&) = delete;
    Mouse& operator=(const Mouse&) = delete;
private:
    // サブクラス用ウィンドウプロシージャ
    static LRESULT CALLBACK subclass_proc(
        HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
        UINT_PTR id, DWORD_PTR ref_data);
    // RawInputデータ処理
    void process_raw_input(HRAWINPUT raw_input);

private:
    // ウィンドウハンドル
    HWND hwnd_;
    // ボタン状態の型
    using ButtonState = std::array<bool, NUM_BUTTONS>;
    // 現在のボタン状態（RawInputバッファ）
    ButtonState buttons_{};
    // ボタンデータのダブルバッファ
    ButtonState current_buttons_{};
    ButtonState previous_buttons_{};
    // マウス移動量（RawInputバッファ）
    LONG delta_x_{};
    LONG delta_y_{};
    // フレーム間の移動量
    LONG current_delta_x_{};
    LONG current_delta_y_{};
    // ホイール回転量（RawInputバッファ）
    LONG wheel_{};
    // フレーム間のホイール回転量
    LONG current_wheel_{};
    // カーソル位置（ウィンドウクライアント座標）
    LONG position_x_{};
    LONG position_y_{};
    // カーソル表示状態
    bool cursor_visible_{ true };

    // RawInputデータ用バッファ（再利用）
    std::vector<BYTE> raw_input_buffer_;
};

#endif

