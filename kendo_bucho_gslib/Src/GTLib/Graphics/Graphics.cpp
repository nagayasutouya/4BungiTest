#include "Graphics.h"
#include "GTLib/Math/Mathf.h"
#include "GraphicsParts/d3dx12.h"
#include <unordered_map>
#include <memory>

namespace {
    // スプライト用パラメータの設定
    struct Sprite2DParams {
        Vector2 size{ 0.0f, 0.0f };		   // スプライトのサイズ
        Vector2 position{ 0.0f, 0.0f };		   // スプライトの描画位置
        Vector2 scale{ 1.0f, 1.0f };		   // スプライトの拡大率
        float   angle{ 0.0f };			   // スプライトの回転角度
        float   padding{};				   // パディング
        Vector2 center{ 0.0f, 0.0f };		   // スプライトの中心
        Vector2 screen{ 0.0f, 0.0f };		   // スクリーンのサイズ
        Color   color{ 1.0f, 1.0f, 1.0f, 1.0f };   // スプライトの色
        Vector2 texture_position{ 0.0f, 0.0f };	   // テクスチャの切り出し開始位置
        Vector2 texture_size{ 1.0f, 1.0f };	   // テクスチャの切り出しサイズ
    };
    // 3Dスプライト用パラメータの設定
    struct Sprite3DParams {
        Vector3 position{ 0.0f, 0.0f, 0.0f };       // スプライトの描画位置
        float   angle{ 0.0f };                      // スプライトの回転角度
        Vector2 scale{ 1.0f, 1.0f };                // スプライトの拡大率
        Vector2 size{ 0.0f, 0.0f };                 // スプライトのサイズ
        Vector2 center{ 0.0f, 0.0f };               // スプライトの中心
        float   padding[2]{};                       // アライメント用パディング
        Color   color{ 1.0f, 1.0f, 1.0f, 1.0f };    // スプライトの色
        Vector2 texture_position{ 0.0f, 0.0f };     // テクスチャの切り出し開始位置
        Vector2 texture_size{ 1.0f, 1.0f };         // テクスチャの切り出しサイズ
    };
    // 3Dオブジェクト用パラメータ (b0)
    struct ObjectParams {
        Matrix4x4 world_matrix;                     // ワールド行列
    };
    // カメラ用パラメータ (b1)
    struct CameraParams {
        Matrix4x4 view_matrix;                      // ビュー行列
        Matrix4x4 projection_matrix;                // プロジェクション行列
        Vector3   view_position;                    // カメラの位置
        float     padding;                          // パディング
    };
    // ライト用パラメータ (b2)
    struct LightParams {
        Color   ambient;                            // 環境光
        Color   diffuse;                            // 拡散反射光
        Color   specular;                           // 鏡面反射光
        Vector3 position;                           // 位置
        float   padding;                            // パディング
    };
    // グラフィックスデバイス
    GraphicsDevice* s_graphics_device{ nullptr };
    // ルートシグネチャ
    std::unique_ptr<RootSignature> s_root_signature;
    // パイプラインステート管理用マップ
    std::unordered_map<std::uint32_t, std::unique_ptr<PipelineState>> s_pipeline_states;
    // テクスチャ管理用マップ
    std::unordered_map<std::uint32_t, std::unique_ptr<Texture>> s_textures;
    // メッシュ管理用マップ
    std::unordered_map<std::uint32_t, std::unique_ptr<Mesh>> s_meshes;
    // スキニングメッシュ管理用マップ
    std::unordered_map<std::uint32_t, std::unique_ptr<SkinnedMesh>> s_skinned_meshes;
    // インスタンスメッシュ管理用マップ
    std::unordered_map<std::uint32_t, std::unique_ptr<InstanceMesh>> s_instance_meshes;
    // インスタンススキニングメッシュ管理用マップ
    std::unordered_map<std::uint32_t, std::unique_ptr<InstanceSkinnedMesh>>s_instance_skinned_meshes;
    
