#ifndef PIPELINE_STATE_ID_H_
#define PIPELINE_STATE_ID_H_

#include <cstdint>

// パイプラインステートID
namespace PipelineStateID {
    // スプライト（不透明）
    constexpr std::uint32_t Sprite2D_Opaque = 0;
    // スプライト（透明）
    constexpr std::uint32_t Sprite2D_Transparent = 1;
    // スプライト（加算）
    constexpr std::uint32_t Sprite2D_Additive = 2;
    // 3Dスプライト
    constexpr std::uint32_t Sprite3D_Opaque = 3;
    constexpr std::uint32_t Sprite3D_Transparent = 4;
    constexpr std::uint32_t Sprite3D_Additive = 5;
    // メッシュ
    constexpr std::uint32_t Mesh_Opaque = 6;
    // スキニングメッシュ
    constexpr std::uint32_t SkinnedMesh_Opaque = 7;
    // インスタンス描画スキニングメッシュ
    constexpr std::uint32_t InstanceSkinnedMesh_Opaque = 10;
    // スカイドーム
    constexpr std::uint32_t Mesh_Skydome = 8;
}

#endif