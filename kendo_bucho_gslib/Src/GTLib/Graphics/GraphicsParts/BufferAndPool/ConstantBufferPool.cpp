#include "ConstantBufferPool.h"
#include <memory>
#include <cassert>
#include "GTLib/Graphics/GraphicsParts/d3dx12.h"

// コンストラクタ
ConstantBufferPool::ConstantBufferPool(ID3D12Device* device, UINT buffer_size) :
    buffer_size_{ (buffer_size + 0xff) & ~0xff } { // バッファサイズは256バイトアライメント
    // コンスタントバッファ用のリソースを作成する
    CD3DX12_HEAP_PROPERTIES heap_prop{ D3D12_HEAP_TYPE_UPLOAD };
    const D3D12_RESOURCE_DESC& res_desc{ CD3DX12_RESOURCE_DESC::Buffer(buffer_size_) };
    device->CreateCommittedResource(
        &heap_prop,
        D3D12_HEAP_FLAG_NONE,
        &res_desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&buffer_)
    );
    // バッファの先頭アドレスを取得
    buffer_->Map(0, nullptr, (void**)&buffer_ptr_);
}
// デストラクタ
ConstantBufferPool::~ConstantBufferPool() {
    buffer_->Unmap(0, nullptr);
    buffer_->Release();
}
// バッファの位置をリセットする
void ConstantBufferPool::reset() {
    current_buffer_offset_ = 0;
}
// コンスタントバッファに値を設定する
D3D12_GPU_VIRTUAL_ADDRESS ConstantBufferPool::upload(const void* data, UINT size) {
    // バッファサイズを256バイトの倍数に調整（256バイトアライメント)
    UINT aligned_size = (size + 0xff) & ~0xff;
    // バッファサイズを超えていないかチェック
    assert((current_buffer_offset_ + aligned_size) <= buffer_size_);
    // コンスタントバッファにデータをコピーする
    std::memcpy(buffer_ptr_ + current_buffer_offset_, data, size);
    // バッファの先頭アドレスを計算する
    D3D12_GPU_VIRTUAL_ADDRESS buffer_location = buffer_->GetGPUVirtualAddress() + current_buffer_offset_;
    // バッファの位置を更新
    current_buffer_offset_ += aligned_size;
    // バッファの先頭アドレスを返す
    return buffer_location;
}