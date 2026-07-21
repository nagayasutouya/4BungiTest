#ifndef RADAR_H_
#define RADAR_H_

#include "Actor.h"

// レーダークラス
class Radar : public Actor {
public:
    // コンストラクタ
    Radar(IWorld* world);
    // 描画
    void draw_gui() const override;

private:
    // レーダーの点を描画
    void draw_point(const Vector3& position, const Color& color) const;
};

#endif
