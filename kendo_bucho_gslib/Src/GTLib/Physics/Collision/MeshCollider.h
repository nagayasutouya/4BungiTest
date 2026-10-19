#ifndef MESH_COLLIDER_H_
#define MESH_COLLIDER_H_

#include <vector>
#include <string>
#include "GTLib/Math/Vector3.h"
#include "GTLib/Math/Plane.h"
#include "GTLib/Math/AABB.h"
#include "GTLib/Math/Line.h"
#include "GTLib/Math/Sphere.h"
#include "GTLib/Math/Triangle.h"

// メッシュコライダー
class MeshCollider {
public:
    // オクトリーノード構造体
    struct Node {
        // バウンディングボックス
        AABB bounding_box;
        // 三角形の先頭インデックス (-1 = 内部ノードで三角形なし)
        int triangle_index{ -1 };
        // このノード内にある三角形の要素数
        int num_triangles{ 0 };
        // 子ノードの配列 (-1 = 子なし)
        int children[8]{ -1, -1, -1, -1, -1, -1, -1, -1 };
    };

public:
    // コンストラクタ
    MeshCollider() = default;
    // デストラクタ
    ~MeshCollider() = default;

    // ファイルの読み込み
    void load(const std::string& file_name);
    // 球との衝突判定
    bool intersect_sphere(const Sphere& sphere, SphereHit* out_hit = nullptr,
        int max_iterations = 4) const;
    // 線分との衝突判定
    bool intersect_line_segment(const Line& line,
        LineHit* out_hit = nullptr) const;
    // 消去
    void clear();

private:
    // 内部探索用再帰関数 (球: 面領域)
    void traverse_sphere_face(const Node& node, Sphere& current_sphere,
        Vector3& total_push_out, bool& collided, float& max_penetration,
        Vector3& best_hit_point, Vector3& best_hit_normal) const;
    // 内部探索用再帰関数 (球: エッジ・点領域)
    void traverse_sphere_edge(const Node& node, Sphere& current_sphere,
        Vector3& total_push_out, bool& collided, float& max_penetration,
        Vector3& best_hit_point, Vector3& best_hit_normal) const;
    // 内部探索用再帰関数 (線分)
    void traverse_line_segment(const Node& node, const Line& line,
        const Vector3& dir_norm, float max_dist, bool& hit_any, float& closest_dist,
        LineHit& best_hit) const;

private:
    // ノード配列
    std::vector<Node> nodes_;
    // 頂点座標配列
    std::vector<Triangle> triangles_;
};

#endif

