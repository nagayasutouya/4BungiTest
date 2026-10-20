#include "GTLib/GTLib.h"
#include "SceneManager.h"
#include "GamePlayScene.h"
#include "TitleScene.h"

#include "Sibungi/FPSCheck.h"

class MyGame : public Game {
public:
    // コンストラクタ
    MyGame() : Game{ 1920, 1080 } {}
    // 開始
    void start() override {
        // グラフィックスシステムの初期化
        Graphics::initialize(graphics_device_);
        // パイプラインステートの基本設定
        PipelineSetup::setup_defaults();
        Graphics::initialize_effect();
        scene_manager_.add("TitleScene", new TitleScene());
        scene_manager_.add("GamePlayScene", new GamePlayScene());
        scene_manager_.change("TitleScene");
    }
    //　更新
    void update(float delta_time) override {
        Graphics::effect_data()->send_new_frame();
        fps_.Instance()->update();
        if (Input::get_key_down('P'))stop_ = (stop_) ? false : true;
        if (stop_)return;
        // ワールドクラスの更新
        float delta_frame = delta_time * 60.0f;
        scene_manager_.update(delta_frame);
    }
    // 描画
    void draw() override {
        if (stop_)return;
        // シーン開始
        Graphics::begin_scene(0.5f, 0.5f, 1.0f, 1.0f);
        scene_manager_.draw();
        // シーン終了
        Graphics::end_scene();
    }
    // 終了
    void end() override {
        scene_manager_.end();
    }

private:
    // シーンマネージャー
    SceneManager scene_manager_;
    bool stop_{false};
    //FPS
    FPSCheck fps_;
};

int main() {
    return MyGame().run();
}



// メッシュ衝突判定テスト
//class MyGame : public Game {
//    // 球体の位置
//    Vector3 sphere_pos_{ 0.0f, 1.0f, 0.0f };
//    // 球体の半径
//    float sphere_radius_{ 0.5f };
//    MeshCollider mesh_collider_;
//
//    // 開始
//    void start() override {
//        // パイプライン状態の基本設定
//        PipelineSetup::setup_defaults();
//
//        // メッシュコライダーファイルの読み込み
//        mesh_collider_.load("Assets/test/Terrain.col");
//
//        // 地面用のメッシュの読み込み
//        Graphics::load_mesh(0, "Assets/test/Terrain.mesh");
//        // 球体メッシュの読み込み
//        Graphics::load_mesh(1, "Assets/test/Sphere.mesh");
//
//        // ライトの設定
//        Graphics::Light light;
//        light.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
//        light.diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
//        light.specular = { 1.0f, 1.0f, 1.0f, 1.0f };
//        light.position = { 100.0f, 100.0f, 100.0f };
//        Graphics::set_light(light);
//    }
//    // 更新
//    void update(float delta_time) override {
//        // カーソルキー入力による XZ 平面上の移動
//        Vector3 move_dir{ 0.0f, 0.0f, 0.0f };
//        if (Input::get_key(VK_UP))    move_dir.z -= 1.0f;
//        if (Input::get_key(VK_DOWN))  move_dir.z += 1.0f;
//        if (Input::get_key(VK_LEFT))  move_dir.x -= 1.0f;
//        if (Input::get_key(VK_RIGHT)) move_dir.x += 1.0f;
//        sphere_pos_ += move_dir.normalized() * 8.0f * delta_time;
//
//        // 壁との判定（球による判定と押し出し)
//        // 地面との衝突を避けるため、球体の少し上の部分と判定する
//        Sphere sphere{ sphere_pos_ + Vector3{ 0.0f, 0.5f, 0.0f }, sphere_radius_ };
//        SphereHit sphere_hit;
//        if (mesh_collider_.intersect_sphere(sphere, &sphere_hit)) {
//            // 衝突時は押し出しベクトルを適用
//            sphere_hit.push_out.y = 0.0f;
//            sphere_pos_ += sphere_hit.push_out;
//        }
//
//        // 地面との判定 (線分によるレイキャスト)
//        // 空中に浮かないように、地面よりも下の部分と判定する
//        Line ground_ray;
//        ground_ray.start = sphere_pos_;
//        ground_ray.end = sphere_pos_ - Vector3{ 0.0f, 1.0f, 0.0f };
//        LineHit ground_hit;
//        if (mesh_collider_.intersect_line_segment(ground_ray, &ground_hit)) {
//            // 地面の高さに球の底面を合わせる (球中心Y = hit.point.y + radius)
//            sphere_pos_.y = ground_hit.point.y + sphere_radius_;
//        }
//    }
//    // 描画
//    void draw() override {
//        // 描画開始
//        Graphics::begin_scene();
//
//        // 射影行列の設定
//        Matrix4x4 proj = Matrix4x4::perspective(45.0f, 640.0f / 480.0f, 0.1f, 1000.0f);
//        Graphics::set_projection_matrix(proj);
//
//        // ビュー行列の設定 (球体を斜め上から追従)
//        Vector3 camera_pos = sphere_pos_ + Vector3(0.0f, 10.0f, 15.0f);
//        Matrix4x4 view = Matrix4x4::look_at(
//            camera_pos,
//            sphere_pos_,
//            Vector3{ 0.0f, 1.0f, 0.0f }
//        );
//        Graphics::set_view_matrix(view);
//
//        // 地面用メッシュの描画
//        Graphics::set_world_matrix(Matrix4x4::identity());
//        Graphics::draw_mesh(0);
//
//        // 球体メッシュの描画
//        Matrix4x4 sphere_world = Matrix4x4::translate(sphere_pos_);
//        Graphics::set_world_matrix(sphere_world);
//        Graphics::draw_mesh(1);
//
//        // 描画終了
//        Graphics::end_scene();
//    }
//    // 終了
//    void end() override {
//        // GPUの終了待ち
//        Graphics::wait_for_gpu();
//        // メッシュの解放
//        Graphics::delete_mesh(0);
//        Graphics::delete_mesh(1);
//        // メッシュコライダ―の消去
//        mesh_collider_.clear();
//    }
//};

