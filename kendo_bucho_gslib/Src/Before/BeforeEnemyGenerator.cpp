#include "BeforeEnemyGenerator.h"
#include "IWorld.h"
#include "BeforeEnemy.h"

//最大出現数
const int BEnemyMaxCount{ 20 };
// 出現間隔
const float GenerateInterval{ 5.0f };

// コンストラクタ
BEnemyGenerator::BEnemyGenerator(IWorld* world) {
    world_ = world;
    tag_ = "BEnemyGeneratorTag";
    name_ = "BEnemyGenerator";
}

// 更新
void BEnemyGenerator::update(float delta_time) {
    // 敵の数を取得
    int enemy_count = world_->count_actor_with_tag("EnemyTag");
    // 敵の最大よりも小さければ？
    if (enemy_count < BEnemyMaxCount) {
        timer_ += delta_time;
        // 生成時間を超えたか？
        if (GenerateInterval < timer_) {
            // 敵の生成
            generate();
            timer_ = 0.0f;
        }
    }
}

// 敵の生成
void BEnemyGenerator::generate() {
    // グラウンドの端からランダムに出現
    Vector3 position{ 0.0f,0.0f,0.0f };
    switch ((0, 3)) {
    case 0:
        position.x = Random::range(-21.0f, 21.0f);
        position.z = -21.0f;
        break;
    case 1:
        position.x = 21.0f;
        position.z = Random::range(-21.0f, 21.0f);
        break;
    case 2:
        position.x = Random::range(-21.0f, 21.0f);
        position.z = 21.0f;
        break;
    case 3:
        position.x = -21.0f;
        position.z = Random::range(-21.0f, 21.0f);
        break;
    }
    world_->add_actor(new BEnemy(world_, position));
}
