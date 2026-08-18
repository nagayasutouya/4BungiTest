#include "GraphicsDevice.h"
#include "d3dx12.h"
#include <string>
#include <initguid.h>

// コンストラクタ
GraphicsDevice::GraphicsDevice(HWND hwnd, const Desc& desc) {
#ifdef _DEBUG
    // Debugモード時はデバッグレイヤーを有効にする
    ID3D12Debug* debug_layer{};
    D3D12GetDebugInterface(IID_PPV_ARGS(&debug_layer));
    debug_layer->EnableDebugLayer();
    debug_layer->Release();
#endif
    // DirectX12デバイスの作成
    D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device_));
    // コマンドキューの作成
    D3D12_COMMAND_QUEUE_DESC cmd_queue_desc{};
    cmd_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    cmd_queue_desc.NodeMask = 0;
    cmd_queue_desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    cmd_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    device_->CreateCommandQueue(&cmd_queue_desc, IID_PPV_ARGS(&command_queue_));
    // フレームバッファの作成
    frame_buffer_ = new FrameBuffer{ device_, hwnd, desc.window_width, desc.window_height, desc.back_buffer_format, command_queue_ };
    // 現在のバックバッファのインデックスを取得
    current_back_buffer_ = frame_buffer_->back_buffer_index();
    // グラフィックスコマンドの作成
    commands_[0] = new GraphicsCommand{ device_, command_queue_ };
    commands_[1] = new GraphicsCommand{ device_, command_queue_ };
    // コマンドリストの取得
    command_list_ = commands_[current_back_buffer_]->reset().list();
    // ディスクリプタプールを作成する
    D3D12_DESCRIPTOR_HEAP_DESC desc_heap_desc{};
    desc_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    desc_heap_desc.NodeMask = 0;
    desc_heap_desc.NumDescriptors = desc.num_descriptor;
    desc_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    descriptor_pool_ = new DescriptorPool{ device_, desc_heap_desc };
    // コンスタントバッファプールを作成する
    constant_buffer_pool_[0] = new ConstantBufferPool{ device_, desc.constant_buffer_size };
    constant_buffer_pool_[1] = new ConstantBufferPool{ device_, desc.constant_buffer_size };
}
// デストラクタ
GraphicsDevice::~GraphicsDevice() {
    // GPUの実行を待つ
    wait_for_gpu();
    // コンスタントバッファプールの削除
    delete constant_buffer_pool_[1];
    delete constant_buffer_pool_[0];
    // ディスクリプタプールの削除
    delete descriptor_pool_;
    // コマンドリストの削除
    delete commands_[1];
    delete commands_[0];
    // フレームバッファの削除
    delete frame_buffer_;
    // コマンドキューの削除
    command_queue_->Release();
    // DirectX12デバイスの削除
    device_->Release();
}

// ルートシグネチャの作成
RootSignature* GraphicsDevice::create_root_signature(UINT num_cbv, UINT num_srv,UINT num_inline_srv,UINT num_sampler, const D3D12_STATIC_SAMPLER_DESC sampler[]) {
    return new RootSignature(device_, num_cbv, num_srv,num_inline_srv ,num_sampler, sampler);
}

// パイプラインステートの作成
PipelineState* GraphicsDevice::create_pipeline_state(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc) {
    return new PipelineState(device_, desc);
}

// 頂点バッファの作成
VertexBuffer* GraphicsDevice::create_vertex_buffer(const void* data, UINT size, UINT stride) {
    return new VertexBuffer(device_, data, size, stride);
}

// インデックスバッファの作成
IndexBuffer* GraphicsDevice::create_index_buffer(const void* data, UINT size, DXGI_FORMAT format) {
    return new IndexBuffer(device_, data, size, format);
}

// コンスタントバッファの作成
ConstantBuffer* GraphicsDevice::create_constant_buffer(const void* data, UINT size) {
    return new ConstantBuffer(device_, data, size);
}

// テクスチャの作成
Texture* GraphicsDevice::create_texture(const std::string& file_name) {
    return new Texture(device_, command_queue_, descriptor_pool_, file_name);
}

