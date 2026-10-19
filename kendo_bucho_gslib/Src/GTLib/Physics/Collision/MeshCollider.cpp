#include "MeshCollider.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

// 消去
void MeshCollider::clear() {
    nodes_.clear();
    triangles_.clear();
}
// ファイルの読み込み
void MeshCollider::load(const std::string& file_name) {
    std::ifstream file{ file_name };
    if (!file) {
        // ファイルがオープンできなかった
        throw std::runtime_error("can not open " + file_name);
    }
    // 消去
    clear();

    // ノードの読み込み
    size_t node_count{ 0 };
    // ノード数の読み込み
    file >> node_count;
    nodes_.resize(node_count);
    for (Node& node : nodes_) {
        file >> node.bounding_box.min_bound.x
            >> node.bounding_box.min_bound.y
            >> node.bounding_box.min_bound.z;
        file >> node.bounding_box.max_bound.x
            >> node.bounding_box.max_bound.y
            >> node.bounding_box.max_bound.z;
        file >> node.triangle_index >> node.num_triangles;
        for (int& child : node.children) {
            file >> child;
        }
    }

    // 三角形の読み込み
    size_t triangle_count{ 0 };
    // 三角形数の読み込み
    file >> triangle_count;
    triangles_.resize(triangle_count);
    for (Triangle& tri : triangles_) {
        file >> tri.vertices[0].x >> tri.vertices[0].y >> tri.vertices[0].z;
        file >> tri.vertices[1].x >> tri.vertices[1].y >> tri.vertices[1].z;
        file >> tri.vertices[2].x >> tri.vertices[2].y >> tri.vertices[2].z;
        file >> tri.plane.normal.x >> tri.plane.normal.y >> tri.plane.normal.z >> tri.plane.d;
    }
}
// 球の衝突判定
bool MeshCollider::intersect_sphere(const Sphere& sphere, SphereHit* out_hit,
    int max_iterations) const {
    // データがない場合は衝突しない
    if (nodes_.empty() || triangles_.empty()) return false;
    // 衝突判定を繰り返すための変数
    Sphere current_sphere{ sphere };
    Vector3 total_push_out{ 0.0f, 0.0f, 0.0f };
    Vector3 best_hit_point;
    Vector3 best_hit_normal;
    float max_penetration = 0.0f;
    bool collided = false;
    // 繰り返し回数分だけ衝突判定を行う
    for (int iter = 0; iter < max_iterations; ++iter) {
        bool iter_collided = false;
        // 面での優先押し出し
        traverse_sphere_face(nodes_[0], current_sphere, total_push_out, iter_collided, max_penetration, best_hit_point, best_hit_normal);
        // エッジ・点での二次押し出し
        traverse_sphere_edge(nodes_[0], current_sphere, total_push_out, iter_collided, max_penetration, best_hit_point, best_hit_normal);
        // 繰り返し終了条件
        if (!iter_collided) break;
        // 衝突フラグを立てる
        collided = true;
    }
    // 結果を格納
    if (collided && out_hit) {
        out_hit->hit = true;
        out_hit->push_out = total_push_out;
        out_hit->point = best_hit_point;
        out_hit->normal = best_hit_normal;
    }
    return collided;
}
// 再帰オクトリー探索 (面領域押し出し)
void MeshCollider::traverse_sphere_face(const Node& node, Sphere& current_sphere, Vector3& total_push_out, bool& collided, float& max_penetration, Vector3& best_hit_point, Vector3& best_hit_normal) const {
    // ノードの AABB と触れてなければスキップ
    if (!node.bounding_box.intersects_sphere(current_sphere)) return;

    // 三角形があるノードか？
    if (node.num_triangles > 0) {
        int end_idx = node.triangle_index + node.num_triangles;
        // ノード内の三角形すべてに対して処理
        for (int i = node.triangle_index; i < end_idx; ++i) {
            // 現在の三角形と球の中心との距離を求める
            const Triangle& tri = triangles_[i];
            float dist_to_plane = tri.plane.distance_to_point(current_sphere.center);
            // 球の中心が面から離れすぎてる場合はスキップ
            if (dist_to_plane < 0.0f || dist_to_plane > current_sphere.radius) continue;

            // 球の中心から面に最も近い点を求める
            const Vector3& face_normal = tri.plane.normal;
            Vector3 proj_point = current_sphere.center - face_normal * dist_to_plane;

            // 球の中心が三角形の内部にあるか判定
            if (tri.contains_point(proj_point)) {
                // 衝突を検出
                collided = true;
                // 侵入している深さを計算
                float penetration = current_sphere.radius - dist_to_plane;
                // 押し出しベクトルを計算
                Vector3 push = face_normal * penetration;
                // 球の中心を面から押し出す
                current_sphere.center += push;
                total_push_out += push;

                // 最大侵入深さを更新
                if (penetration > max_penetration) {
                    max_penetration = penetration;
                    best_hit_point = proj_point;
                    best_hit_normal = face_normal;
                }
            }
        }
    }
    // 子ノードの再帰探索
    for (int child_index : node.children) {
        if (child_index >= 0) {
            traverse_sphere_face(nodes_[child_index], current_sphere, total_push_out, collided, max_penetration, best_hit_point, best_hit_normal);
        }
    }
}
// 再帰オクトリー探索 (エッジ・点領域押し出し)
void MeshCollider::traverse_sphere_edge(const Node& node, Sphere& current_sphere,
    Vector3& total_push_out, bool& collided, float& max_penetration,
    Vector3& best_hit_point, Vector3& best_hit_normal) const {
    // ノードの AABB と触れてなければスキップ
    if (!node.bounding_box.intersects_sphere(current_sphere)) return;

    // 三角形があるノードか？
    if (node.num_triangles > 0) {
        int end_idx = node.triangle_index + node.num_triangles;
        for (int i = node.triangle_index; i < end_idx; ++i) {
            const Triangle& tri = triangles_[i];
            // 最近接点を求める
            Vector3 closest = tri.closest_point(current_sphere.center);
            Vector3 diff = current_sphere.center - closest;
            float dist_sq = diff.sqr_magnitude();
            // 衝突判定
            if (dist_sq < current_sphere.radius * current_sphere.radius && dist_sq > 1e-8f) {
                collided = true;
                float dist = std::sqrt(dist_sq);
                float penetration = current_sphere.radius - dist;
                // 法線を求める
                Vector3 normal = diff / dist;
                // 押し出す
                Vector3 push = normal * penetration;
                current_sphere.center += push;
                total_push_out += push;
                // 最大侵入深さを更新
                if (penetration > max_penetration) {
                    max_penetration = penetration;
                    best_hit_point = closest;
                    best_hit_normal = normal;
                }
            }
        }
    }
    // 子ノードの再帰探索
    for (int child_index : node.children) {
        if (child_index >= 0) {
            traverse_sphere_edge(nodes_[child_index], current_sphere, total_push_out, collided, max_penetration, best_hit_point, best_hit_normal);
        }
    }
}
// 線分との衝突判定
bool MeshCollider::intersect_line_segment(const Line& line, LineHit* out_hit) const {
    // データがない場合は衝突しない
    if (nodes_.empty() || triangles_.empty()) return false;

    // 線分の方向ベクトルを正規化
    Vector3 dir = line.end - line.start;
    float max_dist = dir.magnitude();
    if (max_dist < 1e-6f) return false;
    Vector3 dir_norm = dir / max_dist;
    // 衝突判定を繰り返すための変数
    bool hit_any = false;
    float closest_dist = max_dist + 1.0f;
    LineHit best_hit;
    // 再帰オクトリー探索
    traverse_line_segment(nodes_[0], line, dir_norm, max_dist, hit_any, closest_dist, best_hit);
    // 結果を格納
    if (hit_any && out_hit) {
        *out_hit = best_hit;
    }
    // 衝突判定
    return hit_any;
}
// 再帰オクトリー探索 (線分)
void MeshCollider::traverse_line_segment(const Node& node, const Line& line, const Vector3& dir_norm, float max_dist, bool& hit_any, float& closest_dist, LineHit& best_hit) const {
    // ノードの AABB と触れてなければスキップ
    if (!node.bounding_box.intersects_line_segment(line)) return;

    // 三角形があるノードか？
    if (node.num_triangles > 0) {
        int end_idx = node.triangle_index + node.num_triangles;
        for (int tri_idx = node.triangle_index; tri_idx < end_idx; ++tri_idx) {
            const Triangle& tri = triangles_[tri_idx];
            LineHit hit;
            if (tri.intersect_line_segment(line.start, dir_norm, max_dist, hit)) {
                if (hit.distance < closest_dist) {
                    closest_dist = hit.distance;
                    best_hit = hit;
                    hit_any = true;
                }
            }
        }
    }
    // 子ノードの再帰探索
    for (int child_index : node.children) {
        if (child_index >= 0) {
            traverse_line_segment(nodes_[child_index],
                line, dir_norm, max_dist, hit_any, closest_dist, best_hit);
        }
    }
}