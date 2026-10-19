#ifndef COLLISION_H_ 
#define COLLISION_H_

#include"GTLib/Math/Vector3.h"
#include "GTLib/Math/Line.h"
#include "GTLib/Math/Sphere.h"
#include <string>
#include <cstdint>

// 前方宣言
class MeshCollider;

namespace CollisionPhysics {
    // 衝突判定システムの初期化
    void initialize();
    // 衝突判定システムの終了
    void finalize();
    
    //球と球の当たり判定
	bool CollisionSphereAndSphere(const Vector3& flom_pos, const float flom_radius, const Vector3& at_pos, const float at_radius );

    // メッシュコライダ―ファイルの読み込み
    MeshCollider* load_mesh_collider(std::uint32_t mesh_collider_id,
        const std::string& file_name);
    // メッシュコライダ―の削除
    void delete_mesh_collider(std::uint32_t mesh_collider_id);
    // メッシュコライダ―の取得
    MeshCollider* get_mesh_collider(std::uint32_t mesh_collider_id);

    // メッシュコライダ―と球の衝突判定
    bool intersect_mesh_sphere(std::uint32_t mesh_collider_id,
        const Sphere& sphere, SphereHit* out_hit = nullptr, int max_iterations = 4);
    // メッシュコライダ―と線分の衝突判定
    bool intersect_mesh_line_segment(std::uint32_t mesh_collider_id,
        const Line& line, LineHit* out_hit = nullptr);
    //そのメッシュコライダーがあるか
    bool is_mesh_collider_alive(std::uint32_t mesh_collider_id);
}

#endif