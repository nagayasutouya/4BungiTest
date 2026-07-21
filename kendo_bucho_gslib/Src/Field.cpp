#include "Field.h"
#include "Ray.h"
#include "Line.h"
#include "BoundingSphere.h"

// コンストラクタ
Field::Field(unsigned int octree, unsigned int collider, unsigned int skybox) :
	octree_{ octree }, collider_{ collider }, skybox_{ skybox } {
}

// 更新
void Field::update(float delta_time) { }

// 描画
void Field::draw() const {
	// スカイボックスの描画
	Graphics::set_mesh_pipeline_state(PipelineStateID::Mesh_Skydome);
	Graphics::draw_mesh(skybox_);
	Graphics::set_mesh_pipeline_state(PipelineStateID::Mesh_Opaque);
	Matrix4x4 world = Matrix4x4::identity();
	Graphics::set_world_matrix(world);
	Graphics::draw_mesh(octree_);
}

// 線分との衝突判定
bool Field::collide(const Line& line, Vector3* intersect, Plane* plane) const {
	return false;
}

// レイとの衝突判定
bool Field::collide(const Ray& ray, float max_distance, Vector3* intersect, Plane* plane) const {
	return false;
}

bool Field::collide(const BoundingSphere& sphere, Vector3* center) const{
	return false;
}
