#ifndef VECTOR2_UNIT_H_
#define VECTOR2_UNIT_H_

#include"TweenUnit.h"

// Vector3型を動かすためのTweenUnit
class Vector2Unit : public TweenUnit {
public:
    // コンストラクタ
    Vector2Unit(const Vector2& from, const Vector2& to, float duration, std::function<void(const Vector2&)> update_func);
    // 更新時の処理
    virtual void on_update(float progress) override;

private:
    Vector2 from_;                                // 開始値
    Vector2 to_;                                  // 終了値
    std::function<void(Vector2)> update_func_;    // 更新時の処理
};

#endif