#include "EnemyGenerator.h"
#include "IWorld.h"
#include "Enemy.h"
#include "Assets.h"

//最大出現数
const int EnemyMaxCount{ 2000 };
// 出現間隔
const float GenerateInterval{ 1.0f };
const int OneTimeAdd{ 100 };

// コンストラクタ
EnemyGenerator::EnemyGenerator(IWorld* world ) {
    world_ = world;
    tag_   = "EnemyGeneratorTag";
    name_  = "EnemyGenerator";
}

// 更新
void EnemyGenerator::update(float delta_time) {
    // 敵の数を取得
    int enemy_count = world_->count_actor_with_tag("EnemyTag");
    // 敵の最大よりも小さければ？
    if (enemy_count < EnemyMaxCount) {
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
void EnemyGenerator::generate() {
    for (int i = 0; i < OneTimeAdd; i++) {
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
        world_->add_actor(new Enemy(world_, position));
    }
}

//一括描画
void  EnemyGenerator::draw_instance()const {
    Graphics::draw_instance_skinned_mesh(Mesh_Enemy);
}
// メッセージ処理
void  EnemyGenerator::handle_message(const std::string& message, void* param){
}