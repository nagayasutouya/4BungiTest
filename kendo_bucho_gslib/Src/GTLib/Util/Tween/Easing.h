#ifndef EASING_H_
#define EASING_H_

#include <cmath>

inline float GTEasingInBack(float t, float s = 1.70158f){
	return t * t * ((s + 1) * t - s);
}

inline float GTEasingInBounce(float t){
	t = 1.0f - t;
	if (t < 1.0f / 2.75f) {
		return 1.0f - (7.5625f * t * t);
	}
	if (t < 2.0f / 2.75f) {
		t -= 1.5f / 2.75f;
		return 1.0f - (7.5625f * t * t + 0.75f);
	}
	if (t < 2.5f / 2.75f) {
		t -= 2.25f / 2.75f;
		return 1.0f - (7.5625f * t * t + 0.9375f);
	}
	t -= 2.625f / 2.75f;
	return 1.0f - (7.5625f * t * t + 0.984375f);
}

inline float GTEasingInCirc(float t){
	return -(std::sqrt(1.0f - t * t) - 1.0f);
}

inline float GTEasingInCubic(float t){
	return t * t * t;
}

inline float GTEasingInElastic(float t, float s = 2.094395102f){
	return t <= 0.0f
		? 0.0f
		: t >= 1.0f
		? 1.0f
		: -pow(2.0f, 10.0f * t - 10.0f) * sin((t * 10.0 - 10.75f) * s);
}

inline float GTEasingInExpo(float t){
	return (t <= 0.0) ? 0.0f : std::pow(2.0f, 10.0f * (t - 1.0f));
}

inline float GTEasingInQuad(float t){
	return t * t;
}

inline float GTEasingInQuart(float t){
	return t * t * t * t;
}

inline float GTEasingInQuint(float t){
	return t * t * t * t * t;
}

inline float GTEasingInSine(float t){
	return -std::cos(t * (3.14159265359f / 2.0f)) + 1.0f;
}

inline float GTEasingOutBack(float t, float s = 1.70158f){
	t -= 1.0f;
	return t * t * ((s + 1.0f) * t + s) + 1.0f;
}

inline float GTEasingOutBounce(float t){
	if (t < 1.0f / 2.75f) {
		return 7.5625f * t * t;
	}
	if (t < 2.0f / 2.75f) {
		t -= 1.5f / 2.75f;
		return 7.5625f * t * t + 0.75f;
	}
	if (t < 2.5f / 2.75f) {
		t -= 2.25f / 2.75f;
		return 7.5625f * t * t + 0.9375f;
	}
	t -= 2.625f / 2.75f;
	return 7.5625f * t * t + 0.984375f;
}

inline float GTEasingOutCirc(float t){
	t -= 1.0f;
	return std::sqrt(1.0f - t * t);
}

inline float GTEasingOutCubic(float t){
	t -= 1.0f;
	return t * t * t + 1.0f;
}

inline float GTEasingOutElastic(float t, float s = 2.094395102f){
	return t <= 0.0f
		? 0.0f
		: t >= 1.0f
		? 1.0f
		: std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * s) + 1.0f;
}

inline float GTEasingOutExpo(float t){
	return (t >= 1.0f) ? 1.0f : (-std::pow(2.0f, -10.0f * t) + 1.0f);
}

inline float GTEasingOutQuad(float t){
	return -t * (t - 2.0f);
}

inline float GTEasingOutQuart(float t){
	t -= 1.0f;
	return 1.0f - t * t * t * t;
}

inline float GTEasingOutQuint(float t){
	t -= 1.0f;
	return t * t * t * t * t + 1.0f;
}

inline float GTEasingOutSine(float t){
	return std::sin(t * (3.14159265359f / 2.0f));
}

inline float GTEasingInOutBack(float t, float s = 1.70158f){
	float k = 1.525f;
	t *= 2.0f;
	s *= k;
	if (t < 1.0f) {
		return 0.5f * (t * t * ((s + 1) * t - s));
	}
	t -= 2.0f;
	return 0.5f * (t * t * ((s + 1) * t + s) + 2.0f);
}

inline float GTEasingInOutBounce(float t){
	return (t < 0.5f)
		? (GTEasingInBounce(t * 2.0f) * 0.5f)
		: (GTEasingOutBounce(t * 2.0f - 1.0f) * 0.5f + 0.5f);
}

inline float GTEasingInOutCirc(float t){
	t *= 2.0f;
	if (t < 1.0f) {
		return -0.5f * (std::sqrt(1.0f - t * t) - 1.0f);
	}
	t -= 2.0f;
	return 0.5f * (std::sqrt(1.0f - t * t) + 1.0f);
}

inline float GTEasingInOutCubic(float t){
	t *= 2.0f;
	if (t < 1.0f) {
		return 0.5f * t * t * t;
	}
	t -= 2.0f;
	return 0.5f * (t * t * t + 2.0f);
}

inline float GTEasingInOutElastic(float t, float s = 1.396263402f){
	return t <= 0.0f
		? 0.0f
		: t >= 1.0f
		? 1.0f
		: t < 0.5f
		? -(std::pow(2.0f, 20.0f * t - 10.0f) * std::sin((20.0f * t - 11.125f) * s)) / 2.0f
		: (std::pow(2.0f, -20.0f * t + 10.0f) * std::sin((20.0f * t - 11.125f) * s)) / 2.0f + 1.0f;
}

inline float GTEasingInOutExpo(float t){
	if (t == 0.0f) {
		return 0.0f;
	}
	if (t == 1.0f) {
		return 1.0f;
	}
	t *= 2.0f;
	if (t < 1.0f) {
		return 0.5f * std::pow(2.0f, 10.0f * (t - 1.0f));
	}
	t -= 1.0f;
	return 0.5f * (-std::pow(2.0f, -10.0f * t) + 2.0f);
}

inline float GTEasingInOutQuad(float t){
	t *= 2.0f;
	if (t < 1.0f) {
		return 0.5f * t * t;
	}
	t -= 1.0f;
	return -0.5f * (t * (t - 2.0f) - 1.0f);
}

inline float GTEasingInOutQuart(float t){
	t *= 2.0f;
	if (t < 1.0f) {
		return 0.5f * t * t * t * t;
	}
	t -= 2.0f;
	return -0.5f * (t * t * t * t - 2.0f);
}

inline float GTEasingInOutQuint(float t){
	t *= 2.0f;
	if (t < 1.0f) {
		return 0.5f * t * t * t * t * t;
	}
	t -= 2.0f;
	return 0.5f * (t * t * t * t * t + 2.0f);
}

inline float GTEasingInOutSine(float t){
	return -0.5f * (std::cos(3.14159265359f * t) - 1.0f);
}

#endif