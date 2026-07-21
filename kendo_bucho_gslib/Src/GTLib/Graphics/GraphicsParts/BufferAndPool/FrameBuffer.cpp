#include "FrameBuffer.h"
#include "GTLib/Graphics/GraphicsParts/d3dx12.h"

// コンストラクタ
FrameBuffer::FrameBuffer(ID3D12Device* device, HWND hwnd, UINT width, UINT height, DXGI_FORMAT format, ID3D12CommandQueue* command_queue) {
    // スワップチェーンの作成
    IDXGIFactory6* dxgi_factory{};
    CreateDXGIFactory2(0, IID_PPV_ARGS(&dxgi_factory));
    desc_.Width = width;
    desc_.Height = height;
    desc_.Format = format;
    desc_.Stereo = false;
    desc_.SampleDesc.Count = 1;
    desc_.SampleDesc.Quality = 0;
    desc_.BufferUsage = DXGI_USAGE_BACK_BUFFER;
    desc_.BufferCount = FrameBufferCount;
    desc_.Scaling = DXGI_SCALING_STRETCH;
    desc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc_.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    desc_.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    dxgi_factory->CreateSwapChainForHwnd(
        command_queue,
        hwnd,
        &desc_,
        nullptr,
        nullptr,
        (IDXGISwapChain1**)&swap_chain_);
    dxgi_factory->Release();

    // バックバッファ用のレンダーターゲットビューのディスクプリタヒープを作成
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    heapDesc.NumDescriptors = FrameBufferCount;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    heapDesc.NodeMask = 0;
    device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&render_target_heaps_));
    // バックバッファ用のレンダーターゲットビューの作成
    D3D12_CPU_DESCRIPTOR_HANDLE handle{ render_target_heaps_->GetCPUDescriptorHandleForHeapStart() };
    for (UINT i = 0; i < FrameBufferCount; ++i) {
        swap_chain_->GetBuffer(i, IID_PPV_ARGS(&back_buffer_[i]));
        device->CreateRenderTargetView(back_buffer_[i], nullptr, handle);
        render_target_views_[i] = handle;
        handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }
    // デプス・ステンシルバッファの作成
    CD3DX12_HEAP_PROPERTIES heap_prop{ D3D12_HEAP_TYPE_DEFAULT };
    D3D12_RESOURCE_DESC res_desc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, width, height, 1, 1);
    res_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    CD3DX12_CLEAR_VALUE clear_value{ res_desc.Format, 1.0f, 0 };
    device->CreateCommittedResource(
        &heap_prop,
        D3D12_HEAP_FLAG_NONE,
        &res_desc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &clear_value,
        IID_PPV_ARGS(&depth_stencil_buffer_)
    );
    // デプス・ステンシルビューの作成
    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    dsvHeapDesc.NodeMask = 0;
    device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&depth_stencil_heap_));
    depth_stencil_view_ = depth_stencil_heap_->GetCPUDescriptorHandleForHeapStart();
    device->CreateDepthStencilView(depth_stencil_buffer_, nullptr, depth_stencil_view_);
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

// デプス・ステンシルバッファの取得
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