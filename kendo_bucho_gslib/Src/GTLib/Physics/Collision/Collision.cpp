#include <unordered_map>
#include <memory>
#include "Collision.h"
#include "MeshCollider.h"

namespace {
	std::unordered_map < std::uint32_t, std::unique_ptr<MeshCollider>> mesh_colliders_;//メッシュコライダーの塊
}

// 衝突判定システムの初期化
void CollisionPhysics::initialize() {
	mesh_colliders_.clear();
}
// 衝突判定システムの終了
void CollisionPhysics::finalize() {
	mesh_colliders_.clear();
}

//球と球
bool CollisionPhysics::CollisionSphereAndSphere(const Vector3& flom_pos, const float flom_radius, const Vector3& at_pos, const float at_radius) {
	float distance = (at_pos-flom_pos).magnitude();
	return distance<=flom_radius+at_radius;
}

// メッシュコライダ―ファイルの読み込み
MeshCollider* CollisionPhysics::load_mesh_collider(std::uint32_t mesh_collider_id, const std::string& file_name) {
	MeshCollider* mesh_c = new MeshCollider{};
	mesh_c->load(file_name);
	mesh_colliders_[mesh_collider_id] = std::unique_ptr<MeshCollider>(mesh_c);
	return mesh_c;
}
// メッシュコライダ―の削除
void CollisionPhysics::delete_mesh_collider(std::uint32_t mesh_collider_id) {
	mesh_colliders_.erase(mesh_collider_id);
}
// メッシュコライダ―の取得
MeshCollider* CollisionPhysics::get_mesh_collider(std::uint32_t mesh_collider_id) {
	//なかったらnullptr
	return (is_mesh_collider_alive(mesh_collider_id))?mesh_colliders_[mesh_collider_id].get() : nullptr;
}
// メッシュコライダ―と球の衝突判定
bool CollisionPhysics::intersect_mesh_sphere(std::uint32_t mesh_collider_id,const Sphere& sphere, SphereHit* out_hit , int max_iterations) {
	//なかったらfalse
	if (!is_mesh_collider_alive(mesh_collider_id))return false;
	return mesh_colliders_[mesh_collider_id]->intersect_sphere(sphere, out_hit, max_iterations);
}
// メッシュコライダ―と線分の衝突判定
bool CollisionPhysics::intersect_mesh_line_segment(std::uint32_t mesh_collider_id, const Line& line, LineHit* out_hit) {
	//なかったらfalse
	if (!is_mesh_collider_alive(mesh_collider_id))return false;
	return mesh_colliders_[mesh_collider_id]->intersect_line_segment(line,out_hit);
}
//そのメッシュコライダーがあるか
bool CollisionPhysics::is_mesh_collider_alive(std::uint32_t mesh_collider_id) {
	return mesh_colliders_.find(mesh_collider_id) != mesh_colliders_.end();
}