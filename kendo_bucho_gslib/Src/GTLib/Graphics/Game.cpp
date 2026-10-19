#include "Game.h"
#include "GTLib/Graphics/Graphics.h"
#include "GTLib/Input/Input.h"
#include "GTLib/Sound/Sound.h"
#include "GTLib/GTLibTextrueID.h"

// DirectXに関係するライブラリの指定
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

// ウィンドウプロシージャ(ウインドウに対するイベント処理に対応する関数）
static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    // ゲームクラスのインスタンスを取得する
    Game* game = reinterpret_cast<Game*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    switch (uMsg) {
    case WM_SIZE:
        if (game) {
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            // 最小化された時は width/height が 0 になるため除外
            if (width > 0 && height > 0) {
                game->resize(static_cast<int>(width), static_cast<int>(height));
            }
        }
        return 0;
    case WM_DESTROY:         // ウィンドウが破壊された
        PostQuitMessage(0);  // 終了メッセージ(WM_QUIT)を送る
        return 0;
    }
    // その他メッセージに対するデフォルト処理を行う
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
// コンストラクタ
Game::Game(int window_width, int window_height, const LPCSTR window_name, bool fullscreen) :
    window_width_{ window_width }, window_height_{ window_height } {
    // ウィンドウを作成する
    // ウィンドウクラスを登録する
    wnd_class_.lpfnWndProc = WndProc;
    wnd_class_.hInstance = GetModuleHandle(0);
    wnd_class_.lpszClassName = window_name;
    wnd_class_.hCursor = LoadCursor(NULL, IDC_ARROW);
    wnd_class_.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    RegisterClass(&wnd_class_);
    // ウィンドウサイズを計算
    RECT rect{ 0, 0, window_width, window_height };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
    // ウィンドウの作成
    hwnd_ = CreateWindow(window_name, window_name,
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, wnd_class_.hInstance, NULL);
    // Gameインスタンスのポインタをウィンドウに関連付ける
    SetWindowLongPtr(hwnd_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    // グラフィックスデバイスの作成
    GraphicsDevice::Desc desc{};
    desc.window_width = window_width;
    desc.window_height = window_height;
    graphics_device_ = new GraphicsDevice{ hwnd_, desc };
    // フルスクリーンの設定 (非表示の状態でボーダレスフルスクリーン化を行います)
    graphics_device_->set_borderless_fullscreen(fullscreen);
    // 設定完了後にウィンドウを表示する
    if (fullscreen) {
        ShowWindow(hwnd_, SW_SHOWNORMAL);// フルスクリーンモード
    }
    else {
        ShowWindow(hwnd_, SW_SHOWDEFAULT);// ウィンドウモード
    }
    UpdateWindow(hwnd_);
    //テクスチャIDを初期化
    GTLibTextureID::Instance()->initialize();
}

// デストラクタ
Game::~Game() {
    // グラフィックスデバイスの削除
    delete graphics_device_;
    // ウィンドウの登録解除
    UnregisterClass(wnd_class_.lpszClassName, wnd_class_.hInstance);
}

// 実行
int Game::run() {
    // 各システムの初期化
    Graphics::initialize(graphics_device_);
    Input::initialize(hwnd_);
    Sound::initialize();
    // 開始
    start();
    // 最初のフレームでデルタタイムが異常にならないよう、現在時刻で初期化
    last_update_time_ = std::chrono::steady_clock::now();
    MSG msg{};
    bool quit_message = false;
    while (!quit_message && is_running()) {
        // ウィンドウメッセージの取得（メッセージがあるかぎり優先的に取得）
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                quit_message = true;
            }
            TranslateMessage(&msg);     // 仮想キーコードを文字コードに変換
            DispatchMessage(&msg);      // ウィンドウプロシージャにメッセージを送信
        }
        // デルタタイムを計算（秒単位）
        auto now = std::chrono::steady_clock::now();    // 現在時刻の取得
        float delta_time = std::chrono::duration<float>(now - last_update_time_).count();
        last_update_time_ = now;
        // デルタタイムの最大値の調整
        delta_time = (delta_time < max_delta_time_) ? delta_time : max_delta_time_;
        // 入力の更新
        Input::update();
        // 更新
        update(delta_time);
        // 描画
        draw();
        // バッファをスワップする
        graphics_device_->swap_buffers();
    }
    // GPUの終了を待つ
    Graphics::wait_for_gpu();
    // 終了
    end();
    // 各システムの終了
    Sound::finalize();
    Input::finalize();
    Graphics::finalize();
    return static_cast<int>(msg.wParam);
}
// 実行中か？
bool Game::is_running() const {
    // エスケープキーが押されていない？（押されたら終了）
    return GetAsyncKeyState(VK_ESCAPE) == 0;
}
// ウィンドウサイズ変更時の処理
void Game::resize(int width, int height) {
    window_width_ = width;
    window_height_ = height;
    if (graphics_device_) {
        graphics_device_->resize_back_buffer((UINT)width, (UINT)height);
    }
}