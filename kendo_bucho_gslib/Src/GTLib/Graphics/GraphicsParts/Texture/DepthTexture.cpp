#include "DepthTexture.h"
#include "GTLib/Graphics/GraphicsParts/BufferAndPool/DescriptorPool.h"
#include "GTLib/Graphics/GraphicsParts/d3dx12.h"

// コンストラクタ
DepthTexture::DepthTexture(
    ID3D12Device* device,
    DescriptorPool* pool,
    UINT width,
    UINT height,
    DXGI_FORMAT format
) : Texture() {
    // DSV フォーマットと SRV フォーマットの決定
    // リソースは Typeless で作成し、DSV/SRV でそれぞれの型付きフォーマットを使う
    DXGI_FORMAT resource_format{};
    DXGI_FORMAT dsv_format{};
    DXGI_FORMAT srv_format{};
    switch (format) {
    case DXGI_FORMAT_D32_FLOAT:
        resource_format = DXGI_FORMAT_R32_TYPELESS;
        dsv_format = DXGI_FORMAT_D32_FLOAT;
        srv_format = DXGI_FORMAT_R32_FLOAT;
        break;
    case DXGI_FORMAT_D24_UNORM_S8_UINT:
        resource_format = DXGI_FORMAT_R24G8_TYPELESS;
        dsv_format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        srv_format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
        break;
    case DXGI_FORMAT_D16_UNORM:
        resource_format = DXGI_FORMAT_R16_TYPELESS;
        dsv_format = DXGI_FORMAT_D16_UNORM;
        srv_format = DXGI_FORMAT_R16_UNORM;
        break;
    }
    // クリア値の設定（深度: 1.0f、ステンシル: 0）
    D3D12_CLEAR_VALUE clear_value{};
    clear_value.Format = dsv_format;
    clear_value.DepthStencil.Depth = 1.0f;
    clear_value.DepthStencil.Stencil = 0;

    // ヒーププロパティの設定
    CD3DX12_HEAP_PROPERTIES heap_prop(D3D12_HEAP_TYPE_DEFAULT);
    // リソースの設定（Typelessフォーマット + ALLOW_DEPTH_STENCIL）
    resource_desc_ = CD3DX12_RESOURCE_DESC::Tex2D(
        resource_format, width, height, 1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
    // 現在のテクスチャの状態をレンダーターゲットに設定
    current_state_ = D3D12_RESOURCE_STATE_DEPTH_WRITE;
    // リソースの作成
    device->CreateCommittedResource(
        &heap_prop,
        D3D12_HEAP_FLAG_NONE,
        &resource_desc_,
        current_state_,
        &clear_value,
        IID_PPV_ARGS(&resource_)
    );

    // シェーダーリソースビュー(SRV)の作成
    // 深度値をRチャンネルとして読み取れるようにSRVフォーマットを指定する
    shader_resource_view_ = pool->allocate();
    D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};
    srv_desc.Format = srv_format;
    srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srv_desc.Texture2D.MipLevels = 1;
    srv_desc.Texture2D.MostDetailedMip = 0;
    srv_desc.Texture2D.PlaneSlice = 0;
    srv_desc.Texture2D.ResourceMinLODClamp = 0.0f;
    device->CreateShaderResourceView(resource_, &srv_desc,
        shader_resource_view_->cpu_handle());

    // デプスステンシルビュー(DSV)用ディスクリプタヒープをクラス内で作成
    D3D12_DESCRIPTOR_HEAP_DESC dsv_heap_desc{};
    dsv_heap_desc.NumDescriptors = 1;
    dsv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsv_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // DSVはシェーダー不可視でOK
    dsv_heap_desc.NodeMask = 0;
    device->CreateDescriptorHeap(&dsv_heap_desc, IID_PPV_ARGS(&dsv_heap_));

    // デプスステンシルビュー(DSV)の作成
    depth_stencil_view_ = dsv_heap_->GetCPUDescriptorHandleForHeapStart();
    D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc{};
    dsv_desc.Format = dsv_format;
    dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsv_desc.Flags = D3D12_DSV_FLAG_NONE;
    dsv_desc.Texture2D.MipSlice = 0;
    device->CreateDepthStencilView(resource_, &dsv_desc, depth_stencil_view_);
}
// デストラクタ
DepthTexture::~DepthTexture() {
    dsv_heap_->Release();
}
