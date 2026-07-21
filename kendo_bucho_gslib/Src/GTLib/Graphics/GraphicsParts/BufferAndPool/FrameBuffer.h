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
    // デプス・ステンシルバッファの取得
    ID3D12Resource* depth_stencil_buffer() const;
    // バックバッファのビューを取得
    D3D12_CPU_DESCRIPTOR_HANDLE render_target_view() const;
    // デプス・ステンシルバッファのビューを取得
    D3D12_CPU_DESCRIPTOR_HANDLE depth_stencil_view() const;
    // フレームバッファの幅を取得
    UINT width() const;
    // フレームバッファの高さを取得
    UINT height() const;

    // コピー禁止
    FrameBuffer(const FrameBuffer&) = delete;
    FrameBuffer& operator=(const FrameBuffer&) = delete;
private:
    // フレームバッファ数
    static const UINT FrameBufferCount{ 2 };
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
    // デプスステンシルバッファのデプス・ステンシルビュー
    D3D12_CPU_DESCRIPTOR_HANDLE depth_stencil_view_{};
};

#endif

