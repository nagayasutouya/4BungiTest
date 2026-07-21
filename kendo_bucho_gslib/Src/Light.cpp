#include "Light.h"
//#include <imgui/imgui.h>

// コンストラクタ
Light::Light(IWorld* world) {
    world_ = world;
    name_ = "Light";
    tag_ = "LightTag";
}
// 描画
void Light::draw() const {
    static float ambient[4] = { 0.4f, 0.4f, 0.4f, 1.0f };
    static float diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    static float specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    static float position[4] = { 100.0f, 100.0f, 100.0f, 0.0f };

    //glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    //glLightfv(GL_LIGHT0, GL_POSITION, position);
    //glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHTING);
}
