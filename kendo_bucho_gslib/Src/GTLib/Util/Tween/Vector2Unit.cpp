#include "Vector2Unit.h"

Vector2Unit::Vector2Unit(const Vector2& from, const Vector2& to, float duration, std::function<void(const Vector2&)> update_func) :TweenUnit{ duration }, from_{ from }, to_{ to }, update_func_{ update_func } {}
// XV‚Ìˆ—
void Vector2Unit::on_update(float progress) {
    update_func_(Mathf::LERP<Vector2>(progress, from_, to_));
}