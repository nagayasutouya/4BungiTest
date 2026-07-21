#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <d3d12.h>
#include <string>
#include <vector>

class DescriptorPool;
class DescriptorHandle;

// テクスチャ
class Texture {
public:
    // コンストラクタ
    Texture(ID3D12Device* device, ID3D12CommandQueue* queue, DescriptorPool* pool, const std::string& file_name);
    // デストラクタ
    ~Texture();
    // 幅を取得
    UINT64 width() const;
    // 高さを取得
    UINT64 height() const;
    // テクスチャ用のリソースを取得
    ID3D12Resource* resource() const;
    // シェーダーリソースビューの取得    
    D3D12_GPU_DESCRIPTOR_HANDLE shader_resource_view() const;

    // コピー禁止
    Texture(const Texture&) = delete;
    Texture& operator = (const Texture&) = delete;
private:
    // リソースの更新
    void update_subresources(ID3D12Device* device, ID3D12CommandQueue* queue, const std::vector<D3D12_SUBRESOURCE_DATA>& subresource_data);
private:
    // リソースディスクプリタ
    D3D12_RESOURCE_DESC resource_desc_{};
    // テクスチャ用のリソース
    ID3D12Resource* resource_{};
    // シェーダーリソースビュー
    DescriptorHandle* shader_resource_view_{};
};

#endif