// バックバッファの描画開始
void GraphicsDevice::begin_render_back_buffer() {
    // バックバッファのレンダーターゲットの取得
    render_target_resource_ = frame_buffer_->back_buffer();
    render_target_view_ = frame_buffer_->render_target_view();
    // デプスステンシルバッファの取得
    depth_stencil_resource_ = frame_buffer_->depth_stencil_buffer();
    depth_stencil_view_ = frame_buffer_->depth_stencil_view();
    // リソースバリアーの設定
    const D3D12_RESOURCE_BARRIER& barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        render_target_resource_, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    command_list_->ResourceBarrier(1, &barrier);
    // レンダーターゲットの設定
    command_list_->OMSetRenderTargets(1, &render_target_view_, false, &depth_stencil_view_);
    // ビューポートの設定（現在のレンダーターゲットのサイズにする）
    set_viewport();
}

// バックバッファの描画終了
void GraphicsDevice::end_render_back_buffer() {
    // リソースバリアーの設定
    const D3D12_RESOURCE_BARRIER& barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        render_target_resource_, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    command_list_->ResourceBarrier(1, &barrier);
}

// カラーバッファのクリア
void GraphicsDevice::clear_color_buffer(float r, float g, float b, float a) {
    // レンダーターゲットがなければ何もしない
    if (render_target_resource_ == nullptr) return;
    float clear_color[]{ r, g, b, a };
    command_list_->ClearRenderTargetView(render_target_view_, clear_color, 0, NULL);
}

// デプス・ステンシルバッファのクリア
void GraphicsDevice::clear_depth_buffer(float depth, UINT8 stencil) {
    // デプス・ステンシルビューがなければ何もしない
    if (depth_stencil_resource_ == nullptr) return;
    // デプス・ステンシルバッファのクリア
    command_list_->ClearDepthStencilView(depth_stencil_view_, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, depth, stencil, 0, NULL);
}
// バックバッファのスワップ
void GraphicsDevice::swap_buffers(UINT sync_interval, UINT flags) {
    // 現在のフレームのコマンドバッファを実行する
    commands_[current_back_buffer_]->execute();
    // バックバッファのスワップ
    frame_buffer_->swap_buffers(sync_interval, flags);
    // 次のフレームのバッファのインデックスを取得する
    current_back_buffer_ = frame_buffer_->back_buffer_index();
    // 次のフレームが描画可能になるまで待つ（前回のフレームの描画終了待つ）
    commands_[current_back_buffer_]->wait();
    // 次のフレームのコマンドリストをリセットする
    command_list_ = commands_[current_back_buffer_]->reset().list();
    // 次のフレームのコンスタントバッファプールをリセットする
    constant_buffer_pool_[current_back_buffer_]->reset();
}

// ルートシグネチャの設定
void GraphicsDevice::set_root_signature(RootSignature* root_signature) {
    command_list_->SetGraphicsRootSignature(root_signature->root_signature());
    // ルートシグネチャを保存しておく
    root_signature_ = root_signature;
}

// パイプラインステートの設定
void GraphicsDevice::set_pipeline_state(PipelineState* pipeline_state, RootSignature* root_signature) {
    // パイプラインステートの設定
    command_list_->SetPipelineState(pipeline_state->pipeline_state());
    if (root_signature != nullptr) {
        // ルートシグネチャの設定
        set_root_signature(root_signature);
    }
}

// 頂点バッファの設定
void GraphicsDevice::set_vertex_buffer(UINT slot, VertexBuffer* vertex_buffer) {
    command_list_->IASetVertexBuffers(slot, 1, &vertex_buffer->vertex_buffer_view());
}

// インデックスバッファの設定
void GraphicsDevice::set_index_buffer(IndexBuffer* index_buffer) {
    command_list_->IASetIndexBuffer(&index_buffer->index_buffer_view());
}

// テクスチャの設定
void GraphicsDevice::set_texture(UINT shader_register, Texture* texture) {
    UINT root_index = root_signature_->srv_root_index() + shader_register;
    command_list_->SetGraphicsRootDescriptorTable(root_index, texture->shader_resource_view());
}

// コンスタントバッファの設定
void GraphicsDevice::set_constant_buffer(UINT shader_register, ConstantBuffer* constant_buffer) {
    UINT root_index = root_signature_->cbv_root_index() + shader_register;
    command_list_->SetGraphicsRootConstantBufferView(root_index,
        constant_buffer->constant_buffer_view().BufferLocation);
}

