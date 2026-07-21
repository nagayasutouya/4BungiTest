#ifndef GAME_H_
#define GAME_H_

#include "GTLib/Graphics/GraphicsParts/GraphicsDevice.h"
#include <chrono>

// main関数をエントリポイントに指定する
#pragma comment(linker,"/ENTRY:mainCRTStartup") 

// ゲームクラス
class Game {
public:
    // コンストラクタ
    Game(int window_width = 640, int window_height = 480, bool fullscreen = false);
    // デストラクタ
    virtual ~Game();
    // 実行
    int run();

protected:
    // 開始
    virtual void start() {}
    // 更新
    virtual void update(float) {}
    // 描画
    virtual void draw() {}
    // 終了
    virtual void end() {}
    // 実行中か？
    virtual bool is_running() const;

protected:
    // ウィンドウの幅
    int window_width_;
    // ウィンドウの高さ
    int window_height_;
    // ウィンドウハンドラ
    HWND hwnd_{};
    // ウィンドウクラス
    WNDCLASS wnd_class_{};
    // グラフィックスデバイス
    GraphicsDevice* graphics_device_{};
    // 前フレームの時刻（デルタタイム計算用）
    std::chrono::steady_clock::time_point last_update_time_;
    // デルタタイムの最大値
    float max_delta_time_{ 1.0f / 30.f };
};

#endif

