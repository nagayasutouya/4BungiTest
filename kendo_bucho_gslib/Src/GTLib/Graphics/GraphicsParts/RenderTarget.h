#ifndef RENDER_TARGET_H_
#define RENDER_TARGET_H_

#include <d3d12.h>

#include "Texture/RenderTexture.h"
#include "Texture/DepthTexture.h"

// レンダーターゲットクラス
class RenderTarget {
public:
    // コンストラクタ
    RenderTarget(Texture* render_textures[], UINT num_render_textures, Texture* depth_texture);

    // リソースバリア: 現在の状態 → RENDER_TARGET / DEPTH_WRITE（描画開始時）
    void barrier_begin(ID3D12GraphicsCommandList* cmd_list) const;
    // リソースバリア: RENDER_TARGET / DEPTH_WRITE → PIXEL_SHADER_RESOURCE（描画終了時）
    void barrier_end(ID3D12GraphicsCommandList* cmd_list) const;

    // RTV/DSV をコマンドリストにバインドする（OMSetRenderTargets）
    void apply(ID3D12GraphicsCommandList* cmd_list) const;

    // 全RTVをクリアする
    void clear_color(ID3D12GraphicsCommandList* cmd_list, float r, float g, float b, float a) const;
    // DSVをクリアする
    void clear_depth(ID3D12GraphicsCommandList* cmd_list, float depth, UINT8 stencil) const;

    // 幅を返す
    UINT width()  const;
    // 高さを返す
    UINT height() const;

private:
    UINT num_render_textures_{};    // レンダーテクスチャ数
    Texture* render_textures_[D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT]{};// レンダーテクスチャ
    Texture* depth_texture_{};// デプステクスチャ
};

#endif