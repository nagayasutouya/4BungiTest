#include "ActorManager.h"
#include "Actor.h"

// デストラクタ
ActorManager::~ActorManager() {
    clear();
}

// アクターの追加
void ActorManager::add(Actor* actor) {
<<<<<<< HEAD
    actor->add_space();
    actors_.push_back(actor);
=======
    actors_.push_back(quad_tree_.add_actor(actor));
>>>>>>> Ver2.0
}

// アクターの更新
void ActorManager::update(float delta_time) {
    for (auto actor : actors_) {
        actor->obj()->update(delta_time);
    }
}

// アクターの遅延更新
void ActorManager::late_update(float delta_time) {
    for (auto actor : actors_) {
<<<<<<< HEAD
       if(actor->enable_collider())quadtree_.add_object(actor->space_check_size(), actor->space());
=======
        actor->obj()->late_update(delta_time);
>>>>>>> Ver2.0
    }
}

// アクターの描画
void ActorManager::draw() const {
    for (auto actor : actors_) {
        actor->obj()->draw();
    }
}

// 半透明アクターの描画
void ActorManager::draw_transparent() const {
    for (auto actor : actors_) {
        actor->obj()->draw_transparent();
    }
}

// アクターのGUI描画
void ActorManager::draw_gui() const {
    for (auto actor : actors_) {
        actor->obj()->draw_gui();
    }
}

// アクターの衝突判定
void ActorManager::collide() {
<<<<<<< HEAD
    //衝突判定リストを作成
    GTUINT size=quadtree_.make_collide_list(actors_);
    printf("Size:%d\n", size);
    //リストをもとにチェック
    for (GTUINT i = 0; i < size; i += 2) {
        printf("%s:%d %s:%d\n", quadtree_.collide_list()[i]->name().c_str(), quadtree_.collide_list()[i]->space()->space()->space_id(), quadtree_.collide_list()[i + 1]->name().c_str(), quadtree_.collide_list()[i + 1]->space()->space()->space_id());
        quadtree_.collide_list()[i]->collide(*quadtree_.collide_list()[i+1]);
=======
    for (auto actor : actors_) {
        if (actor->obj() != nullptr) {
            quad_tree_.entry_object_to_space(actor->obj()->transform().position(), actor->obj()->collider().radius, actor);
        }
>>>>>>> Ver2.0
    }
    quad_tree_.collide();
}

// 死亡しているアクターの削除
void ActorManager::remove() {
    for (auto i = actors_.begin(); i != actors_.end(); ) {
        if ((*i)->obj()->is_dead()) {
            (*i)->clear();
            printf("削除\n");
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
        if (actor->obj()->name() == name) {
            return actor->obj();
        }
    }
    return nullptr;
}

// 指定したタグ名のアクターの検索
std::vector<Actor*> ActorManager::find_with_tag(const std::string& tag) const {
    std::vector<Actor*> result;
    for (auto actor : actors_) {
        if (actor->obj()->tag() == tag) {
            result.push_back(actor->obj());
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
        if (actor->obj()->tag() == tag) {
            ++result;
        }
    }
    return result;
}

// メッセージの送信
void ActorManager::send_message(const std::string& message, void* param) {
    for (auto actor : actors_) {
        actor->obj()->handle_message(message, param);
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
void ActorManager::initialize(GTUINT layer_num,Rect field_size) {
    quadtree_.initialize(layer_num,field_size);
}