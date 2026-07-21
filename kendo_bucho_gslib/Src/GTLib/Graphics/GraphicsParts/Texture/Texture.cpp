#include "Texture.h"
#include "GTLib/Graphics/GraphicsParts/BufferAndPool/DescriptorPool.h"
#include "GTLib/Graphics/GraphicsParts/d3dx12.h"
#include "lodepng.h"
#include "GTLib/Graphics/GraphicsParts/GraphicsCommand.h"

// コンストラクタ
Texture::Texture(ID3D12Device* device, ID3D12CommandQueue* queue, DescriptorPool* pool, const std::string& file_name) {
    // pngファイルの読み込み
    unsigned int width{ 0 };
    unsigned int height{ 0 };
    std::vector<unsigned char> image;
    lodepng::decode(image, width, height, file_name);

    // ヒーププロパティの設定
    CD3DX12_HEAP_PROPERTIES heap_prop{ D3D12_HEAP_TYPE_DEFAULT };
    // リソースの設定
    resource_desc_ = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, width, height, 1, 1);
    // テクスチャ用のリソースを作成
    device->CreateCommittedResource(
        &heap_prop,
        D3D12_HEAP_FLAG_NONE,
        &resource_desc_,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&resource_)
    );
    // 画像データのアップロード
    std::vector<D3D12_SUBRESOURCE_DATA> subresource_data(1);
    subresource_data[0].pData = image.data();
    subresource_data[0].RowPitch = width * 4;
    subresource_data[0].SlicePitch = width * height * 4;
    update_subresources(device, queue, subresource_data);
    // シェーダーリソースビュー用のディクリプタヒープを作成
    shader_resource_view_ = pool->allocate();
    // シェーダーリソースビューの作成
    device->CreateShaderResourceView(resource_, nullptr, shader_resource_view_->cpu_handle());
}

// リソースの更新
void Texture::update_subresources(ID3D12Device* device, ID3D12CommandQueue* queue, const std::vector<D3D12_SUBRESOURCE_DATA>& subresource_data) {
    // サブリソースのサイズ
    UINT subresource_size = static_cast<UINT>(subresource_data.size());
    // アップロード用のバッファのサイズを取得
    UINT64 upload_size = GetRequiredIntermediateSize(resource_, 0, subresource_size);
    // アップロード用のバッファを作成
    CD3DX12_HEAP_PROPERTIES upload_heap_prop{ D3D12_HEAP_TYPE_UPLOAD };
    CD3DX12_RESOURCE_DESC upload_resource_desc{ CD3DX12_RESOURCE_DESC::Buffer(upload_size) };
    ID3D12Resource* upload_heap{};
    device->CreateCommittedResource(
        &upload_heap_prop,
        D3D12_HEAP_FLAG_NONE,
        &upload_resource_desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&upload_heap)
    );
    // コマンドリストの作成
    GraphicsCommand command{ device, queue };
    // コマンドリストの取得
    ID3D12GraphicsCommandList* command_list = command.reset().list();
    // イメージデータを転送
    UpdateSubresources(
        command_list,
        resource_,
        upload_heap,
        0,
        0,
        subresource_size,
        subresource_data.data()
    );
    // リソースバリアの設定
    CD3DX12_RESOURCE_BARRIER resource_barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        resource_, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
    );
    command_list->ResourceBarrier(1, &resource_barrier);
    // コマンドリストの実行を待つ
    command.execute().wait();
    // アップロード用のリソースを削除
    upload_heap->Release();
}
// デストラクタ
Texture::~Texture() {
    shader_resource_view_->release();
    resource_->Release();
}
// 幅を取得
UINT64 Texture::width() const {
    return resource_desc_.Width;
}

// 高さを取得
UINT64 Texture::height() const {
    return resource_desc_.Height;
}
// テクスチャ用のリソースを取得
ID3D12Resource* Texture::resource() const {
    return resource_;
}

// シェーダーリソースビューの取得	
D3D12_GPU_DESCRIPTOR_HANDLE Texture::shader_resource_view() const {
    return shader_resource_view_->gpu_handle();
}