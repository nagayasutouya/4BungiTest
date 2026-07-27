#ifndef ACTOR_H_
#define ACTOR_H_

#include "GTLib/GTLib.h"
#include <string>
#include "BoundingSphere.h"
#include "Sibungi/QuadTreeManager.h"

class IWorld;

// アクタークラス
class Actor {
public:
	// コンストラクタ
	Actor() = default;
	// 仮想デストラクタ
	~Actor();
	// 更新
	virtual void update(float delta_time);
	// 遅延更新
	virtual void late_update(float delta_time);
	// 描画
	virtual void draw() const;
	// 半透明の描画
	virtual void draw_transparent() const;
	// GUIの描画
	virtual void draw_gui() const;
	// 衝突リアクション
	virtual void react(Actor& other);
	// メッセージ処理
	virtual void handle_message(const std::string& message, void* param);
	// 衝突判定
	void collide(Actor& other);
	// 死亡する
	void die();
	//削除
	void clear();
	// 衝突しているか？
	bool is_collide(const Actor& other) const;
	bool enable_collider()const { return enable_collider_; }
	// 死亡しているか？
	bool is_dead() const;
	// 名前を取得
	const std::string& name() const;
	// タグ名を取得
	const std::string& tag() const;
	// トランスフォームを取得（const版）
	const Transform& transform() const;
	// トランスフォームを取得
	Transform& transform();
	// 移動量を取得
	Vector3 velocity() const;
	// 衝突判定データを取得
	BoundingSphere collider() const;
	//空間データ
	void add_space();
	QuadTreeObj<Actor>* space()const { return obj_; }
	//サイズ
	Rect space_check_size();
	// コピー禁止
	Actor(const Actor& other) = delete;
	Actor& operator = (const Actor& other) = delete;

protected:
	// ワールド
	IWorld*			world_{ nullptr };
	// タグ名
	std::string		tag_;
	// 名前
	std::string		name_;
	// トランスフォーム
	Transform		transform_;
	// 移動量
	Vector3		velocity_{ 0.0f, 0.f, 0.0f };
	// 衝突判定が有効か？
	bool			enable_collider_{ true };
	// 衝突判定
	BoundingSphere	collider_;
	// 死亡フラグ
	bool			dead_{ false };
	//空間接続用
	QuadTreeObj<Actor>* obj_{nullptr};
};

#endif
