#ifndef INDEX_BUFFER_H_
#define INDEX_BUFFER_H_

#include <d3d12.h>

// インデックスバッファクラス
class IndexBuffer {
public:
    // コンストラクタ
    IndexBuffer(ID3D12Device* device, const void* data, UINT size, DXGI_FORMAT format);
    // デストラクタ
    ~IndexBuffer();
    // マップ
    void* map() const;
    // アンマップ
    void unmap() const;
    // インデックスバッファビューを返す
    const D3D12_INDEX_BUFFER_VIEW& index_buffer_view() const;
    // コピー禁止
    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer& operator = (const IndexBuffer&) = delete;

private:
    // バッファ
    ID3D12Resource* buffer_{};
    // インデックスバッファビュー
    D3D12_INDEX_BUFFER_VIEW view_{};
};

#endif