    // スプライト用のパイプラインステートID
    std::uint32_t s_sprite2d_pipeline_state_id{ 0 };
    // 3Dスプライト用のパイプラインステートID
    std::uint32_t s_sprite3d_pipeline_state_id{ 0 };
    // メッシュ用のパイプラインステートID
    std::uint32_t s_mesh_pipeline_state_id{ 0 };
    // スキニングメッシュ用のパイプラインステートID
    std::uint32_t s_skinned_mesh_pipeline_state_id{ 0 };
    // インスタンスメッシュ用のパイプラインステートID
    std::uint32_t s_instance_mesh_pipeline_state_id{ 0 };
    // インスタンススキニングメッシュ用のパイプラインステートID
    std::uint32_t s_instance_skinned_mesh_pipeline_state_id{0};

    // 3Dオブジェクト用パラメータ
    ObjectParams s_object_params{};
    // カメラ用パラメータ
    CameraParams s_camera_params{};
    // ライト用パラメータ
    LightParams  s_light_params{};

    // ダーティフラグ（GPU転送最適化用）
    bool s_cb_camera_dirty{ true };
    bool s_cb_light_dirty{ true };
}
// グラフィックス関連の初期化
void Graphics::initialize(GraphicsDevice* device) {
    // グラフィックスデバイスの保持
    s_graphics_device = device;
    // ルートシグネチャの作成
    CD3DX12_STATIC_SAMPLER_DESC sampler{ 0, D3D12_FILTER_MIN_MAG_MIP_LINEAR };
    s_root_signature = std::unique_ptr<RootSignature>(s_graphics_device->create_root_signature(8, 8, 2 ,1 ,&sampler));

    // オブジェクトパラメータの初期化
    s_object_params.world_matrix = Matrix4x4::identity();

    // カメラパラメータの初期化
    s_camera_params.view_matrix = Matrix4x4::identity();
    s_camera_params.projection_matrix = Matrix4x4::identity();
    s_camera_params.view_position = Vector3{ 0.0f, 0.0f, 0.0f };

    // ライトの初期化
    s_light_params.ambient = Color{ 0.2f, 0.2f, 0.2f, 1.0f };
    s_light_params.diffuse = Color{ 1.0f, 1.0f, 1.0f, 1.0f };
    s_light_params.specular = Color{ 1.0f, 1.0f, 1.0f, 1.0f };
    s_light_params.position = Vector3{ 100.0f, 100.0f, 100.0f };
}
// グラフィックス関連の終了処理
void Graphics::finalize() {
    // GPUの終了を待つ
    s_graphics_device->wait_for_gpu();
    // パイプラインステートの破棄
    s_pipeline_states.clear();
    // テクスチャの破棄
    s_textures.clear();
    // メッシュの破棄
    s_meshes.clear();
    // スキニングメッシュの破棄
    s_skinned_meshes.clear();
    // ルートシグニチャの削除
    s_root_signature.reset();
    // グラフィックスデバイスをnullptrで初期化
    s_graphics_device = nullptr;
}
// 描画の開始
void Graphics::begin_scene(float r, float g, float b, float a) {
    s_cb_camera_dirty = true;
    s_cb_light_dirty = true;
    set_root_signature();
    set_descriptor_heaps();
    s_graphics_device->begin_render_back_buffer();
    clear_color_buffer(r, g, b, a);
    clear_depth_buffer();
}
// 描画の終了
void Graphics::end_scene() {
    s_graphics_device->end_render_back_buffer();
}
// カラーバッファのクリア
void Graphics::clear_color_buffer(float r, float g, float b, float a) {
    s_graphics_device->clear_color_buffer(r, g, b, a);
}
// デプスバッファのクリア
void Graphics::clear_depth_buffer() {
    s_graphics_device->clear_depth_buffer();
}
// ビューポートの設定
void Graphics::set_viewport(float x, float y, float w, float h) {
    s_graphics_device->set_viewport(x, y, w, h);
}

