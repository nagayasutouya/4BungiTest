#include "Vector3Unit.h"

Vector3Unit::Vector3Unit(const Vector3& from, const Vector3& to, float duration,std::function<void(const Vector3&)> update_func) :TweenUnit{ duration },from_{ from },to_{ to },update_func_{ update_func } {}
// XV‚Ìˆ—
void Vector3Unit::on_update(float progress) {
    update_func_(Mathf::LERP<Vector3>(progress, from_, to_));
}