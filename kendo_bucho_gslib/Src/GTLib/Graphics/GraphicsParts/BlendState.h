#ifndef BLENDSTATE_H_
#define BLENDSTATE_H_

#include <d3d12.h>

// ブレンド状態の名前付き定数
namespace BlendState {
    // 不透明（ブレンドなし）
    constexpr D3D12_RENDER_TARGET_BLEND_DESC Opaque = {
        FALSE, FALSE,
        D3D12_BLEND_ONE,       D3D12_BLEND_ZERO,            D3D12_BLEND_OP_ADD,
        D3D12_BLEND_ONE,       D3D12_BLEND_ZERO,            D3D12_BLEND_OP_ADD,
        D3D12_LOGIC_OP_NOOP,
        D3D12_COLOR_WRITE_ENABLE_ALL
    };
    // 透明（アルファブレンド）
    // Color = SrcColor * SrcAlpha + DestColor * (1 - SrcAlpha)
    constexpr D3D12_RENDER_TARGET_BLEND_DESC Transparent = {
        TRUE, FALSE,
        D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_INV_SRC_ALPHA,   D3D12_BLEND_OP_ADD,
        D3D12_BLEND_ONE,       D3D12_BLEND_ZERO,            D3D12_BLEND_OP_ADD,
        D3D12_LOGIC_OP_NOOP,
        D3D12_COLOR_WRITE_ENABLE_ALL
    };
    // 加算
    // Color = SrcColor * SrcAlpha + DestColor
    constexpr D3D12_RENDER_TARGET_BLEND_DESC Additive = {
        TRUE, FALSE,
        D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_ONE,             D3D12_BLEND_OP_ADD,
        D3D12_BLEND_ONE,       D3D12_BLEND_ZERO,            D3D12_BLEND_OP_ADD,
        D3D12_LOGIC_OP_NOOP,
        D3D12_COLOR_WRITE_ENABLE_ALL
    };
}

#endif

