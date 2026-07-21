#ifndef VERTEX_BUFFER_H_
#define VERTEX_BUFFER_H_

#include <d3d12.h>

// 頂点バッファ
class VertexBuffer {
public:
    // コンストラクタ
    VertexBuffer(ID3D12Device* device, const void* data, UINT size, UINT stride);
    // デストラクタ
    ~VertexBuffer();
    // マップ
    void* map() const;
    // アンマップ
    void unmap() const;
    // 頂点バッファビューを返す
    const D3D12_VERTEX_BUFFER_VIEW& vertex_buffer_view() const;
    // コピー禁止
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator = (const VertexBuffer&) = delete;

private:
    // 頂点バッファ用のリソース
    ID3D12Resource* buffer_{};
    // 頂点バッファビュー
    D3D12_VERTEX_BUFFER_VIEW view_{};
};

#endif