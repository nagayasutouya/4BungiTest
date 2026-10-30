#ifndef VECTOR3_UNIT_H_
#define VECTOR3_UNIT_H_

#include"TweenUnit.h"

// Vector3型を動かすためのTweenUnit
class Vector3Unit : public TweenUnit {
public:
    // コンストラクタ
    Vector3Unit(const Vector3& from, const Vector3& to, float duration, std::function<void(const Vector3&)> update_func);
    // 更新時の処理
    virtual void on_update(float progress) override;

private:
    Vector3 from_;                                // 開始値
    Vector3 to_;                                  // 終了値
    std::function<void(Vector3)> update_func_;    // 更新時の処理
};

#endif

