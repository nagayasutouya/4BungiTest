#ifndef ROOT_SIGNATURE_H_
#define ROOT_SIGNATURE_H_

#include <d3d12.h>

// ルートシグネチャクラス
class RootSignature {
public:
    // コンストラクタ
    RootSignature(ID3D12Device* device, UINT num_cbv, UINT num_srv, UINT num_inline_srv, UINT num_sampler, const D3D12_STATIC_SAMPLER_DESC sampler[]);
    // デストラクタ
    ~RootSignature();
    // ルートシグネチャの取得
    ID3D12RootSignature* root_signature() const;
    // コンスタントバッファビューの先頭のインデックスを返す
    UINT cbv_root_index() const;
    // シェーダーリソースビューの先頭のインデックスを返す
    UINT srv_root_index() const;
    // インラインシェーダーリソースビューの先頭のインデックスを返す
    UINT inline_srv_root_index() const;
    // コピー禁止
    RootSignature(const RootSignature&) = delete;
    RootSignature& operator = (const RootSignature&) = delete;

private:
    // ルートシグネチャ
    ID3D12RootSignature* root_signature_{};
    // コンスタントバッファビューの先頭のインデックス
    UINT cbv_root_index_;
    // シェーダーリソースビューの先頭のインデックス
    UINT srv_root_index_;
    // インラインシェーダーリソースビューの先頭のインデックス
    UINT inline_srv_root_index_;
};

#endif