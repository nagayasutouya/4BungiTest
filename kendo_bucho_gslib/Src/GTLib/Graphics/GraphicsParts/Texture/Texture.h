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
    virtual ~Texture();
    // 幅を取得
    UINT64 width() const;
    // 高さを取得
    UINT64 height() const;
    // テクスチャ用のリソースを取得
    ID3D12Resource* resource() const;
    // シェーダーリソースビューの取得    
    D3D12_GPU_DESCRIPTOR_HANDLE shader_resource_view() const;
    // 深度ステンシルビュー取得
    D3D12_CPU_DESCRIPTOR_HANDLE depth_stencil_view() const;
    // レンダーターゲットビュー取得
    D3D12_CPU_DESCRIPTOR_HANDLE render_target_view() const;

    // リソースバリア設定（現在の状態から更新）
    void barrier(ID3D12GraphicsCommandList* command_list, D3D12_RESOURCE_STATES state_after);

    // コピー禁止
    Texture(const Texture&) = delete;
    Texture& operator = (const Texture&) = delete;
protected:
    // デフォルトコンストラクタ（継承用）
    Texture() = default;
private:
    // リソースの更新
    void update_subresources(ID3D12Device* device, ID3D12CommandQueue* queue, const std::vector<D3D12_SUBRESOURCE_DATA>& subresource_data);
protected:
    // リソースディスクプリタ
    D3D12_RESOURCE_DESC resource_desc_{};
    // テクスチャ用のリソース
    ID3D12Resource* resource_{};
    // シェーダーリソースビュー
    DescriptorHandle* shader_resource_view_{};
    // レンダーターゲットビュー
    D3D12_CPU_DESCRIPTOR_HANDLE render_target_view_{};
    // デプスステンシルビュー
    D3D12_CPU_DESCRIPTOR_HANDLE depth_stencil_view_{};
    // 現在のリソース状態
    D3D12_RESOURCE_STATES current_state_{};
};

#endif