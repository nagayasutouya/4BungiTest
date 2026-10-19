#ifndef RENDER_TEXTURE_H_
#define RENDER_TEXTURE_H_

#include "Texture.h"

// レンダーテクスチャクラス
class RenderTexture : public Texture {
public:
    // コンストラクタ
    RenderTexture(ID3D12Device* device, DescriptorPool* srv_pool,
        UINT width, UINT height, DXGI_FORMAT format,
        const float* clear_color = nullptr);
    // デストラクタ
    ~RenderTexture() override;
private:
    ID3D12DescriptorHeap* rtv_heap_{};// RTV用ディスクリプタヒープ
};

#endif