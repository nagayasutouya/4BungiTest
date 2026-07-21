#include "VertexBuffer.h"
#include "GTLib/Graphics/GraphicsParts/d3dx12.h"
#include <memory>

// コンストラクタ
VertexBuffer::VertexBuffer(ID3D12Device* device, const void* data, UINT size, UINT stride) {
    // ヒーププロパティの設定
    CD3DX12_HEAP_PROPERTIES heap_prop{ D3D12_HEAP_TYPE_UPLOAD };
    // リソースの設定
    const D3D12_RESOURCE_DESC& res_desc{ CD3DX12_RESOURCE_DESC::Buffer(size) };
    // 頂点バッファ用のリソースを作成
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
    // 頂点バッファビューの作成
    view_.BufferLocation = buffer_->GetGPUVirtualAddress();
    view_.SizeInBytes = size;
    view_.StrideInBytes = stride;
}

// デストラクタ
VertexBuffer::~VertexBuffer() {
    buffer_->Release();
}

// マップ
void* VertexBuffer::map() const {
    void* mapped_ptr{};
    buffer_->Map(0, nullptr, &mapped_ptr);
    return mapped_ptr;
}

// アンマップ
void VertexBuffer::unmap() const {
    buffer_->Unmap(0, nullptr);
}

// 頂点バッファビューを返す
const D3D12_VERTEX_BUFFER_VIEW& VertexBuffer::vertex_buffer_view() const {
    return view_;
}