// ルートシグネチャを設定
void Graphics::set_root_signature() {
    s_graphics_device->set_root_signature(s_root_signature.get());
}
// ルートシグニチャを取得
RootSignature* Graphics::get_root_signature() {
    return s_root_signature.get();
}
// ディスクリプタヒープを設定
void Graphics::set_descriptor_heaps() {
    // ディスクリプタヒープを設定
    s_graphics_device->set_descriptor_heaps();
}
// パイプラインステートを登録
void Graphics::add_pipeline_state(std::uint32_t pipeline_state_id, PipelineState* pipeline_state) {
    s_pipeline_states[pipeline_state_id] = std::unique_ptr<PipelineState>(pipeline_state);
}
// パイプラインステートを設定
void Graphics::set_pipeline_state(std::uint32_t pipeline_state_id) {
    auto it = s_pipeline_states.find(pipeline_state_id);
    if (it != s_pipeline_states.end()) {
        s_graphics_device->set_pipeline_state(it->second.get());
    }
}
// パイプラインステートの解放
void Graphics::delete_pipeline_state(std::uint32_t pipeline_state_id) {
    s_pipeline_states.erase(pipeline_state_id);
}

// 3D行列の設定 (変数の更新)
void Graphics::set_world_matrix(const Matrix4x4& world) {
    s_object_params.world_matrix = world;
}
// 視点行列の設定 (変数の更新)
void Graphics::set_view_matrix(const Matrix4x4& view) {
    s_camera_params.view_matrix = view;
    // ビュー行列の逆行列からカメラ位置を算出
    s_camera_params.view_position = view.inverse().position();
    s_cb_camera_dirty = true;
}
// 射影行列の設定 (変数の更新)
void Graphics::set_projection_matrix(const Matrix4x4& projection) {
    s_camera_params.projection_matrix = projection;
    s_cb_camera_dirty = true;
}
// ライトパラメータの設定
void Graphics::set_light(const Light& light) {
    s_light_params.ambient = light.ambient;
    s_light_params.diffuse = light.diffuse;
    s_light_params.specular = light.specular;
    s_light_params.position = light.position;
    s_cb_light_dirty = true;
}
// 3D行列のGPU転送
void Graphics::apply_object_params() {
    s_graphics_device->set_constant_buffer(0, &s_object_params, sizeof(s_object_params));
}
// カメラパラメータのGPU転送
void Graphics::apply_camera_params() {
    if (!s_cb_camera_dirty) return;
    s_graphics_device->set_constant_buffer(1, &s_camera_params, sizeof(s_camera_params));
    s_cb_camera_dirty = false;
}
// ライトパラメータのGPU転送
void Graphics::apply_light_params() {
    if (!s_cb_light_dirty) return;
    s_graphics_device->set_constant_buffer(2, &s_light_params, sizeof(s_light_params));
    s_cb_light_dirty = false;
}

// テクスチャの読み込み
Texture* Graphics::load_texture(std::uint32_t texture_id, const std::string& filename) {
    // テクスチャ作成
    Texture* texture = s_graphics_device->create_texture(filename);
    // テクスチャの登録
    s_textures[texture_id] = std::unique_ptr<Texture>(texture);
    // テクスチャを返す
    return texture;
}
// テクスチャ解放
void Graphics::delete_texture(std::uint32_t texture_id) {
    s_textures.erase(texture_id);
}
// テクスチャ取得
Texture* Graphics::get_texture(std::uint32_t texture_id) {
    auto it = s_textures.find(texture_id);
    if (it != s_textures.end()) {
        return it->second.get();
    }
    return nullptr;
}

