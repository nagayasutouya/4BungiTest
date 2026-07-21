#include "RootSignature.h"
#include <vector>
#include "d3dx12.h"

// コンストラクタ
RootSignature::RootSignature(ID3D12Device* device, UINT num_cbv, UINT num_srv, UINT num_sampler, const D3D12_STATIC_SAMPLER_DESC sampler[]) {
    // シェーダーリソースビューのディスクリプタレンジの作成
    std::vector<CD3DX12_DESCRIPTOR_RANGE> descriptor_ranges(num_srv);
    for (UINT i = 0; i < num_srv; ++i) {
        descriptor_ranges[i].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, i);
    }
    // ルートパラメータ
    std::vector<CD3DX12_ROOT_PARAMETER> root_params(num_cbv + num_srv);
    // ルートパラメータ数
    UINT num_root_params{ 0 };
    // ルートコンスタントバッファビューの設定
    cbv_root_index_ = num_root_params;
    for (UINT i = 0; i < num_cbv; ++i) {
        root_params[num_root_params++].InitAsConstantBufferView(i);
    }
    // シェーダーリソースビューの設定
    srv_root_index_ = num_root_params;
    for (UINT i = 0; i < num_srv; ++i) {
        root_params[num_root_params++].InitAsDescriptorTable(1, &descriptor_ranges[i]);
    }
    // ルートシグネチャの作成
    CD3DX12_ROOT_SIGNATURE_DESC desc{
        num_root_params, root_params.data(),
        num_sampler, sampler,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
    };
    // ルートシグネチャの作成
    ID3DBlob* blob{};
    D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1_0, &blob, nullptr);
    device->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&root_signature_));
    blob->Release();
}
// デストラクタ
RootSignature::~RootSignature() {
    root_signature_->Release();
}
// ルートシグネチャの取得
ID3D12RootSignature* RootSignature::root_signature() const {
    return root_signature_;
}
// コンスタントバッファビューの先頭のインデックスを返す
UINT RootSignature::cbv_root_index() const {
    return cbv_root_index_;
}

// シェーダーリソースビューの先頭のインデックスを返す
UINT RootSignature::srv_root_index() const {
    return srv_root_index_;
}