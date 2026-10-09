#include "Mathf.h"

// 角度からラジアンに変換
float Mathf::deg_to_rad(float deg) {
    return deg * (PI / 180.0f);
}

// ラジアンから角度に変換
float Mathf::rad_to_deg(float rad) {
    return rad * (180.0f / PI);
}

//デカい方
float Mathf::Max(float a, float b) { return (a > b) ? a : b; }
int Mathf::Max(int a, int b) { return (a > b) ? a : b; }

//小さい方
float Mathf::Min(float a, float b) { return (a < b) ? a : b; }
int Mathf::Min(int a, int b) { return (a < b) ? a : b; }

//値を収める
float Mathf::CLAMP(float current, float min, float max) { return Mathf::Max(min, Mathf::Min(current, max)); }
int Mathf::CLAMP(int current, int min, int max) { return Mathf::Max(min, Mathf::Min(current, max)); }

//絶対値
int Mathf::ABS(int a) { return (a > 0) ? a : -a; }

//乗
int Mathf::nth_power(int target, int n) {
	int result = target;
	for (int i = 1; i < n; i++)result *= target;
	return result;
}
//乗
float Mathf::nth_power(float target, int n) {
	float result = target;
	for (int i = 1; i < n; i++)result *= target;
	return result;
}
// f の符号
float Mathf::sign(float f) {
	return f >= 0.0f ? 1.0f : -1.0f;
}