#ifndef ENEMY_H_
#define ENEMY_H_

#include "Actor.h"
#include "AnimatedMesh.h"

// 敵クラス
class Enemy : public Actor {
public:
    //敵の状態
    enum class State {
        Spawn,      // 出現中
        Move,       // 移動中
        Damage,     // ダメージ中
        Down,       // ダウン中
        Leave       // 消滅中
    };
    // コンストラクタ
    Enemy(IWorld* world, const Vector3& position);
    // 更新
    virtual void update(float delta_time) override;
    //  描画
    virtual void draw() const override;
    // 衝突処理
    virtual void react(Actor& other) override;

private:
    // 状態の更新
    void update_state(float delta_time);
    // 状態の変更
    void change_state(State state, unsigned int motion, bool loop = true);
    // 出現
    void spawn(float delta_time);
    // 移動
    void move(float delta_time);
    // ダメージ
    void damage(float delta_time);
    // ダウン
    void down(float delta_time);
    // 退出
    void leave(float delta_time);
    // フィールドとの衝突判定
    void collide_field();

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
    // 点滅タイマ
    float           blink_timer_{ 0.0f };
};

#endif