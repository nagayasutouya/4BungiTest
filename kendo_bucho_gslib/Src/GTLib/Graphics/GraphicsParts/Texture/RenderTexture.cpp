#include "RenderTexture.h"
#include "GTLib/Graphics/GraphicsParts/BufferAndPool/DescriptorPool.h"
#include "GTLib/Graphics/GraphicsParts/d3dx12.h"

// コンストラクタ
RenderTexture::RenderTexture(
    ID3D12Device* device, DescriptorPool* pool,
    UINT width, UINT height, DXGI_FORMAT format,
    const float* clear_color
) : Texture() {
    // クリアカラーの設定
    D3D12_CLEAR_VALUE clear_value{};
    clear_value.Format = format;
    if (clear_color) {
        clear_value.Color[0] = clear_color[0];
        clear_value.Color[1] = clear_color[1];
        clear_value.Color[2] = clear_color[2];
        clear_value.Color[3] = clear_color[3];
    }
    // ヒーププロパティの設定
    CD3DX12_HEAP_PROPERTIES heap_prop(D3D12_HEAP_TYPE_DEFAULT);
    // リソースの作成
    resource_desc_ = CD3DX12_RESOURCE_DESC::Tex2D(
        format, width, height, 1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
    );
    // 現在のテクスチャの状態をレンダーターゲットに設定
    current_state_ = D3D12_RESOURCE_STATE_RENDER_TARGET;

    device->CreateCommittedResource(
        &heap_prop,
        D3D12_HEAP_FLAG_NONE,
        &resource_desc_,
        current_state_,
        &clear_value,
        IID_PPV_ARGS(&resource_)
    );

    // シェーダーリソースビュー(SRV)の作成
    shader_resource_view_ = pool->allocate();
    device->CreateShaderResourceView(resource_, nullptr, shader_resource_view_->cpu_handle());

    // レンダーターゲットビュー(RTV)用ディスクリプタヒープを作成
    D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc{};
    rtv_heap_desc.NumDescriptors = 1;
    rtv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtv_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    rtv_heap_desc.NodeMask = 0;
    device->CreateDescriptorHeap(&rtv_heap_desc, IID_PPV_ARGS(&rtv_heap_));

    // レンダリングターゲットビュー(RTV)の作成
    render_target_view_ = rtv_heap_->GetCPUDescriptorHandleForHeapStart();
    device->CreateRenderTargetView(resource_, nullptr, render_target_view_);
}
// デストラクタ
RenderTexture::~RenderTexture() {
    rtv_heap_->Release();
}