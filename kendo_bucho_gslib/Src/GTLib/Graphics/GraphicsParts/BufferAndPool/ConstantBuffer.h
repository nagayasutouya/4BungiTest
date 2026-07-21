#ifndef CONSTANT_BUFFER_H_
#define CONSTANT_BUFFER_H_

#include <d3d12.h>

// コンスタントバッファ
class ConstantBuffer {
public:
    // コンストラクタ
    ConstantBuffer(ID3D12Device* device, const void* data, UINT size);
    // デストラクタ
    ~ConstantBuffer();
    // マップ
    void* map() const;
    // アンマップ
    void unmap() const;
    // コンスタントバッファビューを取得
    const D3D12_CONSTANT_BUFFER_VIEW_DESC& constant_buffer_view() const;

    // コピー禁止
    ConstantBuffer(const ConstantBuffer&) = delete;
    ConstantBuffer& operator = (const ConstantBuffer&) = delete;
private:
    // バッファ
    ID3D12Resource* buffer_;
    // コンスタントバッファビュー
    D3D12_CONSTANT_BUFFER_VIEW_DESC view_;
};

#endif