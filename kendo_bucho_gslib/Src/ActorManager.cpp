#include "ActorManager.h"
#include "Actor.h"

// デストラクタ
ActorManager::~ActorManager() {
    clear();
}

// アクターの追加
void ActorManager::add(Actor* actor) {
    actor->add_space();
    actors_.push_back(actor);
}

// アクターの更新
void ActorManager::update(float delta_time) {
    for (auto actor : actors_) {
        actor->update(delta_time);
    }
}

// アクターの遅延更新
void ActorManager::late_update(float delta_time) {
    for (auto actor : actors_) {
        quadtree_.change_space(actor->space_check_size(), actor->space());
    }
}

// アクターの描画
void ActorManager::draw() const {
    for (auto actor : actors_) {
        actor->draw();
    }
}

// 半透明アクターの描画
void ActorManager::draw_transparent() const {
    for (auto actor : actors_) {
        actor->draw_transparent();
    }
}

// アクターのGUI描画
void ActorManager::draw_gui() const {
    for (auto actor : actors_) {
        actor->draw_gui();
    }
}

// アクターの衝突判定
void ActorManager::collide() {
    GTUINT size=quadtree_.creat_collide_list(actors_);
    //リストをもとにチェック
    for (GTUINT i = 0; i < size; i += 2) {
        quadtree_.collide_list()[i]->collide(*quadtree_.collide_list()[i + 1]);
    }
}

// 死亡しているアクターの削除
void ActorManager::remove() {
    for (auto i = actors_.begin(); i != actors_.end(); ) {
        if ((*i)->is_dead()) {
            delete *i;
            i = actors_.erase(i);
        } else {
            ++i;
        }
    }
}

// アクターの検索
Actor* ActorManager::find(const std::string& name) const {
    for (auto actor : actors_) {
        if (actor->name() == name) {
            return actor;
        }
    }
    return nullptr;
}

// 指定したタグ名のアクターの検索
std::vector<Actor*> ActorManager::find_with_tag(const std::string& tag) const {
    std::vector<Actor*> result;
    for (auto actor : actors_) {
        if (actor->tag() == tag) {
            result.push_back(actor);
        }
    }
    return result;
}

// アクター数を返す
int ActorManager::count() const {
    return (int)actors_.size();
}

// 指定したタグ名を持つアクター数を返す
int ActorManager::count_with_tag(const std::string& tag) const {
    int result = 0;
    for (auto actor : actors_) {
        if (actor->tag() == tag) {
            ++result;
        }
    }
    return result;
}

// メッセージの送信
void ActorManager::send_message(const std::string& message, void* param) {
    for (auto actor : actors_) {
        actor->handle_message(message, param);
    }
}

// 消去
void ActorManager::clear() {
    for (auto actor : actors_) {
        actor->clear();
        delete actor;
    }
    actors_.clear();
}

//初期化
void ActorManager::initialize(GTUINT layer_num, Rect field_size) {
    quadtree_.initialize(field_size);
}