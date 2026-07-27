#include "Actor.h"

Actor::~Actor() {
	clear();
}

// 更新
void Actor::update(float delta_time) { }

// 遅延更新
void Actor::late_update(float delta_time) { }

// 描画
void Actor::draw() const { }

// 半透明オブジェクトの描画
void Actor::draw_transparent() const { }

// GUIの描画
void Actor::draw_gui() const { }

// 衝突リアクション
void Actor::react(Actor& other) { }

// メッセージ処理
void Actor::handle_message(const std::string& message, void* param) { }

// 衝突判定
void Actor::collide(Actor& other) {
	// どちらのアクターも衝突判定が有効か？
	if (enable_collider_ && other.enable_collider_) {
		// 衝突判定をする
		if (is_collide(other)) {
			// 衝突した場合は、お互いに衝突リアクションをする
			react(other);
			other.react(*this);
		}
	}
}
//削除
void Actor::clear() {
	obj_->remove();
	obj_->clear();
	delete obj_;
	obj_ = nullptr;
}
// 死亡する
void Actor::die() {
	dead_ = true;
}

// 衝突しているか？
bool Actor::is_collide(const Actor& other) const {
	return collider().intersects(other.collider());
}

// 死亡しているか？
bool Actor::is_dead() const {
	return dead_;
}

// 名前を取得
const std::string& Actor::name() const {
	return name_;
}

// タグ名を取得
const std::string& Actor::tag() const {
	return tag_;
}

// トランスフォームを取得（const版）
const Transform& Actor::transform() const {
	return transform_;
}

// トランスフォームを取得
Transform& Actor::transform() {
	return transform_;
}

// 移動量を取得
Vector3 Actor::velocity() const {
	return velocity_;
}

// 衝突判定データを取得
BoundingSphere Actor::collider() const {
	return collider_.transform(transform_.local_to_world_matrix());
}

void Actor::add_space() {
	QuadTreeObj<Actor>* obj = new QuadTreeObj<Actor>{};
	obj->obj = this;
	obj_=obj;
}
//サイズ
Rect Actor::space_check_size() {
	float rad = collider().radius;
	return Rect{ transform_.position().x - rad,transform_.position().z + rad,transform_.position().x + rad,transform_.position().z - rad };
}