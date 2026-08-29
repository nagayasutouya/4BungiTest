#ifndef UTIL_H_
#define UTIL_H_

#include <string>
#include <vector>
#include <unordered_map>
#include "GTLib/GTLib.h"

class Actor;
//いろんなところで使えそうな計算類
namespace Util {
	extern inline float Vector3ToFloat(const Vector3& add1, const Vector3& add2);//二つのベクター３をかけ合わせて各要素を足してfloatにする
	extern inline Vector3 Cross(Vector3 add1, Vector3 add2);//
	extern inline int RandomRange(int min, int max);	//指定した範囲のなかからランダムな数字を返してくれる
	extern inline float RandomRange(float min, float max);	//指定した範囲のなかからランダムな数字を返してくれる
	extern inline float RandomPlusMinus(float value);	//指定した数値～指定数値の負の数までのランダムな数字を返してくれる
	extern inline float UIBarChange(float cooltime, float texture_size, float now);//現状の数値が最大値のどれくらいになるのかを、テクスチャの高さに適応してくれる　UIのスキルチャージ等にどうぞ
	extern inline int UIBarChange(int cooltime, int texture_size, int now);//現状の数値が最大値のどれくらいになるのかを、テクスチャの高さに適応してくれる　UIのスキルチャージ等にどうぞ
	extern inline bool IsHpUnder(int actor_hp, int line);//HPが指定数値より下ならtrue
	extern inline float PointInToSquare(float point_x, float point_y, const Vector2& left_top, const Vector2& right_bottom);	//対象の点が指定した四角形の範囲内にいるかどうか
	extern inline float TargetDistance(const Vector3& target, const Vector3& from);	//ターゲット一体との距離を返す
	extern inline float TargetDistance(const Vector2&target,const Vector2&from);	//ターゲット一体との距離を返す
	extern inline Vector3 ToTarget(const Vector3& target_position, const Vector3& from_position);	//対象に向かって移動するベクトルを出してくれる
	extern inline Vector2 ToTarget(const Vector2& target_position, const Vector2& from_position);	//対象に向かって移動するベクトルを出してくれる
	extern inline bool IsOverLine(float target,float line,float add);//足し引きに関わらず一定のラインを超えたらTrue返す
	extern inline bool IsOverLine(int target,int line,int add);//足し引きに関わらず一定のラインを超えたらTrue返す
	extern inline float TargetAngle(const Transform& target, const Transform& center);//円周上から角度出す
	extern inline float TargetAngle(const Vector2& target, const Vector2& center);//円周上から角度出す
	extern inline Vector3 RoundVelocity(float angle, const Vector3& center, const float radius);//回転速度出す
	extern inline Vector2 RoundVelocity(float angle, const Vector2& center, const float radius);//回転速度出す
	extern inline float PlusOrMinus(const float target);//対象が正か負かを返す
	extern inline int PlusOrMinus(const int target);//対象が正か負かを返す
	extern inline unsigned int StringCount(std::string data);//文字列の文字数を返す
	extern inline int porm(float input, float line);
	template<typename CheckData>
	extern inline bool FindTargetInVector(std::vector<CheckData>contena, CheckData data) {//対象のデータがあるかを返す
		return contena.end()!=std::find(contena.begin(), contena.end(), data);
	}	
	template<typename CheckDataKey, typename CheckDataUM>
	extern inline bool FindTargetKeyInUnorderedMap(std::unordered_map<CheckDataKey,CheckDataUM>contena, CheckDataKey data) {//対象のデータがあるかを返す
		return contena.find(data)!=contena.end();
	}
};
#endif