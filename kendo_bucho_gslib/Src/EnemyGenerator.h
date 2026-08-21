#ifndef ENEMY_GENERATOR_H_
#define ENEMY_GENERATOR_H_

#include "Actor.h"
#include "InstanceTest/InstanceAnimatedMesh.h"

//敵生成クラス
class EnemyGenerator : public Actor {
public:
    //コンストラクタ
    EnemyGenerator(IWorld* world, unsigned int id, unsigned int instance_count_max, unsigned int motion = 0, bool loop = true, unsigned int num_bones = 256);
    //更新
    virtual void update(float delta_time) override;
    //一括描画
    virtual void draw()const override;
    // メッセージ処理
    virtual void handle_message(const std::string& message, void* param)override;
private:
    //敵の生成
    void generate();
private:
    // 出現タイマー
    float timer_{ 0.0f };
    InstanceAnimatedMesh mesh_;
};

#endif

