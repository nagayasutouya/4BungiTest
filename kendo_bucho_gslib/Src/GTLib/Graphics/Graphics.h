#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "GraphicsParts/GraphicsDevice.h"
#include "GTLib/Math/Vector2.h"
#include "GTLib/Math/Vector3.h"
#include "GTLib/Math/Matrix4x4.h"
#include "GTLib/Math/Color.h"
#include "GTLib/Math/Rect.h"
#include "GraphicsParts/Mesh/Mesh.h"
#include "GraphicsParts/Mesh/SkinnedMesh.h"
#include "GraphicsParts/Mesh/Instance/InstanceMesh.h"
#include "GraphicsParts/Mesh/Instance/InstanceSkinnedMesh.h"
#include <string>
#include <cstdint>

// グラフィックス関連
namespace Graphics {
    // ライト設定用構造体
    struct Light {
        Color ambient;      // 環境光
        Color diffuse;      // 拡散反射光
        Color specular;     // 鏡面反射光
        Vector3 position;   // 位置
    };
    // グラフィックス関連の初期化
    void initialize(GraphicsDevice* device);
    // グラフィックス関連の終了処理
    void finalize();

    // 描画の開始
    void begin_scene(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
    // 描画の終了
    void end_scene();

    // カラーバッファのクリア
    void clear_color_buffer(float r, float g, float b, float a);
    // デプスバッファのクリア
    void clear_depth_buffer();
    // ビューポート設定
    void set_viewport(float x, float y, float w, float h);

    // ルートシグネチャ設定
    void set_root_signature();
    // ルートシグニチャ取得
    RootSignature* get_root_signature();
    // ディスクリプタヒープ設定
    void set_descriptor_heaps();

    // パイプラインステート登録
    void add_pipeline_state(std::uint32_t pipeline_state_id, PipelineState* pipeline_state);
    // パイプラインステート削除
    void delete_pipeline_state(std::uint32_t pipeline_state_id);
    // パイプラインステート設定
    void set_pipeline_state(std::uint32_t pipeline_state_id);

    // テクスチャ読み込み
    Texture* load_texture(std::uint32_t texture_id, const std::string& filename);
    // テクスチャ削除
    void delete_texture(std::uint32_t texture_id);
    // テクスチャ取得
    Texture* get_texture(std::uint32_t texture_id);

    // ワールド行列の設定
    void set_world_matrix(const Matrix4x4& world);
    // ビュー行列の設定
    void set_view_matrix(const Matrix4x4& view);
    // プロジェクション行列の設定
    void set_projection_matrix(const Matrix4x4& projection);
    // ライトの設定
    void set_light(const Light& light);

    // スプライトのパイプラインステートを設定
    void set_sprite2d_pipeline_state(std::uint32_t pipeline_state_id);
    // スプライトのパイプラインステートを取得
    std::uint32_t get_sprite2d_pipeline_state();
    // スプライト描画
    void draw_sprite2d(
        std::uint32_t texture_id,                           // テクスチャID
        const Vector2& position,                            // 座標
        const Rect* source_rect = nullptr,                  // テクスチャの切り出し範囲
        const Vector2* center = nullptr,                    // 回転の中心座標
        const Color* color = nullptr,                       // 色
        const Vector2* scale = nullptr,                     // 拡大率
        float angle = 0.0f                                  // 回転角度
    );

    // 3Dスプライトのパイプラインステート設定
    void set_sprite3d_pipeline_state(std::uint32_t pipeline_state_id);
    // 3Dスプライトのパイプラインステート取得
    std::uint32_t get_sprite3d_pipeline_state();
    // 3Dスプライト(ビルボード)描画
    void draw_sprite3d(
        std::uint32_t texture_id,                           // テクスチャID
        const Vector3& position,                            // 平行移動量
        const Rect& sprite_rect,                            // スプライトの矩形
        const Rect* source_rect = nullptr,                  // テクスチャ内の位置
        const Color* color = nullptr,                       // カラー
        const Vector2* scale = nullptr,                     // スケール値
        float angle = 0.0f                                  // 回転角度
    );

    //メッシュ
    // メッシュ読み込み
    Mesh* load_mesh(std::uint32_t mesh_id, const std::string& filename);
    // メッシュ削除
    void delete_mesh(std::uint32_t mesh_id);
    // メッシュ取得
    Mesh* get_mesh(std::uint32_t mesh_id);
    // メッシュのパイプラインステート設定
    void set_mesh_pipeline_state(std::uint32_t pipeline_state_id);
    // メッシュのパイプラインステート取得
    std::uint32_t get_mesh_pipeline_state();
    // メッシュ描画
    void draw_mesh(std::uint32_t mesh_id);

    //スキニングメッシュ
    // スキニングメッシュ読み込み
    SkinnedMesh* load_skinned_mesh(std::uint32_t skinned_mesh_id,
        const std::string& mesh_file,
        const std::string& skeleton_file,
        const std::string& animation_file);
    // スキニングメッシュ削除
    void delete_skinned_mesh(std::uint32_t skinned_mesh_id);
    // スキニングメッシュ取得
    SkinnedMesh* get_skinned_mesh(std::uint32_t skinned_mesh_id);

    // スキニングメッシュのパイプラインステートを設定
    void set_skinned_mesh_pipeline_state(std::uint32_t pipeline_state_id);
    // スキニングメッシュのパイプラインステートを取得
    std::uint32_t get_skinned_mesh_pipeline_state();
    // ローカル行列計算
    void compute_local_matrices(std::uint32_t skinned_mesh_id,
        int anim_id, float frame_no, Matrix4x4 local[]);
    // ブレンドされたローカル行列計算
    void compute_blended_matrices(std::uint32_t skinned_mesh_id,
        int anim_id1, float frame1,
        int anim_id2, float frame2,
        float weight, Matrix4x4 local[]);
    // ワールド行列計算
    void compute_world_matrices(std::uint32_t skinned_mesh_id,
        const Matrix4x4 local[], Matrix4x4 world[],
        const Matrix4x4& parent = Matrix4x4::identity());
    // スキニング行列計算
    void compute_skinned_matrices(std::uint32_t skinned_mesh_id,
        const Matrix4x4 world[], Matrix4x4 skinned[]);
    // スキニング行列を設定する
    void set_skinned_matrices(std::uint32_t skinned_mesh_id, const Matrix4x4 skinned[]);
    // スキニングメッシュを描画する (事前にスキニング行列のセットが必要)
    void draw_skinned_mesh(std::uint32_t skinned_mesh_id);
    // 指定したスキンメッシュ・アニメーションの終了フレーム（時間）を返す
    float get_skinned_mesh_last_frame(std::uint32_t skinned_mesh_id, int anim_id);
    // 指定したスキンメッシュのボーン数を返す
    std::size_t get_skinned_mesh_bone_count(std::uint32_t skinned_mesh_id);

    //インスタンスメッシュ
    // メッシュ読み込み
    InstanceMesh* load_instance_mesh(std::uint32_t mesh_id, const std::string& filename);
    // メッシュ削除
    void delete_instance_mesh(std::uint32_t mesh_id);
    // メッシュ取得
    InstanceMesh* get_instance_mesh(std::uint32_t mesh_id);
    // メッシュのパイプラインステート設定
    void set_instance_mesh_pipeline_state(std::uint32_t pipeline_state_id);
    // メッシュのパイプラインステート取得
    std::uint32_t get_instance_mesh_pipeline_state();
    // メッシュ描画
    void draw_instance_mesh(std::uint32_t mesh_id, unsigned int instance_count);

    //インスタンススキニングメッシュ
    // インスタンススキニングメッシュ読み込み
    InstanceSkinnedMesh* load_instance_skinned_mesh(std::uint32_t skinned_mesh_id,const std::string& mesh_file,const std::string& skeleton_file,const std::string& animation_file, int instance_count=100);
    // スキニングメッシュ削除
    void delete_instance_skinned_mesh(std::uint32_t skinned_mesh_id);
    // スキニングメッシュ取得
    InstanceSkinnedMesh* get_instance_skinned_mesh(std::uint32_t skinned_mesh_id);
    // スキニングメッシュのパイプラインステートを設定
    void set_instance_skinned_mesh_pipeline_state(std::uint32_t pipeline_state_id);
    // スキニングメッシュのパイプラインステートを取得
    std::uint32_t get_instance_skinned_mesh_pipeline_state();
    // ローカル行列計算
    void compute_local_matrices_instance(std::uint32_t skinned_mesh_id, int instance_id, int anim_id, float frame_no, Matrix4x4 local[]);
    // ブレンドされたローカル行列計算
    void compute_blended_matrices_instance(std::uint32_t skinned_mesh_id, int instance_id, BlendedMat& mat, const float lerp_time);
    // ワールド行列計算
    void compute_world_matrices_instance(std::uint32_t skinned_mesh_id, int instance_id, const Matrix4x4 local[], Matrix4x4 world[], const Matrix4x4& parent);
    // スキニング行列計算
    void compute_skinned_matrices_instance(std::uint32_t skinned_mesh_id, int instance_id, const Matrix4x4 world[], Matrix4x4 skinned[]);
    // スキニング行列を設定する
    void set_instance_skinned_matrices(std::uint32_t instance_skinned_mesh_id, unsigned int instance_count, const SkinnedMat instance_skinned[]);
    // スキニングメッシュを描画する (事前にスキニング行列のセットが必要)
    void draw_instance_skinned_mesh(std::uint32_t instance_skinned_mesh_id, unsigned int instance_count,const InstanceObject* first_obj);
    // 指定したスキンメッシュ・アニメーションの終了フレーム（時間）を返す
    float get_instance_skinned_mesh_last_frame(std::uint32_t instance_skinned_mesh_id, int anim_id);
    // 指定したスキンメッシュのボーン数を返す
    std::size_t get_instance_skinned_mesh_bone_count(std::uint32_t instance_skinned_mesh_id);

    // GPUの待機
    void wait_for_gpu();

    // グラフィックスデバイスの取得
    GraphicsDevice* get_graphics_device();

    // 3D行列GPU転送
    void apply_object_params();
    // カメラパラメータGPU転送
    void apply_camera_params();
    // ライトパラメータGPU転送
    void apply_light_params();
}

#endif