//#include "GTLib/GTLib.h"
//#include "GTLib/GTLibTextrueID.h"
//#include "GTLib/GTLibCollision.h"

//// メイン関数
//int main() {
//    return MyGame().run();
//}

//static const Vector2 screen_size_{ 640.0f,480.0f };
//// メッシュ衝突判定テスト
//class MyGame : public Game {
//private:
//    // 球体の位置
//    Vector3 sphere_pos_{ 0.0f, 1.0f, 0.0f };
//    // 球体の半径
//    float sphere_radius_{ 0.5f };
//public:
//
//    MyGame():Game{(int)screen_size_.x,(int)screen_size_.y,"TestQuadTree",true} {}
//
//    // 開始
//    void start() override {
//        // 衝突判定システムの初期化
//        CollisionPhysics::initialize();
//
//        // パイプライン状態の基本設定
//        PipelineSetup::setup_defaults();
//
//        //エフェクシアの初期化
//        Graphics::initialize_effect();
//
//        // レンダーターゲットの作成(ゲーム側で想定する解像度に設定する）
//        Graphics::create_render_texture(GTLibTextureID::Instance()->render_target_id_min(), (UINT)screen_size_.x, (UINT)screen_size_.y);
//        Graphics::create_depth_texture(GTLibTextureID::Instance()->depth_id_min(), (UINT)screen_size_.x, (UINT)screen_size_.y);
//
//        // メッシュコライダーファイルの読み込み
//        CollisionPhysics::load_mesh_collider(0, "Assets/test/Terrain.col");
//
//        // ステージのメッシュの読み込み
//        Graphics::load_mesh(0, "Assets/test/Terrain.mesh");
//        // 球体メッシュの読み込み
//        Graphics::load_mesh(1, "Assets/test/Sphere.mesh");
//
//        // ライトの設定
//        Graphics::Light light;
//        light.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
//        light.diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
//        light.specular = { 1.0f, 1.0f, 1.0f, 1.0f };
//        light.position = { 100.0f, 100.0f, 100.0f };
//        Graphics::set_light(light);
//    }
//
//    // 更新
//    void update(float delta_time) override {
//        // カーソルキー入力による XZ 平面上の移動
//        Vector3 move_dir{ 0.0f, 0.0f, 0.0f };
//        if (Input::get_key(VK_UP))    move_dir.z -= 1.0f;
//        if (Input::get_key(VK_DOWN))  move_dir.z += 1.0f;
//        if (Input::get_key(VK_LEFT))  move_dir.x -= 1.0f;
//        if (Input::get_key(VK_RIGHT)) move_dir.x += 1.0f;
//        sphere_pos_ += move_dir.normalized() * 8.0f * delta_time;
//
//        // 壁との判定（球による判定と押し出し)
//        // 地面との衝突を避けるため、球体の少し上の部分と判定する
//        Sphere sphere{ sphere_pos_ + Vector3{ 0.0f, 0.5f, 0.0f }, sphere_radius_ };
//        SphereHit sphere_hit;
//        if (CollisionPhysics::intersect_mesh_sphere(0, sphere, &sphere_hit)) {
//            // 衝突時は押し出しベクトルを適用
//            sphere_hit.push_out.y = 0.0f;
//            sphere_pos_ += sphere_hit.push_out;
//        }
//
//        // 地面との判定 (線分によるレイキャスト)
//        // 空中に浮かないように、地面よりも下の部分と判定する
//        Line ground_ray;
//        ground_ray.start = sphere_pos_;
//        ground_ray.end = sphere_pos_ - Vector3{ 0.0f, 1.0f, 0.0f };
//        LineHit ground_hit;
//        if (CollisionPhysics::intersect_mesh_line_segment(0, ground_ray, &ground_hit)) {
//            // 地面の高さに球の底面を合わせる (球中心Y = hit.point.y + radius)
//            sphere_pos_.y = ground_hit.point.y + sphere_radius_;
//        }
//    }
//    // 描画
//    void draw() override {
//        // 描画開始
//        Graphics::begin_scene();
//        // レンダーターゲットに描画開始
//        Graphics::begin_render_target(GTLibTextureID::Instance()->render_target_id_min(), GTLibTextureID::Instance()->depth_id_min());
//        //レンダーターゲットをクリア
//        Graphics::clear_color_buffer(0.0f,0.0f,0.0f,1.0f);
//        //デプスバッファをクリア
//        Graphics::clear_depth_buffer();
//        // 射影行列の設定
//        Matrix4x4 proj = Matrix4x4::perspective(45.0f, screen_size_.x / screen_size_.y, 0.1f, 1000.0f);
//        Graphics::set_projection_matrix(proj);
//
//        // ビュー行列の設定 (球体を斜め上から追従)
//        Vector3 camera_pos = sphere_pos_ + Vector3(0.0f, 10.0f, 15.0f);
//        Matrix4x4 view = Matrix4x4::look_at(
//            camera_pos,
//            sphere_pos_,
//            Vector3{ 0.0f, 1.0f, 0.0f }
//        );
//        Graphics::set_view_matrix(view);
//
//        // ステージ用メッシュの描画
//        Graphics::set_world_matrix(Matrix4x4::identity());
//        Graphics::draw_mesh(0);
//
//        // 球体メッシュの描画
//        Matrix4x4 sphere_world = Matrix4x4::translate(sphere_pos_);
//        Graphics::set_world_matrix(sphere_world);
//        Graphics::draw_mesh(1);
//        // レンダーターゲットの描画終了
//        Graphics::end_render_target();
//
//        // レンダーターゲットをバックバッファに転送
//        Graphics::blit_to_back_buffer(0, PipelineStateID::Blit);
//
//        // 描画終了
//        Graphics::end_scene();
//    }
//
//    void end() override {
//        // GPUの終了待ち
//        Graphics::wait_for_gpu();
//        // メッシュの削除
//        Graphics::delete_mesh(0);
//        Graphics::delete_mesh(1);
//        // メッシュコライダ―の削除
//        CollisionPhysics::delete_mesh_collider(0);
//        // 衝突判定システムの終了処理
//        CollisionPhysics::finalize();
//    }
//};

// メイン関数
//int main() {
//    return MyGame().run();
//}