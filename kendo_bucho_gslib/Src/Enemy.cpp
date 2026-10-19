#include "Enemy.h"
#include "IWorld.h"
#include "Field.h"
#include "GTLib/Math/Line.h"
#include "Assets.h"

// 敵のモーションデータ
enum {
    MotionIdle = 0,  // アイドル
    MotionRun = 1,  // 走る
    MotionGuard = 10, // ガード中
    MotionDamage = 12, // ダメージ
    MotionDown = 14  // ダウン中
};

// コンストラクタ
Enemy::Enemy(IWorld* world, const Vector3& position) :
    mesh_{Mesh_Enemy,MotionIdle,true},
    motion_{ MotionIdle },
    motion_loop_{ true },
    state_{ State::Spawn }{
    world_ = world;
    name_ = "Enemy";
    tag_ = "EnemyTag";
    collider_ = BoundingSphere{ 0.5f, Vector3{0.0f, 0.8f, 0.0f} };
    transform_.position(position);
    mesh_.transform(transform_.local_to_world_matrix());
}

// 更新
void Enemy::update(float delta_time) {
    // 状態の更新
    update_state(delta_time);
    // 地面との衝突判定
    collide_field();
    // モーションを変更
    mesh_.change_motion(motion_, motion_loop_);
    mesh_.update(delta_time);
    // 行列を設定
    mesh_.transform(transform_.local_to_world_matrix());
    // 点滅処理（2フレームおきに表示）
    if ((int)blink_timer_ % 4 > 2) {
        float aa = 0.3f;
        color_.a = (color_.a <= aa) ? 1.0f :aa;
    }
    else {
        color_.a = 1.0f;
    }
}

// 描画
void Enemy::draw() const {
    //インスタンスにデータ渡す
    Graphics::add_instacne_data(Mesh_Enemy,transform_.local_to_world_matrix(),mesh_.skinned_matrix(),color_);
    // 衝突判定の描画
    collider().draw();
}

// 衝突処理
void Enemy::react(Actor& other) {
    // プレーヤタグのアクターと衝突した場合はダメージ中に遷移
    if (other.tag() == "PlayerTag") {
        // ダメージ効果音を再生
        Sound::play(Se_EnemeyDamage);
        // ダメージ中は衝突判定を無効にする
        enable_collider_ = false;
        // ダメージ中に遷移する
        change_state(State::Damage, MotionDamage, false);
        // 衝突判定用のコライダに衝突した場合はスコアを加算
        if (other.name() == "AttackCollider") {
            world_->add_score(1);
        }
    }
}

// 状態の更新
void Enemy::update_state(float delta_time) {
    switch (state_) {
    case State::Spawn:  spawn(delta_time);  break;
    case State::Move:   move(delta_time);   break;
    case State::Damage: damage(delta_time); break;
    case State::Down:   down(delta_time);   break;
    case State::Leave:  leave(delta_time);  break;
    }
    // 状態タイマの更新
    state_timer_ += delta_time;
}

// 状態の変更
void Enemy::change_state(State state, unsigned int motion, bool loop) {
    motion_ = motion;
    state_ = state;
    state_timer_ = 0.0f;
    motion_loop_ = loop;
}

// 出現中
void Enemy::spawn(float delta_time) {
    // 点滅タイマを更新
    blink_timer_ += delta_time;
    // プレーヤーを検索
    Actor* player = world_->find_actor("Player");
    // プレーヤーがいなければ何もしない
    if (player == nullptr) return;
    // プレーヤー方向を向く
    transform_.look_at(player->transform().position());
    // １秒間経過してから走り始める
    if (state_timer_ >= 60.0f) {
        blink_timer_ = 0.0f;
        change_state(State::Move, MotionRun);
    }
}

// 移動中
void Enemy::move(float delta_time) {
    // 前進する（ローカル座標基準） 
    transform_.translate(0.0f, 0.0f, 0.05f * delta_time);
    // 移動状態
    change_state(State::Move, MotionRun);
    // グラウンドの端まで来たら去る
    if (transform_.position().x > 21.1f ||
        transform_.position().x < -21.1f ||
        transform_.position().z >  21.1f ||
        transform_.position().z < -21.1f) {
        change_state(State::Leave, MotionGuard);
    }
}

// ダメージ中
void Enemy::damage(float delta_time) {
    // アニメーションの終了待ってダウン中に遷移
    if (state_timer_ >= mesh_.motion_end_time()) {
        change_state(State::Down, MotionDown, false);
    }
}

// ダウン中
void Enemy::down(float delta_time) {
    // 点滅タイマを更新
    blink_timer_ += delta_time;
    // 1秒間経過してから死亡する
    if (state_timer_ >= 60.0f) {
        die();
    }
}

// 退出中
void Enemy::leave(float delta_time) {
    // 点滅タイマの更新
    blink_timer_ += delta_time;
    // 1秒間点滅してから死亡する
    if (state_timer_ >= 60.0f) {
        die();
    }
}

// フィールドとの衝突判定
void Enemy::collide_field() {
    // 足元のオフセット
    const float FootOffset{ 0.2f };
    // 地面との衝突判定（線分との交差判定）
    Vector3 position = transform_.position();
    Line line;
    line.start = position + collider_.center;
    line.end = position + Vector3{ 0.0f, -FootOffset, 0.0f };
    Vector3 intersect;  // 地面との交点
    if (world_->field()->collide(line, &intersect)) {
        // 交差した点からy座標のみ補正する
        position.y = intersect.y;
        // 座標を変更する
        transform_.position(position);
    }
}