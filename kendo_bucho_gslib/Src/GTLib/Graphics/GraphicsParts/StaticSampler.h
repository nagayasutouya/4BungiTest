#ifndef STATIC_SAMPLERS_H_
#define STATIC_SAMPLERS_H_

#include <array>
#include "d3dx12.h"

// 汎用的なスタティックサンプラー
inline constexpr std::array<D3D12_STATIC_SAMPLER_DESC, 7> default_static_samplers{ {
        // --------------------------------------------------------
        // s0: Linear / Wrap
        // 用途: 汎用的なテクスチャ、パーティクル
        // --------------------------------------------------------
        {
            D3D12_FILTER_MIN_MAG_MIP_LINEAR,
            D3D12_TEXTURE_ADDRESS_MODE_WRAP,
            D3D12_TEXTURE_ADDRESS_MODE_WRAP,
            D3D12_TEXTURE_ADDRESS_MODE_WRAP,
            0.0f,
            16,
            D3D12_COMPARISON_FUNC_LESS_EQUAL,
            D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE,
            0.0f,
            D3D12_FLOAT32_MAX,
            1,                                  // ShaderRegister (s1)
            0,
            D3D12_SHADER_VISIBILITY_ALL
        },
    // --------------------------------------------------------
    // s1: Anisotropic / Wrap
    // 用途: メイン3D描画用（地形、壁など）。最も汎用的。
    // --------------------------------------------------------
    {
        D3D12_FILTER_ANISOTROPIC,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,
        0.0f,
        8,
        D3D12_COMPARISON_FUNC_LESS_EQUAL,
        D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE,
        0.0f,
        D3D12_FLOAT32_MAX,
        0,                                  // ShaderRegister (s0)
        0,                                  // RegisterSpace
        D3D12_SHADER_VISIBILITY_ALL
    },
    // --------------------------------------------------------
    // s2: Linear / Clamp
    // 用途: ポストエフェクト、ブルーム、UI
    // --------------------------------------------------------
    {
        D3D12_FILTER_MIN_MAG_MIP_LINEAR,
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
        0.0f,
        16,
        D3D12_COMPARISON_FUNC_LESS_EQUAL,
        D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE,
        0.0f,
        D3D12_FLOAT32_MAX,
        2,                                  // ShaderRegister (s2)
        0,
        D3D12_SHADER_VISIBILITY_ALL
    },
    // --------------------------------------------------------
    // s3: Point / Clamp
    // 用途: UI、フォント、G-Bufferのピクセル単位アクセス
    // --------------------------------------------------------
    {
        D3D12_FILTER_MIN_MAG_MIP_POINT,
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
        0.0f,
        16,
        D3D12_COMPARISON_FUNC_LESS_EQUAL,
        D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE,
        0.0f,
        D3D12_FLOAT32_MAX,
        3,                                  // ShaderRegister (s3)
        0,
        D3D12_SHADER_VISIBILITY_ALL
    },
    // --------------------------------------------------------
    // s4: Shadow Comparison / Border
    // 用途: シャドウマップ (SampleCmp関数用)
    // --------------------------------------------------------
    {
        D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT, // PCF用フィルタ
        D3D12_TEXTURE_ADDRESS_MODE_BORDER,              // Border(範囲外は影にならないようにする)
        D3D12_TEXTURE_ADDRESS_MODE_BORDER,
        D3D12_TEXTURE_ADDRESS_MODE_BORDER,
        0.0f,
        16,
        D3D12_COMPARISON_FUNC_LESS_EQUAL,               // 比較関数
        D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE,         // 境界色(白=1.0=影なし)
        0.0f,
        D3D12_FLOAT32_MAX,
        4,                                              // ShaderRegister (s4)
        0,
        D3D12_SHADER_VISIBILITY_ALL
    },
    // --------------------------------------------------------
    // s5: Point / Wrap
    // 用途: ドット絵、ノイズ、タイリング
    // --------------------------------------------------------
    {
        D3D12_FILTER_MIN_MAG_MIP_POINT,     // Filter
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,    // AddressU
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,    // AddressV
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,    // AddressW
        0.0f,
        16,
        D3D12_COMPARISON_FUNC_LESS_EQUAL,
        D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE,
        0.0f,
        D3D12_FLOAT32_MAX,
        5,                                  // ShaderRegister (s5)
        0,
        D3D12_SHADER_VISIBILITY_ALL
    },
    // --------------------------------------------------------
    // s6: Anisotropic / Clamp
    // 用途: 空などビルボードの描画
    // --------------------------------------------------------
    {
        D3D12_FILTER_ANISOTROPIC,
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
        0.0f,
        8,
        D3D12_COMPARISON_FUNC_LESS_EQUAL,
        D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE,
        0.0f,
        D3D12_FLOAT32_MAX,
        6,                                  // ShaderRegister (s6)
        0,
        D3D12_SHADER_VISIBILITY_ALL
    }
} };

#endif

