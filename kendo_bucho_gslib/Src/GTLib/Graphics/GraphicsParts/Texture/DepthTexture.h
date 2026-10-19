#ifndef DEPTH_TEXTURE_H_
#define DEPTH_TEXTURE_H_

#include "Texture.h"

// デプステクスチャクラス
class DepthTexture : public Texture {
public:
    // コンストラクタ
    DepthTexture(ID3D12Device* device, DescriptorPool* srv_pool,
        UINT width, UINT height, DXGI_FORMAT format = DXGI_FORMAT_D32_FLOAT);
    // デストラクタ
    ~DepthTexture() override;

private:
    ID3D12DescriptorHeap* dsv_heap_{};// DSV用ディスクリプタヒープ
};

#endif
