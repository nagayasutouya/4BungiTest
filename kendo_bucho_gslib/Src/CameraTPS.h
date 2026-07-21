#ifndef CAMERA_TPS_H_
#define CAMERA_TPS_H_

#include "Actor.h"

// 三人称カメラクラス
class CameraTPS : public Actor {
public:
    // コンストラクタ
    CameraTPS(IWorld* world, const Vector3& position, const Vector3& at);
    // 更新
    virtual void update(float delta_time) override;
    // 描画
    virtual void draw() const override;
};

#endif
