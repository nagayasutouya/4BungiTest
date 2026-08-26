#ifndef BENEMY_GENERATOR_H_
#define BENEMY_GENERATOR_H_

#include "Actor.h"

//敵生成クラス
class BEnemyGenerator : public Actor {
public:
    //コンストラクタ
    BEnemyGenerator(IWorld* world);
    //更新
    virtual void update(float delta_time) override;

private:
    //敵の生成
    void generate();

private:
    // 出現タイマー
    float timer_{ 0.0f };
};

#endif

