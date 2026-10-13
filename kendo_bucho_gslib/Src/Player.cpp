#include "Player.h"
#include "IWorld.h"
#include "Field.h"
#include "Line.h"
#include "Assets.h"
#include "AttackCollider.h"

enum {                  // プレーヤーモーションデータ
    MotionIdle = 0,     // アイドル
    MotionRun = 1,      // 走り
    MotionAttack = 17,  // 攻撃
    MotionDamage = 14,  // ダメージ
    MotionGetUp = 16    // 起き上がり
};

// コンストラクタ
Player::Player(IWorld* world, const Vector3& position) :
    mesh_{ Mesh_Player, MotionIdle, true },
    motion_{ MotionIdle },
    motion_loop_{ true },
    state_{ State::Move } {
    world_ = world;
    name_ = "Player";
    tag_ = "PlayerTag";
    collider_ = BoundingSphere{ 0.5f, Vector3{0.0f, 0.8f, 0.0f} };
    transform_.position(position);
    mesh_.transform(transform_.local_to_world_matrix());
}

// 更新
void Player::update(float delta_time) {
    // 状態の更新
    update_state(delta_time);
    // 地面との衝突判定 
    collide_field();
    // モーションの変更
    mesh_.change_motion(motion_, motion_loop_);
    // メッシュの更新
    mesh_.update(delta_time);
    // 変換行列の設定
    mesh_.transform(transform_.local_to_world_matrix());
}

// 描画
void Player::draw() const {
    // メッシュの描画
    mesh_.draw();
    // 衝突判定のデバッグ表示
    collider().draw();
}

// 衝突処理
void Player::react(Actor& other) {
    // ダメージ中はリアクションしない
    if (state_ == State::Damage) return;
    // 敵と衝突した場合はダメージ中に遷移
    if (other.tag() == "EnemyTag") {
        // ダメージ効果音を再生
        Sound::play(Se_PlayerDamage);
        // ダメージ中は衝突判定を無効にする
        enable_collider_ = false;
        // ダメージ中に遷移する
        change_state(State::Damage, MotionDamage, false);
    }
}

// 状態の更新
void Player::update_state(float delta_time) {
    // 状態による分岐
    switch (state_) {
    case State::Move:   move(delta_time);   break;
    case State::Attack: attack(delta_time); break;
    case State::Damage: damage(delta_time); break;
    case State::GetUp:  get_up(delta_time); break;
    }
    // 状態タイマの更新
    state_timer_ += delta_time;
}

// 状態の変更
void Player::change_state(State state, unsigned int motion, bool loop) {
    motion_ = motion;
    motion_loop_ = loop;
    state_ = state;
    state_timer_ = 0.0f;
}

// 移動中
void Player::move(float delta_time) {
    // スペースキーで攻撃
    if (Input::get_key_down('V')) {
        // 攻撃効果音を再生
        Sound::play(Se_PlayerAttack);
        // 自分の前方に衝突判定を出現させる
        generate_attack_collider();
        // 攻撃中に遷移
        change_state(State::Attack, MotionAttack, false);
        return;
    }
    // 何もしなければアイドル状態
    unsigned int motion = MotionIdle;
    // 左右キーでｙ軸周りに回転
    float yaw{ 0.0f };
    if (Input::get_key('A'))  yaw = 3.0f;
    if (Input::get_key('D')) yaw = -3.0f;
    transform_.rotate(0.0f, yaw * delta_time, 0.0f);
    // 上キーで前進
    float speed{ 0.0f };
    if (Input::get_key('W')) {
        speed = 0.1f;
        motion = MotionRun;
    }
    float y_speed{ 0.0f };
    if (Input::get_key('Z')) {
        y_speed = 0.1f;
    }
    else if (Input::get_key('X')) {
        y_speed = -0.1f;
    }
    change_state(State::Move, motion);
    // 前進する(ローカル座標基準）
    transform_.translate(0.0f, y_speed*delta_time, speed * delta_time);
    // グラウンド内にクランプ
    Vector3 position = transform_.position();
    position.x = Mathf::CLAMP(position.x, -20.0f, 20.0f);
    position.z = Mathf::CLAMP(position.z, -20.0f, 20.0f);
    transform_.position(position);
}

// 攻撃中
void Player::attack(float delta_time) {
    // 攻撃のモーションが終了したら移動中に遷移
    if (state_timer_ >= mesh_.motion_end_time()) {
        // 攻撃モーションが終了したら移動中に遷移
        change_state(State::Move, MotionIdle);
    }
}

// ダメージ中
void Player::damage(float delta_time) {
    if (state_timer_ >= mesh_.motion_end_time()) {
        // ダメージ中が終了したら起き上がる
        change_state(State::GetUp, MotionGetUp, false);
    }
}

// 起き上がり中
void Player::get_up(float delta_time) {
    // 起き上がりモーションが終了したらアイドル中に遷移
    if (state_timer_ >= mesh_.motion_end_time()) {
        // 衝突判定を有効にする
        enable_collider_ = true;
        // アイドル状態に遷移
        change_state(State::Move, MotionIdle);
    }
}

// フィールドとの衝突判定
void Player::collide_field() {
    // 足元のオフセット
    const float FootOffset{ 0.1f };
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

// 攻撃判定を生成する
void Player::generate_attack_collider() {
    // 攻撃判定を出現させる場所の距離
    const float AttackColliderDistance{ 1.5f };
    // 攻撃判定の半径
    const float AttackColliderRadius{ 1.0f };
    // 攻撃判定を出す場所の高さ
    const float AttackColliderHeight{ 1.0f };

    // 攻撃判定が有効になるまでの遅延時間
    const float AttackCollideDelay{ 0.0f };
    // 攻撃判定の寿命
    const float AttackCollideLifeSpan{ 30.0f };

    // 衝突判定を出現させる座標を求める（前方の位置）
    Vector3 position = transform_.position() + transform_.forward() * AttackColliderDistance;
    // 高さの補正（足元からの高さ）
    position.y += AttackColliderHeight;
    // 衝突判定用の球を作成
    BoundingSphere collider{ AttackColliderRadius, position };
    // 衝突判定を出現させる
    world_->add_actor(new AttackCollider{ world_, collider,
        "PlayerTag", "AttackCollider", tag_, AttackCollideLifeSpan, AttackCollideDelay });
}
