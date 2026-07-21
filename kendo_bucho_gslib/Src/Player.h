#ifndef PLAYER_H_
#define PLAYER_H_

#include "Actor.h"
#include "AnimatedMesh.h"

// プレーヤクラス
class Player : public Actor {
public:
    // 状態
    enum class State {
        Move,       // 移動中
        Attack,     // 攻撃中
        Damage,     // ダメージ中
        GetUp       // 起き上がり中
    };

public:
    // コンストラクタ
    Player(IWorld* world, const Vector3& position);
    // 更新
    virtual void update(float delta_time) override;
    // 描画
    virtual void draw() const override;
    // 衝突処理
    virtual void react(Actor& other) override;

private:
    // 状態の更新
    void update_state(float delta_time);
    // 状態の変更
    void change_state(State state, unsigned int motion, bool loop = true);
    // 移動中
    void move(float delta_time);
    // 攻撃中
    void attack(float delta_time);
    // ダメージ中
    void damage(float delta_time);
    // 起き上がり中
    void get_up(float delta_time);
    // フィールドとの衝突判定
    void collide_field();
    // 攻撃判定を生成する
    void generate_attack_collider();

private:
    // アニメーション付きメッシュ
    AnimatedMesh    mesh_;
    // モーション番号
    unsigned int          motion_;
    // モーションのループフラグ
    bool            motion_loop_;
    // 状態
    State           state_;
    // 状態タイマ
    float           state_timer_{ 0.0f };
};

#endif