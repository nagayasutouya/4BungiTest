#ifndef ENEMY_GENERATOR_H_
#define ENEMY_GENERATOR_H_

#include "Actor.h"

//敵生成クラス
class EnemyGenerator : public Actor {
public:
    //コンストラクタ
    EnemyGenerator(IWorld* world);
    //更新
    virtual void update(float delta_time) override;
    //一括描画
    virtual void draw_instance()const override;
    // メッセージ処理
    virtual void handle_message(const std::string& message, void* param)override;
private:
    //敵の生成
    void generate();
private:
    // 出現タイマー
    float timer_{ 0.0f };
};

#endif