// スプライトのパイプラインステートを設定
void Graphics::set_sprite2d_pipeline_state(std::uint32_t pipeline_state_id) {
    s_sprite2d_pipeline_state_id = pipeline_state_id;
}
// スプライトのパイプラインステートを取得
std::uint32_t Graphics::get_sprite2d_pipeline_state() {
    return s_sprite2d_pipeline_state_id;
}
// スプライト描画
void Graphics::draw_sprite2d(
    std::uint32_t texture_id,                           // テクスチャID
    const Vector2& position,                            // 座標
    const Rect* source_rect,                            // テクスチャの切り出し範囲
    const Vector2* center,                              // 回転の中心座標
    const Color* color,                                 // 色
    const Vector2* scale,                               // 拡大率
    float angle                                         // 回転角度
) {
    // テクスチャ取得
    Texture* texture = get_texture(texture_id);
    // スプライト用パラメータの設定
    Sprite2DParams params{};
    // スプライトの座標の設定
    params.position = position;
    // スプライトのサイズの設定
    params.size = Vector2{ (float)texture->width(), (float)texture->height() };
    // スプライトの拡大率の設定
    if (scale != nullptr) params.scale = *scale;
    // スプライトの回転角度の設定
    params.angle = Mathf::deg_to_rad(angle);
    // スプライトの中心の設定
    if (center != nullptr) params.center = *center;
    // スプライトの色
    if (color != nullptr) params.color = *color;
    // 切り出し座標の指定があるか？
    if (source_rect != nullptr) {
        // 切り出し幅の計算
        float w = source_rect->right - source_rect->left;
        // 切り出し高さの計算
        float h = source_rect->bottom - source_rect->top;
        // スプライトのサイズの設定
        params.size = Vector2{ w, h };
        // テクスチャの切り出し座標の設定
        params.texture_position = Vector2{ source_rect->left / texture->width(),
                                           source_rect->top / texture->height() };
        // テクスチャの切り出しサイズの指定
        params.texture_size = Vector2{ w / texture->width(), h / texture->height() };
    }
    // 画面サイズの設定
    const D3D12_VIEWPORT& view_port = s_graphics_device->viewport();
    params.screen = Vector2{ view_port.Width, view_port.Height };

    // スプライト用のパイプラインステート設定
    set_pipeline_state(s_sprite2d_pipeline_state_id);
    // テクスチャの設定
    s_graphics_device->set_texture(0, texture);
    // コンスタントバッファにパラメータ設定
    s_graphics_device->set_constant_buffer(0, &params, sizeof(params));
    // スプライトの描画
    s_graphics_device->draw(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, 4);
}

// 3Dスプライト用のパイプラインステート設定
void Graphics::set_sprite3d_pipeline_state(std::uint32_t pipeline_state_id) {
    s_sprite3d_pipeline_state_id = pipeline_state_id;
}
// 3Dスプライト用のパイプラインステート取得
std::uint32_t Graphics::get_sprite3d_pipeline_state() {
    return s_sprite3d_pipeline_state_id;
}
// 3Dスプライト(ビルボード)描画
void Graphics::draw_sprite3d(
    std::uint32_t     texture_id,          // テクスチャＩＤ
    const Vector3& position,            // 平行移動量
    const Rect& sprite_rect,         // スプライトの矩形
    const Rect* source_rect,         // テクスチャ内の位置
    const Color* color,               // カラー
    const Vector2* scale,               // スケール値
    float             angle                // 回転角度
) {
    // テクスチャ取得
    Texture* texture = get_texture(texture_id);
    if (texture == nullptr) return;

    // 3Dスプライト用パラメータの設定
    Sprite3DParams params{};
    // 平行移動量の設定
    params.position = position;
    // 回転角度の設定
    params.angle = Mathf::deg_to_rad(angle);
    // スケール値の設定
    if (scale != nullptr) params.scale = *scale;
    // 色の設定
    if (color != nullptr) params.color = *color;
    // スプライトのサイズの計算
    float w = sprite_rect.right - sprite_rect.left;
    float h = sprite_rect.bottom - sprite_rect.top;
    // スプライトの中心の設定
    params.center = Vector2{ -sprite_rect.left, -sprite_rect.top };
    // スプライトのサイズの指定
    params.size = Vector2{ w, h };

    // テクスチャの切り出し位置の計算
    if (source_rect != nullptr) {
        // 切り出し幅の計算
        float sw = source_rect->right - source_rect->left;
        // 切り出し高さの計算
        float sh = source_rect->bottom - source_rect->top;
        // テクスチャの切り出し座標の設定
        params.texture_position = Vector2{ source_rect->left / texture->width(), source_rect->top / texture->height() };
        // テクスチャの切り出しサイズの指定
        params.texture_size = Vector2{ sw / texture->width(), sh / texture->height() };
    }

    // パイプラインステートの設定
    set_pipeline_state(s_sprite3d_pipeline_state_id);

    // テクスチャの設定
    s_graphics_device->set_texture(0, texture);
    // パラメータの設定 (スプライト固有の情報)
    s_graphics_device->set_constant_buffer(0, &params, sizeof(params));

    // カメラの設定をGPUへ転送
    apply_camera_params();

    // 4頂点で描画
    s_graphics_device->draw(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, 4);
}

