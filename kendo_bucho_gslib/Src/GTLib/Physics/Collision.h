#ifndef COLLISION_H_ 
#define COLLISION_H_

#include"GTLib/Math/Vector3.h"

namespace CollisionPhysics {
	bool CollisionSphereAndSphere(const Vector3& flom_pos, const float flom_radius, const Vector3& at_pos, const float at_radius );
}

#endif