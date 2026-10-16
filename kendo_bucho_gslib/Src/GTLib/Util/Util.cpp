#include "GTLib/Util/Util.h"
#include <cmath>

namespace Util {
	//二つのベクター３をかけ合わせて各要素を足してfloatにする
	float Vector3ToFloat(const Vector3& add1, const Vector3& add2) {
		return (add1.x * add2.x) + (add1.y * add2.y) + (add1.z * add2.z);
	}
	Vector3 Cross(Vector3 add1,Vector3 add2){
		return {add1.y * add2.z - add1.z * add2.y,add1.z * add2.x - add1.x * add2.z,	add1.x * add2.y - add1.y * add2.x};
	}
	int RandomRange(int min, int max) {
		return min + (int)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
	}
	float RandomRange(float min, float max) {
		return min + (float)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
	}
	float RandomPlusMinus(float value) {
		return RandomRange(-value, value);
	}
	float UIBarChange(float cooltime, float texture_size, float now) {
		return (texture_size / cooltime) * now;
	}
	int UIBarChange(int cooltime, int texture_size, int now) {
		return (texture_size / cooltime) * now;
	}
	bool IsHpUnder(int actor_hp, int line) {
		if (actor_hp >= line) {
			return false;
		}
		return true;
	}
	float PointInToSquare(float point_x, float point_y, const Vector2& left_top, const Vector2& right_bottom) {
		if (point_x < left_top.x) {
			return false;
		}
		if (point_x > right_bottom.x) {
			return false;
		}
		if (point_y > left_top.y) {
			return false;
		}
		if (point_y < right_bottom.y) {
			return false;
		}
		return true;
	}
	float TargetDistance(const Vector3& target, const Vector3& from) {
		float x = (target.x - from.x);
		float y = (target.y - from.y);
		float z = (target.z - from.z);
		return sqrt(x * x + y * y + z * z);
	}
	float TargetDistance(const Vector2& target, const Vector2& from) {
		float x = (target.x - from.x);
		float y = (target.y - from.y);
		return sqrt(x * x + y * y);
	}
	Vector3 ToTarget(const Vector3& target_pos, const Vector3& from_pos) {
		return (target_pos - from_pos).normalized();
	}
	Vector2 ToTarget(const Vector2& target_pos, const Vector2& from_pos) {
		return (target_pos - from_pos).normalized();
	}
	//余弦定理
	float CosineTheorem(float ab_length, float ac_length, float a_angle) {
		float result = ab_length * ab_length + ac_length * ac_length - 2 * ab_length * ac_length * cos(a_angle);
		return std::sqrt(result);
	}
	//原点とベクトルから、任意の角度の同じ大きさのベクトルを出す
	Vector2 AngleToVector(Vector2 o, Vector2 a, float angle) {
		//原点からの相対距離を出す
		Vector2 a_dash = a - o;
		Vector2 b_dash{ 0.0f,0.0f };
		//角度をラジアン＆絶対値に
		float rad = Mathf::ABS( Mathf::deg_to_rad(angle));
		//反時計回り
		if (angle < 0) {
			b_dash.x = a_dash.x * cos(rad) - a_dash.y * sin(rad);
			b_dash.y = a_dash.x * sin(rad) + a_dash.y * cos(rad);
		}
		//時計回り
		else if (angle > 0) {
			b_dash.x = a_dash.x * cos(rad) + a_dash.y * sin(rad);
			b_dash.y = -(a_dash.x * sin(rad)) + a_dash.y * cos(rad);
		}
		else b_dash = a_dash;
		//絶対座標に戻す
		return b_dash + o;
	}
	bool IsOverLine(float target, float line, float add) {
		if ((add < 0.0f && target <= line) || (add > 0.0f && target >= line)) {
			return true;
		}
		return false;
	}
	bool IsOverLine(int target, int line, int add) {
		if ((add < 0 && target <= line) || (add > 0 && target >= line)) {
			return true;
		}
		return false;
	}
	//角度出す
	float TargetAngle(const Transform& target, const Transform& center) {
		Vector2 from_dot = {center.forward().x,center.forward().z};
		Vector2 target_dot = { target.position().x - center.position().x,target.position().z - center.position().z };
		float angle = atan2(from_dot.x*target_dot.y-from_dot.y*target_dot.x,from_dot.x*target_dot.x+ from_dot.y * target_dot.y);
		return fabs(Mathf::rad_to_deg(angle));
	}
	//角度出す
	float TargetAngle(const Vector2& target, const Vector2& center) {
		float result = 0.0f;
		float radius = TargetDistance(target, center);
		result = (float)acos((target.x - center.x) / radius);
		if (target.y < center.y) {
			result *= -1.0f;
		}
		return result;
	}
	//対象の方向への回転角度を出す
	float target_signed_angle(Transform target, Transform from) {
		Vector3 to_target = target.position() - from.position();
		Vector3 forward = from.forward();

		forward.y = 0.0f;
		to_target.y = 0.0f;

		return Vector3::signed_angle(forward, to_target);
	}
	Vector3 RoundVelocity(float angle, const Vector3& center, const float radius) {
		Vector3 result{ 0.0f,0.0f,0.0f };
		result.x = center.x + (float)cos(Mathf::deg_to_rad(angle)) * radius;//中心点ｘ＋cos(角度)＊半径
		result.y = center.y + (float)sin(Mathf::deg_to_rad(angle)) * radius;//中心点y＋sin(角度)＊半径
		return result;
	}
	Vector2 RoundVelocity(float angle, const Vector2& center, const float radius) {
		Vector2 result{ 0.0f,0.0f };
		result.x = center.x + (float)cos(Mathf::deg_to_rad(angle)) * radius;//中心点ｘ＋cos(角度)＊半径
		result.y = center.y + (float)sin(Mathf::deg_to_rad(angle)) * radius;//中心点y＋sin(角度)＊半径
		return result;
	}
	float PlusOrMinus(const float target) {
		if (target < 0.0f) {
			return -1.0f;
		}
		return 1.0f;
	}
	int PlusOrMinus(const int target) {
		if (target < 0) {
			return -1;
		}
		return 1;
	}
	//文字列の文字数を返す ただしUTF-8にしか使えない
	unsigned int StringCount(std::string data){
		unsigned int result = 0;
		for (unsigned char c : data) {
			if ((c & 0xC0) != 0x80)++result;
		}
		return result;
	}
	int porm(float input, float line) {
		return (input > line) ? 1 : (input < -line) ? -1 : 0;
	}
}