// メッシュの読み込み
Mesh* Graphics::load_mesh(std::uint32_t mesh_id, const std::string& filename) {
    // メッシュの作成
    Mesh* mesh = new Mesh(s_graphics_device);
    // メッシュの読み込み
    mesh->load(filename);
    // メッシュの登録
    s_meshes[mesh_id] = std::unique_ptr<Mesh>(mesh);
    // メッシュを返す
    return mesh;
}
// メッシュ削除
void Graphics::delete_mesh(std::uint32_t mesh_id) {
    s_meshes.erase(mesh_id);
}
// メッシュ取得
Mesh* Graphics::get_mesh(std::uint32_t mesh_id) {
    // メッシュを返す
    auto it = s_meshes.find(mesh_id);
    if (it != s_meshes.end()) {
        return it->second.get();
    }
    return nullptr;
}
// メッシュのパイプラインステート設定
void Graphics::set_mesh_pipeline_state(std::uint32_t pipeline_state_id) {
    s_mesh_pipeline_state_id = pipeline_state_id;
}
// メッシュのパイプラインステート取得
std::uint32_t Graphics::get_mesh_pipeline_state() {
    return s_mesh_pipeline_state_id;
}
// メッシュ描画
void Graphics::draw_mesh(std::uint32_t mesh_id) {
    // メッシュを取得
    Mesh* mesh = get_mesh(mesh_id);
    if (mesh == nullptr) return;

    // パイプラインステートの設定
    set_pipeline_state(get_mesh_pipeline_state());

    // 3D行列とライトの設定をGPUへ転送
    apply_object_params();
    apply_camera_params();
    apply_light_params();

    // 描画
    mesh->draw();
}

