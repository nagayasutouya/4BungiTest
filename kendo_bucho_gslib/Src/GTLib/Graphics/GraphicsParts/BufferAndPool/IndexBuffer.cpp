#include "IndexBuffer.h"
#include "GTLib/Graphics/GraphicsParts/d3dx12.h"
#include <memory>

// コンストラクタ
IndexBuffer::IndexBuffer(ID3D12Device* device, const void* data, UINT size, DXGI_FORMAT format) {
    // ヒーププロパティの設定
    CD3DX12_HEAP_PROPERTIES heap_prop{ D3D12_HEAP_TYPE_UPLOAD };
    // リソースの設定
    const D3D12_RESOURCE_DESC& res_desc{ CD3DX12_RESOURCE_DESC::Buffer(size) };
    // インデックスバッファ用のバッファを作成
    device->CreateCommittedResource(
        &heap_prop,
        D3D12_HEAP_FLAG_NONE,
        &res_desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&buffer_));
    if (data != nullptr) {
        // 初期化データをコピーする
        std::memcpy(map(), data, size);
        unmap();
    }
    // インデックスバッファビューを作成
    view_.BufferLocation = buffer_->GetGPUVirtualAddress();
    view_.Format = format;
    view_.SizeInBytes = size;
}

// デストラクタ
IndexBuffer::~IndexBuffer() {
    buffer_->Release();
}

// マップ
void* IndexBuffer::map() const {
    void* mapped_ptr{};
    buffer_->Map(0, nullptr, &mapped_ptr);
    return mapped_ptr;
}

// アンマップ
void IndexBuffer::unmap() const {
    buffer_->Unmap(0, nullptr);
}

// インデックスバッファビューを返す
const D3D12_INDEX_BUFFER_VIEW& IndexBuffer::index_buffer_view() const {
    return view_;
}