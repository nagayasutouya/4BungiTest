#include "Texture.h"
#include "GTLib/Graphics/GraphicsParts/d3dx12.h"
#include "lodepng.h"
#include "stb_image.h"
#include "DDSTextureLoader12.h"
#include <filesystem>
#include "GTLib/Graphics/GraphicsParts/BufferAndPool/DescriptorPool.h"
#include "GTLib/Graphics/GraphicsParts/GraphicsCommand.h"

// コンストラクタ
Texture::Texture(ID3D12Device* device, ID3D12CommandQueue* queue, DescriptorPool* pool, const std::string& file_name) {
    // 画像ファイルの読み込み
    load_image(device, queue, pool, file_name);
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
    barrier(command_list, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    // コマンドリストの実行を待つ
    command.execute().wait();
    // アップロード用のリソースを削除
    upload_heap->Release();
}
// デストラクタ
Texture::~Texture() {
    if (shader_resource_view_ != nullptr) {
        shader_resource_view_->release();
    }
    if (resource_ != nullptr) {
        resource_->Release();
    }
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
// 奥行きを取得
UINT Texture::depth() const {
    return resource_desc_.DepthOrArraySize;
}

// シェーダーリソースビューの取得	
D3D12_GPU_DESCRIPTOR_HANDLE Texture::shader_resource_view() const {
    return shader_resource_view_->gpu_handle();
}
// レンダーターゲットビューの取得
D3D12_CPU_DESCRIPTOR_HANDLE Texture::render_target_view() const {
    return render_target_view_;
}
// デプスステンシルビューの取得
D3D12_CPU_DESCRIPTOR_HANDLE Texture::depth_stencil_view() const {
    return depth_stencil_view_;
}

// リソースバリア設定（現在の状態から更新）
void Texture::barrier(ID3D12GraphicsCommandList* command_list,
    D3D12_RESOURCE_STATES after_state) {
    // 状態が同じ場合は何もしない
    if (current_state_ == after_state) return;
    // リソースバリアの作成
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        resource_, current_state_, after_state);
    // コマンドリストにリソースバリアを設定
    command_list->ResourceBarrier(1, &barrier);
    // 現在の状態を更新
    current_state_ = after_state;
}

// 画像ファイル読み込み
void Texture::load_image(ID3D12Device* device, ID3D12CommandQueue* queue, DescriptorPool* pool, const std::string& file_name) {
    // 拡張子を取得
    std::filesystem::path ext = std::filesystem::path{ file_name }.extension();
    if (ext == ".dds" || ext == ".DDS") {
        // DDSファイルの読み込み
        load_dds(device, queue, pool, file_name);
    }
    else {
        // 画像ファイルの読み込み
        load_standard_image(device, queue, pool, file_name);
    }
}
// 標準画像読み込み（PNG・JPEG・BMP・TGA など stb_image 対応フォーマット）
void Texture::load_standard_image(ID3D12Device* device, ID3D12CommandQueue* queue, DescriptorPool* pool, const std::string& file_name) {
    // 画像ファイルの読み込み
    int width{};
    int height{};
    int bpp{};
    unsigned char* pixels = stbi_load(file_name.c_str(), &width, &height, &bpp, STBI_rgb_alpha);
    if (!pixels) {
        // ファイルの読み込みに失敗
        throw std::runtime_error("Failed to load: " + file_name);
    }
    // ヒーププロパティの設定
    CD3DX12_HEAP_PROPERTIES heap_prop{ D3D12_HEAP_TYPE_DEFAULT };
    // リソースの設定
    resource_desc_ = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, width, height, 1, 1);
    // 現在のテクスチャの状態をコピーデスティネーションに設定
    current_state_ = D3D12_RESOURCE_STATE_COPY_DEST;
    // テクスチャ用のリソースを作成
    auto check=device->CreateCommittedResource(
        &heap_prop,
        D3D12_HEAP_FLAG_NONE,
        &resource_desc_,
        current_state_,
        nullptr,
        IID_PPV_ARGS(&resource_)
    );
    
    // サブリソースデータ
    std::vector<D3D12_SUBRESOURCE_DATA> subresource_data(1);
    subresource_data[0].pData = pixels;
    subresource_data[0].RowPitch = width * 4;
    subresource_data[0].SlicePitch = width * height * 4;
    // リソースの更新
    update_subresources(device, queue, subresource_data);
    // ピクセルデータの解放
    stbi_image_free(pixels);

    // シェーダーリソースビュー作成
    shader_resource_view_ = pool->allocate();
    device->CreateShaderResourceView(resource_, nullptr, shader_resource_view_->cpu_handle());
}
// DDS読み込み
void Texture::load_dds(ID3D12Device* device, ID3D12CommandQueue* queue, DescriptorPool* pool, const std::string& file_name) {
    // DDSファイルの内容
    std::unique_ptr<uint8_t[]> dds;
    // サブリソースデータ
    std::vector<D3D12_SUBRESOURCE_DATA> subresources;
    // UTF16にファイル名を変換する
    std::wstring w_file_name = std::filesystem::path(file_name).wstring();
    // DDS読み込み（リソース作成とサブリソースデータ取得）
    bool is_cubemap{};
    HRESULT hr = DirectX::LoadDDSTextureFromFile(device, w_file_name.c_str(), &resource_, dds, subresources, 0, nullptr, &is_cubemap);
    if (FAILED(hr)) {
        // ファイルの読み込みに失敗
        throw std::runtime_error("Failed to load: " + file_name);
    }
    // 現在のリソースの状態を初期化
    current_state_ = D3D12_RESOURCE_STATE_COPY_DEST;
    // リソースディスクリプタを取得
    resource_desc_ = resource_->GetDesc();
    // リソースの更新
    update_subresources(device, queue, subresources);

    // シェーダーリソースビュー作成
    shader_resource_view_ = pool->allocate();
    if (is_cubemap) {
        // キューブマップの場合
        D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};
        srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
        srv_desc.TextureCube.MipLevels = resource_desc_.MipLevels;
        srv_desc.TextureCube.MostDetailedMip = 0;
        srv_desc.TextureCube.ResourceMinLODClamp = 0.0f;
        srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srv_desc.Format = resource_desc_.Format;
        device->CreateShaderResourceView(resource_, &srv_desc,shader_resource_view_->cpu_handle());
    }
    else {
        // その他のテクスチャ(2D/3D)の場合
        device->CreateShaderResourceView(resource_, nullptr,shader_resource_view_->cpu_handle());
    }
}


