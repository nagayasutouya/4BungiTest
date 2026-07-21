#ifndef CONSTANT_BUFFER_POOL_H_
#define CONSTANT_BUFFER_POOL_H_

#include <d3d12.h>

// コンスタントバッファプールクラス
class ConstantBufferPool {
public:
    // コンストラクタ
    ConstantBufferPool(ID3D12Device* device, UINT buffer_size);
    // デストラクタ
    ~ConstantBufferPool();
    // バッファの位置をリセットする
    void reset();
    // コンスタントバッファに値を設定する
    D3D12_GPU_VIRTUAL_ADDRESS upload(const void* data, UINT size);
    // コピー禁止
    ConstantBufferPool(const ConstantBufferPool&) = delete;
    ConstantBufferPool& operator = (const ConstantBufferPool&) = delete;

private:
    // バッファ
    ID3D12Resource* buffer_{ nullptr };
    // バッファの先頭アドレス
    char* buffer_ptr_{ nullptr };
    // バッファサイズ
    UINT                    buffer_size_{ 0 };
    // 現在のバッファの位置
    UINT                    current_buffer_offset_{ 0 };
};

#endif