// スキニングメッシュの読み込み
SkinnedMesh* Graphics::load_skinned_mesh(
    std::uint32_t skinned_mesh_id,
    const std::string& mesh_file,
    const std::string& skeleton_file,
    const std::string& animation_file) {
    // スキニングメッシュの作成
    SkinnedMesh* sm = new SkinnedMesh(s_graphics_device);
    sm->load(mesh_file, skeleton_file, animation_file);
    // スキニングメッシュの登録
    s_skinned_meshes[skinned_mesh_id] = std::unique_ptr<SkinnedMesh>(sm);
    return sm;
}
// スキニングメッシュの読み込み
void Graphics::delete_skinned_mesh(std::uint32_t skinned_mesh_id) {
    s_skinned_meshes.erase(skinned_mesh_id);
}
// スキニングメッシュ取得
SkinnedMesh* Graphics::get_skinned_mesh(std::uint32_t skinned_mesh_id) {
    auto it = s_skinned_meshes.find(skinned_mesh_id);
    if (it != s_skinned_meshes.end()) {
        return it->second.get();
    }
    return nullptr;
}
// スキンメッシュのパイプラインステートを設定
void Graphics::set_skinned_mesh_pipeline_state(std::uint32_t pipeline_state_id) {
    s_skinned_mesh_pipeline_state_id = pipeline_state_id;
}
// スキンメッシュのパイプラインステートを取得
std::uint32_t Graphics::get_skinned_mesh_pipeline_state() {
    return s_skinned_mesh_pipeline_state_id;
}
// ローカル行列計算
void Graphics::compute_local_matrices(std::uint32_t skinned_mesh_id,
    int anim_id, float frame_no, Matrix4x4 local[]) {
    SkinnedMesh* sm = get_skinned_mesh(skinned_mesh_id);
    if (sm == nullptr) return;
    sm->compute_local_matrices(anim_id, frame_no, local);
}
// ブレンドされたローカル行列計算
void Graphics::compute_blended_matrices(std::uint32_t skinned_mesh_id,
    int anim_id1, float frame1,
    int anim_id2, float frame2,
    float weight, Matrix4x4 local[]) {
    SkinnedMesh* sm = get_skinned_mesh(skinned_mesh_id);
    if (sm == nullptr) return;
    sm->compute_blended_matrices(anim_id1, frame1, anim_id2, frame2, weight, local);
}
// ワールド行列計算
void Graphics::compute_world_matrices(std::uint32_t skinned_mesh_id,
    const Matrix4x4 local[], Matrix4x4 world[], const Matrix4x4& parent) {
    SkinnedMesh* sm = get_skinned_mesh(skinned_mesh_id);
    if (sm == nullptr) return;
    sm->compute_world_matrices(local, world, parent);
}
// スキニング行列計算
void Graphics::compute_skinned_matrices(std::uint32_t skinned_mesh_id,
    const Matrix4x4 world[], Matrix4x4 skinned[]) {
    SkinnedMesh* sm = get_skinned_mesh(skinned_mesh_id);
    if (sm == nullptr) return;
    sm->compute_skinned_matrices(world, skinned);
}
// スキニング行列をセットする
void Graphics::set_skinned_matrices(std::uint32_t skinned_mesh_id, const Matrix4x4 skinned[]) {
    SkinnedMesh* sm = get_skinned_mesh(skinned_mesh_id);
    if (sm == nullptr) return;
    sm->set_skinned_matrices(skinned);
}
// スキンメッシュを描画する（事前に演算および行列セットが必要）
void Graphics::draw_skinned_mesh(std::uint32_t skinned_mesh_id) {
    SkinnedMesh* sm = get_skinned_mesh(skinned_mesh_id);
    if (sm == nullptr) return;
    // パイプラインステートの設定
    set_pipeline_state(get_skinned_mesh_pipeline_state());

    // 3D行列とライトの設定をGPUへ転送
    apply_object_params();
    apply_camera_params();
    apply_light_params();
    // メッシュ描画
    sm->draw();
}
// アニメーションの終了フレームを返す
float Graphics::get_skinned_mesh_last_frame(std::uint32_t skinned_mesh_id, int anim_id) {
    SkinnedMesh* sm = get_skinned_mesh(skinned_mesh_id);
    if (sm == nullptr) return 0.0f;
    return sm->last_frame(anim_id);
}
// 指定したスキニングメッシュのボーン数を返す
std::size_t Graphics::get_skinned_mesh_bone_count(std::uint32_t skinned_mesh_id) {
    SkinnedMesh* sm = get_skinned_mesh(skinned_mesh_id);
    if (sm == nullptr) return 0;
    return sm->bone_count();
}

