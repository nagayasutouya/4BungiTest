#ifndef GRAPHICS_DEVICE_H_
#define GRAPHICS_DEVICE_H_

#include <d3d12.h>

#include "GTLib/Graphics/GraphicsParts/BufferAndPool/FrameBuffer.h"
#include "GraphicsCommand.h"
#include "GTLib/Graphics/PipelineSetParts/PipelineState.h"
#include "RootSignature.h"
#include "GTLib/Graphics/GraphicsParts/BufferAndPool/DescriptorPool.h"
#include "GTLib/Graphics/GraphicsParts/BufferAndPool/VertexBuffer.h"
#include "GTLib/Graphics/GraphicsParts/BufferAndPool/IndexBuffer.h"
#include "GTLib/Graphics/GraphicsParts/BufferAndPool/ConstantBuffer.h"
#include "GTLib/Graphics/GraphicsParts/BufferAndPool/ConstantBufferPool.h"
#include "GTLib/Graphics/GraphicsParts/Texture/Texture.h"

// グラフィックスデバイスクラス
class GraphicsDevice {
public:
    // ディスクリプタ
    struct Desc {
        // ウィンドウの幅
        UINT        window_width{ 640 };
        // ウィンドウの高さ
        UINT        window_height{ 480 };
        // バックバッファのフォーマット
        DXGI_FORMAT back_buffer_format{ DXGI_FORMAT_R8G8B8A8_UNORM };
        // シェーダーリソースビューのディスクリプタ数
        UINT        num_descriptor{ 1024 };
        // コンスタントバッファプールのサイズ
        UINT        constant_buffer_size{ 1024 * 1024 * 8 };
    };
    // コンストラクタ
    GraphicsDevice(HWND hwnd, const Desc& desc = Desc());
    // デストラクタ
    ~GraphicsDevice();

    // ルートシグネチャの作成
    RootSignature* create_root_signature(UINT num_cbv, UINT num_srv, UINT num_inline_srv, UINT num_sampler, const D3D12_STATIC_SAMPLER_DESC sampler[]);
    // パイプラインステートの作成
    PipelineState* create_pipeline_state(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc);
    // 頂点バッファの作成
    VertexBuffer* create_vertex_buffer(const void* data, UINT size, UINT stride);
    // インデックスバッファの作成
    IndexBuffer* create_index_buffer(const void* data, UINT size, DXGI_FORMAT format);
    // コンスタントバッファの作成
    ConstantBuffer* create_constant_buffer(const void* data, UINT size);
    // テクスチャの作成
    Texture* create_texture(const std::string& file_name);
    //デスクデータを返す
    Desc desc_data()const { return desc_; }

    // バックバッファの描画開始
    void begin_render_back_buffer();
    // バックバッファの描画終了
    void end_render_back_buffer();
    // カラーバッファのクリア
    void clear_color_buffer(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
    // デプスバッファのクリア
    void clear_depth_buffer(float depth = 1.0f, UINT8 stencil = 0);
    // バックバッファのスワップ
    void swap_buffers(UINT sync_interval = 1, UINT flags = 0);
    // ルートシグネチャの設定
    void set_root_signature(RootSignature* root_signature);
    // パイプラインステートの設定
    void set_pipeline_state(PipelineState* pipeline_state,
        RootSignature* root_signature = nullptr);
    // 頂点バッファの設定
    void set_vertex_buffer(UINT slot, VertexBuffer* vertex_buffer);
    // インデックスバッファの設定
    void set_index_buffer(IndexBuffer* index_buffer);
    // テクスチャの設定
    void set_texture(UINT shader_register, Texture* texture);
    // コンスタントバッファの設定
    void set_constant_buffer(UINT shader_register, ConstantBuffer* constant_buffer);
    // コンスタントバッファの設定
    void set_constant_buffer(UINT shader_register, const void* data, UINT size);
    // ストラクテッドバッファの設定
    void set_structured_buffer(UINT shader_register, ConstantBuffer* constant_buffer);
    // インスタンス描画用ストラクテッドバッファの設定
    void set_instance_structured_buffer(UINT shader_register,const void* data, UINT size);

    // ディスクリプタヒープの設定
    void set_descriptor_heaps();

    // ビューポートの設定
    void set_viewport(float x, float y, float w, float h);
    // ビューポートの設定(現在のレンダーターゲットのサイズに合わせる）
    void set_viewport();
    // シザー矩形の設定
    void set_scissor_rect(float x, float y, float w, float h);

    // ポリゴンの描画（頂点インデックスあり）(インスタンス数の設定可能)
    void draw_indexed(D3D_PRIMITIVE_TOPOLOGY primitive, UINT index_count, UINT start_index = 0, UINT start_vertex = 0, UINT instance_count=1, UINT instance_id_offset=0);
    // ポリゴンの描画（頂点インデックスなし）(インスタンス数の設定可能)
    void draw(D3D_PRIMITIVE_TOPOLOGY primitive, UINT vertex_count, UINT start_vertex = 0, UINT instance_count=1, UINT instance_id_offset=0);

    // GPUの実行終了を待つ
    void wait_for_gpu() const;

    // DirectX12デバイスを取得
    ID3D12Device* d3d12_device() const;
    // コマンドキューを取得
    ID3D12CommandQueue* command_queue() const;
    // コマンドリストを取得
    ID3D12GraphicsCommandList* command_list() const;
    // ビューポートを取得
    const D3D12_VIEWPORT& viewport() const;
    // シザー矩形を取得
    const D3D12_RECT& scissor_rect() const;
    // 現在のバックバッファのインデックスを取得
    UINT current_back_buffer() const;

    // コピー禁止
    GraphicsDevice(const GraphicsDevice&) = delete;
    GraphicsDevice& operator = (const GraphicsDevice&) = delete;

private:
    // DirectX12デバイス
    ID3D12Device* device_{};
    // コマンドキュー
    ID3D12CommandQueue* command_queue_{};
    // 登録中のコマンドリスト
    ID3D12GraphicsCommandList* command_list_{};
    // グラフィックスコマンド
    GraphicsCommand* commands_[2]{};
    // ディスクリプタプール
    DescriptorPool* descriptor_pool_{};
    // フレームバッファ
    FrameBuffer* frame_buffer_{};
    // 現在のバックバッファのインデックス
    UINT current_back_buffer_{};
    // レンダーターゲット用のリソース
    ID3D12Resource* render_target_resource_{};
    // レンダーターゲットビュー
    D3D12_CPU_DESCRIPTOR_HANDLE render_target_view_{};
    // デプス・ステンシル用リソース
    ID3D12Resource* depth_stencil_resource_{};
    // デプス・ステンシルビュー
    D3D12_CPU_DESCRIPTOR_HANDLE depth_stencil_view_{};
    // ビューポート
    D3D12_VIEWPORT viewport_{};
    // シザー矩形
    D3D12_RECT scissor_rect_{};
    // コンスタントバッファプール
    ConstantBufferPool* constant_buffer_pool_[2]{};
    // 現在設定中のルートシグネチャ
    RootSignature* root_signature_{};
    //デスクデータ
    Desc desc_;
};

#endif