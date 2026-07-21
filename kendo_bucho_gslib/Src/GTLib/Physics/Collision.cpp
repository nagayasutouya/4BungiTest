#include "Collision.h"

bool CollisionPhysics::CollisionSphereAndSphere(const Vector3& flom_pos, const float flom_radius, const Vector3& at_pos, const float at_radius) {
	float distance = (at_pos-flom_pos).magnitude();
	return distance<=flom_radius+at_radius;
}