// インスタンスメッシュの読み込み
InstanceMesh* Graphics::load_instance_mesh(std::uint32_t instance_mesh_id, const std::string& filename) {
    // インスタンスメッシュの作成
    InstanceMesh* instance_mesh = new InstanceMesh(s_graphics_device);
    // インスタンスメッシュの読み込み
    instance_mesh->load(filename);
    // インスタンスメッシュの登録
    s_instance_meshes[instance_mesh_id] = std::unique_ptr<InstanceMesh>(instance_mesh);
    // インスタンスメッシュを返す
    return instance_mesh;
}
// インスタンスメッシュ削除
void Graphics::delete_instance_mesh(std::uint32_t instance_mesh_id) {
    s_instance_meshes.erase(instance_mesh_id);
}
// インスタンスメッシュ取得
InstanceMesh* Graphics::get_instance_mesh(std::uint32_t instance_mesh_id) {
    // インスタンスメッシュを返す
    auto it = s_instance_meshes.find(instance_mesh_id);
    if (it != s_instance_meshes.end()) {
        return it->second.get();
    }
    return nullptr;
}
// インスタンスメッシュのパイプラインステート設定
void Graphics::set_instance_mesh_pipeline_state(std::uint32_t pipeline_state_id) {
    s_instance_mesh_pipeline_state_id = pipeline_state_id;
}
// インスタンスメッシュのパイプラインステート取得
std::uint32_t Graphics::get_instance_mesh_pipeline_state() {
    return s_instance_mesh_pipeline_state_id;
}
// インスタンスメッシュ描画
void Graphics::draw_instance_mesh(std::uint32_t instance_mesh_id,unsigned int instance_count) {
    // インスタンスメッシュを取得
    InstanceMesh* instance_mesh = get_instance_mesh(instance_mesh_id);
    if (instance_mesh == nullptr) return;

    // パイプラインステートの設定
    set_pipeline_state(get_instance_mesh_pipeline_state());

    // 3D行列とライトの設定をGPUへ転送
    apply_object_params();
    apply_camera_params();
    apply_light_params();

    // 描画
    instance_mesh->draw(instance_count);
}

