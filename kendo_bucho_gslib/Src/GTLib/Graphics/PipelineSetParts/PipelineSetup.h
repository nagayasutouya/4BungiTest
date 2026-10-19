#ifndef PIPELINE_SETUP_H_
#define PIPELINE_SETUP_H_

#include <d3d12.h>

// パイプラインステート構築関数
namespace PipelineSetup {
    // 全デフォルトパイプラインステートの一括登録
    void setup_defaults(
        DXGI_FORMAT rtv_format = DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_FORMAT dsv_format = DXGI_FORMAT_D32_FLOAT);
    // スプライト用のパイプラインステートを作成
    void create_pipeline_sprite2d(
        DXGI_FORMAT rtv_format = DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_FORMAT dsv_format = DXGI_FORMAT_D32_FLOAT);
    // 3Dスプライト用のパイプラインステート作成
    void create_pipeline_sprite3d(
        DXGI_FORMAT rtv_format = DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_FORMAT dsv_format = DXGI_FORMAT_D32_FLOAT);
    // メッシュ用のパイプラインステート作成
    void create_pipeline_mesh(
        DXGI_FORMAT rtv_format = DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_FORMAT dsv_format = DXGI_FORMAT_D32_FLOAT);
    // スキニングメッシュ用のパイプラインステート作成
    void create_pipeline_skinned_mesh(
        DXGI_FORMAT rtv_format = DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_FORMAT dsv_format = DXGI_FORMAT_D32_FLOAT);
    // インスタンス描画スキニングメッシュ用のパイプラインステート作成
    void create_pipeline_instance_skinned_mesh(
        DXGI_FORMAT rtv_format = DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_FORMAT dsv_format = DXGI_FORMAT_D32_FLOAT);
    // スカイドーム用のパイプラインステート作成
    void create_pipeline_skydome(
        DXGI_FORMAT rtv_format = DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_FORMAT dsv_format = DXGI_FORMAT_D32_FLOAT);
    // 画像転送用のパイプラインステート作成
    void create_pipeline_blit(
        DXGI_FORMAT rtv_format = DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_FORMAT dsv_format = DXGI_FORMAT_D32_FLOAT);
}

#endif