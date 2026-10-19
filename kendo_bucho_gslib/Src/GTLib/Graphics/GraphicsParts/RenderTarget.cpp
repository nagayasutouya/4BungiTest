#include "RenderTarget.h"
#include "Texture/RenderTexture.h"
#include "Texture/DepthTexture.h"

// コンストラクタ
RenderTarget::RenderTarget(Texture* render_textures[], UINT num_render_textures, Texture* depth_texture)
    : num_render_textures_{ num_render_textures }
    , depth_texture_{ depth_texture } {
    for (UINT i = 0; i < num_render_textures; ++i) {
        render_textures_[i] = render_textures[i];
    }
}
// リソースバリア: 現在の状態 → RENDER_TARGET / DEPTH_WRITE（描画開始時）
void RenderTarget::barrier_begin(ID3D12GraphicsCommandList* cmd_list) const {
    for (UINT i = 0; i < num_render_textures_; ++i) {
        render_textures_[i]->barrier(cmd_list, D3D12_RESOURCE_STATE_RENDER_TARGET);
    }
    if (depth_texture_) {
        depth_texture_->barrier(cmd_list, D3D12_RESOURCE_STATE_DEPTH_WRITE);
    }
}
// リソースバリア: RENDER_TARGET / DEPTH_WRITE → PIXEL_SHADER_RESOURCE（描画終了時）
void RenderTarget::barrier_end(ID3D12GraphicsCommandList* cmd_list) const {
    for (UINT i = 0; i < num_render_textures_; ++i) {
        render_textures_[i]->barrier(cmd_list, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    }
    if (depth_texture_) {
        depth_texture_->barrier(cmd_list, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    }
}
// RTV/DSV をコマンドリストにバインドする（OMSetRenderTargets）
void RenderTarget::apply(ID3D12GraphicsCommandList* cmd_list) const {
    // RTVハンドルを収集
    D3D12_CPU_DESCRIPTOR_HANDLE rtv_handles[8]{};
    for (UINT i = 0; i < num_render_textures_; ++i) {
        rtv_handles[i] = render_textures_[i]->render_target_view();
    }
    // DSVハンドル（省略可能）
    D3D12_CPU_DESCRIPTOR_HANDLE* dsv_handle{};
    D3D12_CPU_DESCRIPTOR_HANDLE dsv{};
    if (depth_texture_) {
        dsv = depth_texture_->depth_stencil_view();
        dsv_handle = &dsv;
    }
    // レンダーターゲットを設定
    cmd_list->OMSetRenderTargets(num_render_textures_, rtv_handles, FALSE, dsv_handle);
}
// 全 RTV をクリアする
void RenderTarget::clear_color(ID3D12GraphicsCommandList* cmd_list,
    float r, float g, float b, float a) const {
    float color[4]{ r, g, b, a };
    for (UINT i = 0; i < num_render_textures_; ++i) {
        cmd_list->ClearRenderTargetView(render_textures_[i]->render_target_view(),
            color, 0, NULL);
    }
}
// DSV をクリアする
void RenderTarget::clear_depth(ID3D12GraphicsCommandList* cmd_list, float depth, UINT8 stencil) const {
    if (!depth_texture_) return; // デプステクスチャがなければ何もしない
    cmd_list->ClearDepthStencilView(depth_texture_->depth_stencil_view(),
        D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, depth, stencil, 0, NULL);
}
// 幅を返す
UINT RenderTarget::width() const {
    if (num_render_textures_ > 0 && render_textures_[0]) {
        return render_textures_[0]->width();
    }
    if (depth_texture_) {
        return depth_texture_->width();
    }
    return 0;
}
// 高さを返す
UINT RenderTarget::height() const {
    if (num_render_textures_ > 0 && render_textures_[0]) {
        return render_textures_[0]->height();
    }
    if (depth_texture_) {
        return depth_texture_->height();
    }
    return 0;
}