#ifndef FADE_SYSTEM_H_
#define FADE_SYSTEM_H_

#include "GTLib/Data/AssetsStructs.h"

class FadeSystem {
public:
	FadeSystem() = default;
	FadeSystem(TextureData data, float change_speed, float start=0.0f, float change_max = 1.0f) { input(data,change_speed,start,change_max); }
	void update(float delta_time);
	void draw()const;
	void input(TextureData data,float change_speed, float start = 0.0f, float change_max=1.0f);
	bool is_end() { return is_end_; }
private:
	bool is_end_{false};
	TextureData texture_;
	float change_speed_{ 0.0f };
	float change_max_{ 0.0f};
	float current_{ 0.0f };
};

#endif