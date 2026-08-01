#include "FadeSystem.h"
#include "GTLib/Util/Util.h"
#include "GTLib/GTLib.h"

void FadeSystem::update(float delta_time){
	if (is_end_)return;
	current_ += change_speed_*delta_time;
	if (Util::PlusOrMinus(change_speed_) < 0.0f) {
		if (change_max_ > current_)is_end_ = true;
	}
	else if (current_>change_max_) {
		is_end_ = true;
	}
}

void FadeSystem::draw()const{
	Color color{ texture_.color_};
	color.a = current_;
	Graphics::draw_sprite2d(texture_.texture_id_,texture_.position_,&texture_.rect_,&texture_.center_,&color,&texture_.scale_,texture_.angle_);
}

void FadeSystem::input(TextureData data, float change_speed,float start, float change_max){
	texture_ = data;
	change_speed_ = change_speed;
	change_max_ = change_max;
	current_ = start;
}