#include "FrameBuffer.h"
#include "GTLib/Graphics/GraphicsParts/d3dx12.h"

// コンストラクタ
FrameBuffer::FrameBuffer(ID3D12Device* device, HWND hwnd, UINT width, UINT height, DXGI_FORMAT format, ID3D12CommandQueue* command_queue) {
    device_ = device;
    hwnd_ = hwnd;
    // スワップチェーンの作成
    IDXGIFactory6* dxgi_factory{};
    HRESULT hr = CreateDXGIFactory2(0, IID_PPV_ARGS(&dxgi_factory));
    if (FAILED(hr)) {
        OutputDebugStringA("DXGIFactory Creation Failed!\n");
    }
    desc_.Width = width;
    desc_.Height = height;
    desc_.Format = format;
    desc_.Stereo = false;
    desc_.SampleDesc.Count = 1;
    desc_.SampleDesc.Quality = 0;
    desc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc_.BufferCount = FrameBufferCount;
    desc_.Scaling = DXGI_SCALING_STRETCH;
    desc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc_.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
    desc_.Flags = 0;
    dxgi_factory->CreateSwapChainForHwnd(
        command_queue,
        hwnd,
        &desc_,
        nullptr,
        nullptr,
        (IDXGISwapChain1**)&swap_chain_);
    // Alt+Enter 処理を無視させる
    dxgi_factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);
    dxgi_factory->Release();

    // バックバッファ用のレンダーターゲットビューのディスクプリタヒープを作成
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    heapDesc.NumDescriptors = FrameBufferCount;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    heapDesc.NodeMask = 0;
    device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&render_target_heaps_));
    // デプスステンシルビューの作成
    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    dsvHeapDesc.NodeMask = 0;
    device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&depth_stencil_heap_));
    // バックバッファのレンダーターゲットビューを作成
    create_render_target_views();
    // デプス・ステンシルバッファの作成
    create_depth_stencil_buffer();
}
// デストラクタ
FrameBuffer::~FrameBuffer() {
    depth_stencil_heap_->Release();
    depth_stencil_buffer_->Release();
    render_target_heaps_->Release();
    back_buffer_[0]->Release();
    back_buffer_[1]->Release();
    swap_chain_->Release();
}
// バッファのスワップ
void FrameBuffer::swap_buffers(UINT sync_interval, UINT flags) {
    // バックバッファのスワップ
    swap_chain_->Present(sync_interval, flags);
}
// 現在のバックバッファのインデックス
UINT FrameBuffer::back_buffer_index() const {
    return swap_chain_->GetCurrentBackBufferIndex();
}
// バックバッファの取得
ID3D12Resource* FrameBuffer::back_buffer() const {
    return back_buffer_[back_buffer_index()];
}
// デプスステンシルバッファの取得
ID3D12Resource* FrameBuffer::depth_stencil_buffer() const {
    return depth_stencil_buffer_;
}
// バックバッファのビューを取得
D3D12_CPU_DESCRIPTOR_HANDLE FrameBuffer::render_target_view() const {
    return render_target_views_[back_buffer_index()];
}
// デプスバッファのビューを取得
D3D12_CPU_DESCRIPTOR_HANDLE FrameBuffer::depth_stencil_view() const {
    return depth_stencil_view_;
}
// フレームバッファの幅を取得
UINT FrameBuffer::width() const {
    return desc_.Width;
}
// フレームバッファの高さを取得
UINT FrameBuffer::height() const {
    return desc_.Height;
}
// バックバッファのリサイズ
void FrameBuffer::resize(UINT width, UINT height) {
    // スワップチェーンをリサイズする前に、すべてのバックバッファリソースを解放する
    back_buffer_[0]->Release();
    back_buffer_[1]->Release();
    depth_stencil_buffer_->Release();
    // スワップチェーンのリサイズ
    swap_chain_->ResizeBuffers(FrameBufferCount, width, height, desc_.Format, desc_.Flags);
    desc_.Width = width;
    desc_.Height = height;
    // バックバッファのレンダーターゲットビューの作成
    create_render_target_views();
    // デプス・ステンシルバッファの作成
    create_depth_stencil_buffer();
}
// ボーダレスウィンドウフルスクリーン切替
void FrameBuffer::set_borderless_fullscreen(bool fullscreen) {
    // 既にフルスクリーンなら何もしない
    if (fullscreen == is_fullscreen_) return;

    if (fullscreen) {
        // 現在のウィンドウスタイルと位置を保存
        window_style_ = GetWindowLong(hwnd_, GWL_STYLE);
        GetWindowRect(hwnd_, &window_rect_);

        // フルスクリーン対象モニターの解像度を取得
        HMONITOR monitor = MonitorFromWindow(hwnd_, MONITOR_DEFAULTTONEAREST);
        MONITORINFO monitor_info{ sizeof(MONITORINFO) };
        GetMonitorInfo(monitor, &monitor_info);
        const RECT& mr = monitor_info.rcMonitor;

        // 現在のウィンドウスタイルを取得し、可視状態（WS_VISIBLE）を維持する
        LONG current_style = GetWindowLong(hwnd_, GWL_STYLE);
        LONG new_style = WS_POPUP;
        if (current_style & WS_VISIBLE) {
            new_style |= WS_VISIBLE;
        }

        // ウィンドウスタイルをボーダレスに変更
        SetWindowLong(hwnd_, GWL_STYLE, new_style);
        SetWindowPos(hwnd_, HWND_TOP,
            mr.left, mr.top,
            mr.right - mr.left, mr.bottom - mr.top,
            SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

        // バックバッファをモニター解像度に合わせてリサイズ
        resize(static_cast<UINT>(mr.right - mr.left), static_cast<UINT>(mr.bottom - mr.top));
    }
    else {
        // ウィンドウスタイルと位置を復元
        SetWindowLong(hwnd_, GWL_STYLE, window_style_);
        SetWindowPos(hwnd_, HWND_NOTOPMOST,
            window_rect_.left,
            window_rect_.top,
            window_rect_.right - window_rect_.left,
            window_rect_.bottom - window_rect_.top,
            SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        // クライアント領域を取得
        RECT client_rect;
        GetClientRect(hwnd_, &client_rect);
        // バックバッファをクライアント領域のサイズに合わせてリサイズ
        resize(static_cast<UINT>(client_rect.right - client_rect.left),
            static_cast<UINT>(client_rect.bottom - client_rect.top));
    }
    is_fullscreen_ = fullscreen;
}
// バックバッファのレンダーターゲットビューの作成
void FrameBuffer::create_render_target_views() {
    D3D12_CPU_DESCRIPTOR_HANDLE handle{
        render_target_heaps_->GetCPUDescriptorHandleForHeapStart() };
    const UINT rtv_size = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    for (UINT i = 0; i < FrameBufferCount; ++i) {
        swap_chain_->GetBuffer(i, IID_PPV_ARGS(&back_buffer_[i]));
        device_->CreateRenderTargetView(back_buffer_[i], nullptr, handle);
        render_target_views_[i] = handle;
        handle.ptr += rtv_size;
    }
}
// デプス・ステンシルバッファの作成
void FrameBuffer::create_depth_stencil_buffer() {
    // ヒーププロパティの設定
    CD3DX12_HEAP_PROPERTIES heap_prop{ D3D12_HEAP_TYPE_DEFAULT };
    // テクスチャの初期化
    D3D12_RESOURCE_DESC res_desc = CD3DX12_RESOURCE_DESC::Tex2D(
        DXGI_FORMAT_D32_FLOAT, desc_.Width, desc_.Height, 1, 1);
    res_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    // クリア値の設定
    CD3DX12_CLEAR_VALUE clear_value{ res_desc.Format, 1.0f, 0 };
    // リソースの作成
    device_->CreateCommittedResource(
        &heap_prop,
        D3D12_HEAP_FLAG_NONE,
        &res_desc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &clear_value,
        IID_PPV_ARGS(&depth_stencil_buffer_));
    // デプス・ステンシルビューの作成
    depth_stencil_view_ = depth_stencil_heap_->GetCPUDescriptorHandleForHeapStart();
    device_->CreateDepthStencilView(depth_stencil_buffer_, nullptr, depth_stencil_view_);
}