// コンスタントバッファの設定(コンスタントバッファプールを使用）
void GraphicsDevice::set_constant_buffer(UINT shader_register, const void* data, UINT size) {
    UINT root_index = root_signature_->cbv_root_index() + shader_register;
    command_list_->SetGraphicsRootConstantBufferView(root_index,
        constant_buffer_pool_[current_back_buffer_]->upload(data, size));
}

// ストラクテッドバッファの設定
void GraphicsDevice::set_structured_buffer(UINT shader_register, ConstantBuffer* constant_buffer) {
    UINT root_index = root_signature_->cbv_root_index() + shader_register;
    command_list_->SetGraphicsRootConstantBufferView(root_index,
        constant_buffer->constant_buffer_view().BufferLocation);
}

//インラインSRVの設定
void GraphicsDevice::set_shader_resource_view_inline(UINT shader_register, D3D12_GPU_VIRTUAL_ADDRESS srv_address){
    UINT root_index = root_signature_->inline_srv_root_index() + shader_register;
    command_list_->SetGraphicsRootShaderResourceView(root_index, srv_address);
}

// ディスクリプタヒープの設定
void GraphicsDevice::set_descriptor_heaps() {
    ID3D12DescriptorHeap* heaps[]{ descriptor_pool_->heap() };
    command_list_->SetDescriptorHeaps(1, heaps);
}

// ビューポートの設定
void GraphicsDevice::set_viewport(float x, float y, float w, float h) {
    // ビューポートの設定
    viewport_ = CD3DX12_VIEWPORT(x, y, w, h);
    command_list_->RSSetViewports(1, &viewport_);
    // シザー矩形の設定
    set_scissor_rect(viewport_.TopLeftX, viewport_.TopLeftY, viewport_.Width, viewport_.Height);
}

// ビューポートの設定(現在のレンダーターゲットのサイズに合わせる）
void GraphicsDevice::set_viewport() {
    // ビューポートの設定
    viewport_ = CD3DX12_VIEWPORT(render_target_resource_);
    command_list_->RSSetViewports(1, &viewport_);
    // シザー矩形の設定
    set_scissor_rect(viewport_.TopLeftX, viewport_.TopLeftY, viewport_.Width, viewport_.Height);
}

// シザー矩形の設定
void GraphicsDevice::set_scissor_rect(float x, float y, float w, float h) {
    scissor_rect_.left = (LONG)x;
    scissor_rect_.top = (LONG)y;
    scissor_rect_.right = scissor_rect_.left + (LONG)w;
    scissor_rect_.bottom = scissor_rect_.top + (LONG)h;
    command_list_->RSSetScissorRects(1, &scissor_rect_);
}

// ポリゴンの描画（頂点インデックスあり）
void GraphicsDevice::draw_indexed(D3D_PRIMITIVE_TOPOLOGY primitive, UINT index_count, UINT start_index, UINT start_vertex, UINT instance_count) {
    command_list_->IASetPrimitiveTopology(primitive);
    command_list_->DrawIndexedInstanced(index_count, instance_count, start_index, start_vertex, 0);
}

// ポリゴンの描画（頂点インデックスなし）
void GraphicsDevice::draw(D3D_PRIMITIVE_TOPOLOGY primitive, UINT vertex_count, UINT start_vertex, UINT instance_count) {
    command_list_->IASetPrimitiveTopology(primitive);
    command_list_->DrawInstanced(vertex_count, instance_count, start_vertex, 0);
}

// GPUの実行終了を待つ
void GraphicsDevice::wait_for_gpu() const {
    commands_[0]->wait_idle();
    commands_[1]->wait_idle();
}
// DirectX12デバイスを取得
ID3D12Device* GraphicsDevice::d3d12_device() const {
    return device_;
}

// コマンドキューを取得
ID3D12CommandQueue* GraphicsDevice::command_queue() const {
    return command_queue_;
}

// コマンドリストを取得
ID3D12GraphicsCommandList* GraphicsDevice::command_list() const {
    return command_list_;
}

// ビューポートを取得
const D3D12_VIEWPORT& GraphicsDevice::viewport() const {
    return viewport_;
}

// シザー矩形を取得
const D3D12_RECT& GraphicsDevice::scissor_rect() const {
    return scissor_rect_;
}

// 現在のバックバッファのインデックスを取得
UINT GraphicsDevice::current_back_buffer() const {
    return current_back_buffer_;
}