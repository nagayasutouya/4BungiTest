#include "CameraTPS.h"
#include "IWorld.h"
#include "Field.h"
#include "Line.h"

// プレーヤーからの相対座標
const Vector3 PlayerOffset{ 0.0f, 2.0f, -4.0f };
// カメラの注視点の補正値
const Vector3 ReferencePointOffset{ 0.0f, 1.0f, 0.0f };

// コンストラクタ
CameraTPS::CameraTPS(IWorld* world, const Vector3& position, const Vector3& at) {
    // ワールドの設定
    world_ = world;
    // タグの設定
    tag_ = "CameraTag";
    // 名前の設定
    name_ = "Camera";
    // 視点の位置を設定
    transform_.position(position);
    // 注視点を設定
    transform_.look_at(at);
}

// 更新
void CameraTPS::update(float delta_time) {
    // プレーヤを検索する
    Actor* player = world_->find_actor("Player");
    if (player == nullptr) return;
    // 視点の位置を求める（プレーヤーの背後）
    Vector3 position = PlayerOffset * player->transform().local_to_world_matrix();
    // 注視点の位置を求める（プレーヤーの頭部の少し上あたりの座標）
    Vector3 at = player->transform().position() + ReferencePointOffset;

    // フィールドとの衝突判定
    Line line{ at, position };
    Vector3 intersect;
    if (world_->field()->collide(line, &intersect)) {
        position = intersect;
    }

    // 視点の位置を設定
    transform_.position(position);
    // 注視点を設定(注視点の方向に向きを変える）
    transform_.look_at(at);
    Graphics::effect_data()->set_effect_camera(position,at);
}

// 描画
void CameraTPS::draw() const {
    Matrix4x4 proj = Matrix4x4::perspective(45.0f, 1024.0f / 768.0f, 0.1f, 1000.0f);
    Graphics::set_projection_matrix(proj);
    // 視点の位置
    Vector3 eye = transform_.position();
    // 注視点の位置
    Vector3 at = eye + transform_.forward();
    // カメラの設定
    Matrix4x4 view=Matrix4x4::look_at(
        Vector3{ eye.x, eye.y, eye.z },    // 視点の位置
        Vector3{ at.x,  at.y,  at.z },     // 注視点の位置
        Vector3{ 0.0f,  1.0f,  0.0f }      // カメラの上方向
    );
    Graphics::set_view_matrix(view);
}