// インスタンススキニングメッシュの読み込み
InstanceSkinnedMesh* Graphics::load_instance_skinned_mesh(std::uint32_t instance_skinned_mesh_id,const std::string& mesh_file,const std::string& skeleton_file,const std::string& animation_file,int instance_count) {
    // インスタンススキニングメッシュの作成
    InstanceSkinnedMesh* sm = new InstanceSkinnedMesh(s_graphics_device);
    sm->load(mesh_file, skeleton_file, animation_file,instance_count);
    // インスタンススキニングメッシュの登録
    s_instance_skinned_meshes[instance_skinned_mesh_id] = std::unique_ptr<InstanceSkinnedMesh>(sm);
    return sm;
}
// インスタンススキニングメッシュの読み込み
void Graphics::delete_instance_skinned_mesh(std::uint32_t instance_skinned_mesh_id) {
    s_instance_skinned_meshes.erase(instance_skinned_mesh_id);
}
// インスタンススキニングメッシュ取得
InstanceSkinnedMesh* Graphics::get_instance_skinned_mesh(std::uint32_t instance_skinned_mesh_id) {
    auto it = s_instance_skinned_meshes.find(instance_skinned_mesh_id);
    if (it != s_instance_skinned_meshes.end()) {
        return it->second.get();
    }
    return nullptr;
}
// スキンメッシュのパイプラインステートを設定
void Graphics::set_instance_skinned_mesh_pipeline_state(std::uint32_t pipeline_state_id) {
    s_instance_skinned_mesh_pipeline_state_id = pipeline_state_id;
}
// スキンメッシュのパイプラインステートを取得
std::uint32_t Graphics::get_instance_skinned_mesh_pipeline_state() {
    return s_instance_skinned_mesh_pipeline_state_id;
}
// ローカル行列計算
void Graphics::compute_local_matrices_instance(std::uint32_t skinned_mesh_id, int instance_id, int anim_id, float frame_no, Matrix4x4 local[]) {
    InstanceSkinnedMesh* sm = get_instance_skinned_mesh(skinned_mesh_id);
    if (sm == nullptr) return;
    sm->compute_local_matrices(instance_id, anim_id, frame_no, local);
}
// ブレンドされたローカル行列計算
void Graphics::compute_blended_matrices_instance(std::uint32_t skinned_mesh_id,int instance_id,BlendedMat& mat,const float lerp_time) {
    InstanceSkinnedMesh* sm = get_instance_skinned_mesh(skinned_mesh_id);
    if (sm == nullptr) return;
    sm->compute_blended_matrices(instance_id,mat.prev_motion, mat.prev_frame, mat.current_motion, mat.current_frame, mat.lerp/lerp_time, mat.local);
}
// ワールド行列計算
void Graphics::compute_world_matrices_instance(std::uint32_t skinned_mesh_id, int instance_id, const Matrix4x4 local[], Matrix4x4 world[], const Matrix4x4& parent) {
    InstanceSkinnedMesh* sm = get_instance_skinned_mesh(skinned_mesh_id);
    if (sm == nullptr) return;
    sm->compute_world_matrices(instance_id, local, world, parent);
}
// スキニング行列計算
void Graphics::compute_skinned_matrices_instance(std::uint32_t skinned_mesh_id, int instance_id, const Matrix4x4 world[], Matrix4x4 skinned[]) {
    InstanceSkinnedMesh* sm = get_instance_skinned_mesh(skinned_mesh_id);
    if (sm == nullptr) return;
    sm->compute_skinned_matrices(instance_id, world, skinned);
}
// インスタンススキニング行列をセットする
void Graphics::set_instance_skinned_matrices(std::uint32_t instance_skinned_mesh_id,unsigned int instance_count, const Matrix4x4 instance_skinned[]) {
    InstanceSkinnedMesh* sm = get_instance_skinned_mesh(instance_skinned_mesh_id);
    if (sm == nullptr) return;
    sm->set_skinned_matrices(instance_skinned);
}
// インスタンス スキンメッシュを描画する（事前に演算および行列セットが必要）
void Graphics::draw_instance_skinned_mesh(std::uint32_t instance_skinned_mesh_id,unsigned int instance_count,const InstanceObject* first_obj) {
    InstanceSkinnedMesh* sm = get_instance_skinned_mesh(instance_skinned_mesh_id);
    if (sm == nullptr) return;
    // パイプラインステートの設定
    set_pipeline_state(get_instance_skinned_mesh_pipeline_state());
    // 3D行列とライトの設定をGPUへ転送
    s_graphics_device->set_shader_resource_view_inline(0, sm->set_all_object_data(first_obj, instance_count));
    apply_camera_params();
    apply_light_params();
    // メッシュ描画
    sm->draw(instance_count);
}
// アニメーションの終了フレームを返す
float Graphics::get_instance_skinned_mesh_last_frame(std::uint32_t instance_skinned_mesh_id, int anim_id) {
    InstanceSkinnedMesh* sm = get_instance_skinned_mesh(instance_skinned_mesh_id);
    if (sm == nullptr) return 0.0f;
    return sm->last_frame(anim_id);
}
// 指定したインスタンススキニングメッシュのボーン数を返す
std::size_t Graphics::get_instance_skinned_mesh_bone_count(std::uint32_t instance_skinned_mesh_id) {
    InstanceSkinnedMesh* sm = get_instance_skinned_mesh(instance_skinned_mesh_id);
    if (sm == nullptr) return 0;
    return sm->bone_count(0);
}


// GPUの待機
void Graphics::wait_for_gpu() {
    s_graphics_device->wait_for_gpu();
}
// グラフィックスデバイスの取得
GraphicsDevice* Graphics::get_graphics_device() {
    return s_graphics_device;
}