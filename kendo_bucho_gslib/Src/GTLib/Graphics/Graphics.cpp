#include "Graphics.h"
#include "GTLib/Math/Mathf.h"
#include "GraphicsParts/d3dx12.h"
#include "GraphicsParts/StaticSampler.h"
#include <unordered_map>
#include <memory>

namespace {
    // スプライト用パラメータの設定
    struct Sprite2DParams {
        Vector2 size{ 0.0f, 0.0f };		   // スプライトのサイズ
        Vector2 position{ 0.0f, 0.0f };		   // スプライトの描画位置
        
        Vector2 scale{ 1.0f, 1.0f };		   // スプライトの拡大率
        float   angle{ 0.0f };			   // スプライトの回転角度
        bool is_draw_circle{ false };//円形描画するかどうか
        char padding1{ 0 };
        char padding2{ 0 };
        char padding3{ 0 };

        float rect_angle{1.0f}; //全体の何％を描画するか(0～1)（円形）
        Vector2 round_vector{ 1.0f,1.0f };//回転方向
        float donut_radius{0.0f}; //ドーナツ描画時の空白部分
        
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
    // インスタンススキニングメッシュ管理用マップ
    std::unordered_map<std::uint32_t, std::unique_ptr<InstanceSkinnedMesh>>s_instance_skinned_meshes;
    //エフェクト
    GTEffect* g_gteffect_{ nullptr };

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
    s_root_signature = std::unique_ptr<RootSignature>(s_graphics_device->create_root_signature(8, 8,2,(UINT)default_static_samplers.size(), default_static_samplers.data()));

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
    //エフェクシアの破棄
    if(g_gteffect_!=nullptr)g_gteffect_->finish_effect();
    g_gteffect_ = nullptr;
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
// カメラパラメータのビュー行列を返す
Matrix4x4 Graphics::get_view_matrix() {
    return s_camera_params.view_matrix;
}
// カメラパラメータのプロジェクションを返す
Matrix4x4 Graphics::get_projection_matrix() {
    return s_camera_params.projection_matrix;
}
// カメラパラメータの座標を返す
Vector3 Graphics::get_view_position() {
    return s_camera_params.view_position;
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
    float angle,                                         // 回転角度
    bool is_draw_round,//回転描画するか
    const Vector2* round_vector,//回転方向
    float round_angle,//円形描画範囲
    float donut_radius//中心に空白作るか
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
    //回転描画するか
    params.is_draw_circle = is_draw_round;
    //回転方向
    if (round_vector != nullptr)params.round_vector = *round_vector;
    else params.round_vector = Vector2{ 1.0f,1.0f };
    //回転描画範囲
    params.rect_angle = round_angle;
    //ドーナツ中心の長さ
    params.donut_radius = donut_radius;
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
// スキニングメッシュ削除
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

//インスタンススキニングメッシュ
//インスタンススキニングメッシュ読み込み
InstanceSkinnedMesh* Graphics::load_instance_skinned_mesh(std::uint32_t skinned_mesh_id,const std::string& mesh_file,const std::string& skeleton_file,const std::string& animation_file) {
    //データなかったらロード
    SkinnedMesh* sm = get_skinned_mesh(skinned_mesh_id);
    if (sm == nullptr) {
        if(load_skinned_mesh(skinned_mesh_id,mesh_file,skeleton_file,animation_file) == nullptr)return nullptr;
    }
    //新規インスタンススキニングメッシュを作成し保存
    InstanceSkinnedMesh* ism = new InstanceSkinnedMesh(s_graphics_device);
    s_instance_skinned_meshes[skinned_mesh_id] = std::unique_ptr<InstanceSkinnedMesh>(ism);
    return ism;
}
// インスタンススキニングメッシュ削除
void Graphics::delete_instance_skinned_mesh(std::uint32_t skinned_mesh_id) {
    s_skinned_meshes.erase(skinned_mesh_id);
    s_instance_skinned_meshes.erase(skinned_mesh_id);
}
//  インスタンススキニングメッシュ取得
InstanceSkinnedMesh* Graphics::get_instance_skinned_mesh(std::uint32_t skinned_mesh_id) {
    auto it = s_instance_skinned_meshes.find(skinned_mesh_id);
    if (it != s_instance_skinned_meshes.end()) {
        return it->second.get();
    }
    return nullptr;
}
//  インスタンススキンメッシュのパイプラインステートを設定
void Graphics::set_instance_skinned_mesh_pipeline_state(std::uint32_t pipeline_state_id) {
    s_instance_skinned_mesh_pipeline_state_id = pipeline_state_id;
}
//  インスタンススキンメッシュのパイプラインステートを取得
std::uint32_t Graphics::get_instance_skinned_mesh_pipeline_state() {
    return s_instance_skinned_mesh_pipeline_state_id;
}
//各種データの追加
void Graphics::add_instacne_data(unsigned int mesh_id, const Matrix4x4& world_matrix, const Matrix4x4 skinned_matrix[],Color color) {
    InstanceSkinnedMesh* ism = get_instance_skinned_mesh(mesh_id);
    if (ism == nullptr) return;
    ism->add_instance_data(world_matrix,skinned_matrix,static_cast<unsigned int>(get_skinned_mesh_bone_count(mesh_id)),color);
}
//データの削除
void Graphics::reset_instacne_data() {
    for (auto& ism : s_instance_skinned_meshes) {
        ism.second->clear();
    }
}
//GPUデータ転送
void Graphics::draw_instance_skinned_mesh(unsigned int mesh_id) {
    SkinnedMesh* sm = get_skinned_mesh(mesh_id);
    InstanceSkinnedMesh* ism = get_instance_skinned_mesh(mesh_id);
    if (sm == nullptr||ism == nullptr) return;
    //インスタンス数が０なら無視
    if (ism->instance_count() <= 0)return;
    // パイプラインステートの設定
    set_pipeline_state(get_instance_skinned_mesh_pipeline_state());

    // 3D行列とライトの設定をGPUへ転送
    ism->set_skinned_matrices();
    ism->set_instance_matrices();
    apply_camera_params();
    apply_light_params();
    // メッシュ描画
    sm->draw(ism->instance_count());
}

//エフェクト
//エフェクトの初期化
void Graphics::initialize_effect(GTUINT effect_load_count_max, GTUINT instance_count_max) {
    delete g_gteffect_;
    g_gteffect_ = nullptr;
    g_gteffect_ = new GTEffect(s_graphics_device, s_graphics_device->command_queue(),2);
    //作成失敗は無視
    if (g_gteffect_ == nullptr)return;
    //初期化
    g_gteffect_->initialize(effect_load_count_max, instance_count_max);
}
GTEffect* Graphics::effect_data() {
    return g_gteffect_;
}

// レンダーターゲットの作成
RenderTexture* Graphics::create_render_texture(std::uint32_t texture_id,
    UINT width, UINT height, DXGI_FORMAT format, const Color* clear_color) {
    // 消去カラーの設定
    float color[4]{ 0.0f, 0.0f, 0.0f, 1.0f }; // デフォルトカラー（黒）
    if (clear_color) {
        color[0] = clear_color->r;
        color[1] = clear_color->g;
        color[2] = clear_color->b;
        color[3] = clear_color->a;
    }
    // RenderTextureの作成
    RenderTexture* texture = s_graphics_device->create_render_texture(
        width, height, format, color);
    // テクスチャの登録
    s_textures[texture_id] = std::unique_ptr<Texture>(texture);
    // テクスチャを返す
    return texture;
}
// デプステクスチャの作成
DepthTexture* Graphics::create_depth_texture(std::uint32_t texture_id,
    UINT width, UINT height, DXGI_FORMAT format) {
    // デプステクスチャの作成
    DepthTexture* texture = s_graphics_device->create_depth_texture(width, height, format);
    // テクスチャの登録
    s_textures[texture_id] = std::unique_ptr<Texture>(texture);
    // テクスチャを返す
    return texture;
}
// レンダーターゲットの描画開始（デプスバッファなし）
void Graphics::begin_render_target(std::uint32_t texture_id) {
    // 描画対象となるテクスチャを取得
    Texture* render_texture = get_texture(texture_id);
    if (render_texture == nullptr) return;

    Texture* render_textures[]{ render_texture };
    s_graphics_device->begin_render_target(render_textures, 1, nullptr);
}
// レンダーターゲットの描画開始（デプスバッファあり）
void Graphics::begin_render_target(std::uint32_t texture_id, std::uint32_t depth_texture_id) {
    // 描画対象となるテクスチャを取得
    Texture* render_texture = get_texture(texture_id);
    if (render_texture == nullptr) return;

    // デプスバッファを取得
    Texture* depth_texture = get_texture(depth_texture_id);
    if (depth_texture == nullptr) return;

    Texture* render_textures[]{ render_texture };
    s_graphics_device->begin_render_target(render_textures, 1, depth_texture);
}
// デプスバッファのみをレンダーターゲットに設定して描画開始
void Graphics::begin_depth_target(std::uint32_t depth_texture_id) {
    // デプスバッファを取得
    Texture* depth_texture = get_texture(depth_texture_id);
    if (depth_texture == nullptr) return;

    s_graphics_device->begin_render_target(nullptr, 0, depth_texture);
}
// レンダーターゲットの描画終了
void Graphics::end_render_target() {
    s_graphics_device->end_render_target();
}
// バックバッファにテクスチャをコピー
void Graphics::blit_to_back_buffer(std::uint32_t texture_id, std::uint32_t pipeline_state_id) {
    // テクスチャ取得
    Texture* texture = get_texture(texture_id);
    if (texture == nullptr) return;

    // 現在のビューポートのサイズ取得
    const D3D12_VIEWPORT& view_port = s_graphics_device->viewport();
    float view_width = view_port.Width;
    float view_height = view_port.Height;
    // ビューポートの位置
    float view_x = 0.0f;
    float view_y = 0.0f;

    // アスペクト比の計算
    float src_aspect = (float)texture->width() / (float)texture->height();
    float dst_aspect = view_width / view_height;
    // アスペクト比の比較
    if (src_aspect > dst_aspect) {
        // 元画像の方が横長 → 上下に黒帯を入れる（レターボックス）
        view_height = view_port.Width / src_aspect;
        view_y = (view_port.Height - view_height) * 0.5f; // 上下の真ん中に配置
    }
    else if (src_aspect < dst_aspect) {
        // 元画像の方が縦長 → 左右に黒帯を入れる（ピラーボックス）
        view_width = view_port.Height * src_aspect;
        view_x = (view_port.Width - view_width) * 0.5f; // 左右の真ん中に配置
    }
    // ビューポートの設定
    s_graphics_device->set_viewport(view_x, view_y, view_width, view_height);

    // パイプラインステートの設定
    set_pipeline_state(pipeline_state_id);
    // テクスチャの設定
    s_graphics_device->set_texture(0, texture);
    // 3頂点で描画
    s_graphics_device->draw(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, 3);
}
// レンダーターゲットにテクスチャをコピー
void Graphics::blit_to_render_target(std::uint32_t texture_id, std::uint32_t pipeline_state_id) {
    // テクスチャ取得
    Texture* texture = get_texture(texture_id);
    if (texture == nullptr) return;

    // パイプラインステートの設定
    set_pipeline_state(pipeline_state_id);
    // テクスチャの設定
    s_graphics_device->set_texture(0, texture);
    // 3頂点で描画
    s_graphics_device->draw(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, 3);
}
// テクスチャのバインド
void Graphics::bind_texture(std::uint32_t shader_register, std::uint32_t texture_id) {
    Texture* texture = get_texture(texture_id);
    if (texture == nullptr) return;
    s_graphics_device->set_texture(shader_register, texture);
}
// コンスタントバッファにデータを設定
void Graphics::set_constant_buffer(std::uint32_t shader_register, const void* data, size_t size) {
    s_graphics_device->set_constant_buffer(shader_register, data, size);
}

// GPUの待機
void Graphics::wait_for_gpu() {
    s_graphics_device->wait_for_gpu();
}
// グラフィックスデバイスの取得
GraphicsDevice* Graphics::get_graphics_device() {
    return s_graphics_device;
}