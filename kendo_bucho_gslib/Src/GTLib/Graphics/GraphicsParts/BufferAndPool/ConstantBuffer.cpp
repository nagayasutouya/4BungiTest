#include "ConstantBuffer.h"
#include "GTLib/Graphics/GraphicsParts/d3dx12.h"
#include <memory>

// コンストラクタ
ConstantBuffer::ConstantBuffer(ID3D12Device* device, const void* data, UINT size) {
    // ヒーププロパティの設定
    CD3DX12_HEAP_PROPERTIES heap_prop{ D3D12_HEAP_TYPE_UPLOAD };
    // リソースの設定(256バイトアライメント）
    const D3D12_RESOURCE_DESC& res_desc{ CD3DX12_RESOURCE_DESC::Buffer((size + 0xff) & ~0xff) };
    // コンスタントバッファ用のリソースを作成
    device->CreateCommittedResource(
        &heap_prop,
        D3D12_HEAP_FLAG_NONE,
        &res_desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&buffer_)
    );
    if (data != nullptr) {
        // 初期化データをコピーする
        std::memcpy(map(), data, size);
        unmap();
    }
    // コンスタントバッファビューを作成する
    view_.BufferLocation = buffer_->GetGPUVirtualAddress();
    view_.SizeInBytes = static_cast<UINT>(buffer_->GetDesc().Width);
}

// デストラクタ
ConstantBuffer::~ConstantBuffer() {
    // バッファの削除
    buffer_->Release();
}

// マップ
void* ConstantBuffer::map() const {
    void* mapped_ptr{ nullptr };
    buffer_->Map(0, nullptr, &mapped_ptr);
    return mapped_ptr;
}

// アンマップ
void ConstantBuffer::unmap() const {
    buffer_->Unmap(0, nullptr);
}

// コンスタントバッファビューを取得
const D3D12_CONSTANT_BUFFER_VIEW_DESC& ConstantBuffer::constant_buffer_view() const {
    return view_;
}