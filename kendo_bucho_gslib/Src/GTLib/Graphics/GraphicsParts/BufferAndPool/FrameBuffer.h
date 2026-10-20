#ifndef FRAME_BUFFER_H_
#define FRAME_BUFFER_H_

#include <d3d12.h>
#include <dxgi1_6.h>

// フレームバッファクラス
class FrameBuffer {
public:
    // コンストラクタ
    FrameBuffer(ID3D12Device* device, HWND hwnd, UINT width, UINT height, DXGI_FORMAT format, ID3D12CommandQueue* command_queue);
    // デストラクタ
    ~FrameBuffer();
    // バッファのスワップ
    void swap_buffers(UINT sync_interval = 1, UINT flags = 0);
    // 現在のバックバッファのインデックス
    UINT back_buffer_index() const;
    // バックバッファの取得
    ID3D12Resource* back_buffer() const;
    // デプスステンシルバッファの取得
    ID3D12Resource* depth_stencil_buffer() const;
    // バックバッファのビューを取得
    D3D12_CPU_DESCRIPTOR_HANDLE render_target_view() const;
    // デプスステンシルバッファのビューを取得
    D3D12_CPU_DESCRIPTOR_HANDLE depth_stencil_view() const;
    // フレームバッファの幅を取得
    UINT width() const;
    // フレームバッファの高さを取得
    UINT height() const;
    // バックバッファのリサイズ
    void resize(UINT width, UINT height);
    // ボーダレスウィンドウフルスクリーン切替
    void set_borderless_fullscreen(bool fullscreen);

    // コピー禁止
    FrameBuffer(const FrameBuffer&) = delete;
    FrameBuffer& operator=(const FrameBuffer&) = delete;
private:
    // バックバッファのレンダーターゲットビューの作成
    void create_render_target_views();
    // デプス・ステンシルバッファの作成
    void create_depth_stencil_buffer();
private:
    // フレームバッファ数
    static const UINT FrameBufferCount{ 2 };

    // DirectX12デバイス
    ID3D12Device* device_{ nullptr };
    // ウィンドウハンドル
    HWND hwnd_{};
    // スワップチェーンディスクリプタ
    DXGI_SWAP_CHAIN_DESC1 desc_{};
    // スワップチェーン
    IDXGISwapChain4* swap_chain_{};
    // バックバッファ用のリソース
    ID3D12Resource* back_buffer_[FrameBufferCount]{};
    // バックバッファのレンダーターゲットビュー用のディスクプリタヒープ
    ID3D12DescriptorHeap* render_target_heaps_{};
    // バックバッファのレンダーターゲットビュー
    D3D12_CPU_DESCRIPTOR_HANDLE render_target_views_[FrameBufferCount]{};

    // デプスステンシルバッファ
    ID3D12Resource* depth_stencil_buffer_{};
    // デプスステンシルバッファ用のディスクプリタヒープ
    ID3D12DescriptorHeap* depth_stencil_heap_{};
    // デプスステンシルバッファのデプスステンシルビュー
    D3D12_CPU_DESCRIPTOR_HANDLE depth_stencil_view_{};

    // フルスクリーン前のウィンドウスタイル
    LONG window_style_{};
    // フルスクリーン前のウィンドウ位置・サイズ
    RECT window_rect_{};
    // フルスクリーン状態
    bool is_fullscreen_{ false };
};

#endif
