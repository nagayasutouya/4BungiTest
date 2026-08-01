#include "Scroll.h"
#include "GTLib/GTLib.h"

//選択位置スクロール
bool Scroll::select_change(int &current_serect,int change, float delta_time, int max, float wait_max) {
	if (change == 0)return reset();
	if (down_speed_ <= 0.0f) {
		current_serect += change;
		current_serect = Mathf::CLAMP(current_serect, 0, max);
		if (set_scroll(delta_time, wait_max))return true;
	}
	down_speed_ -= delta_time;
	return false;
}
//選択位置スクロール
bool Scroll::select_change_inf(int& current_serect, int change, float delta_time, int max, float wait_max) {
	if (change == 0&&change!=before_change_)return reset();
	before_change_ = change;
	if (down_speed_ <= 0.0f) {
		current_serect += change;
		if (current_serect > max)current_serect = 0;
		else if (current_serect < 0)current_serect = max;
		return set_scroll(delta_time, wait_max);
	}
	down_speed_ -= delta_time;
	return false;
}
//待機スクロールだけ
bool Scroll::wait_and_scroll(bool change,float delta_time, float wait_max) {
	if (!change) { return reset(); }
	if (down_speed_ <= 0.0f) {
		if (set_scroll(delta_time, wait_max))return true;
	}
	down_speed_ -= delta_time;
	return false;
}
bool Scroll::wait(bool change, float delta_time, float wait_max) {
	if (!change) { return reset(); }
	if (down_speed_ <= 0.0f) {
		down_speed_ = wait_max;
		return true;
	}
	down_speed_ -= delta_time;
	return false;
}
//待機時間徐々に減る
bool Scroll::wait_dec(bool change, float delta_time, float wait_max, float dec_speed){
	if (!change) { reset(); down_wait_ = wait_max; return false; }
	if (down_speed_ <= 0.0f) {
		down_speed_ = down_wait_;
		down_wait_ -= dec_speed * delta_time;
		return true;
	}
	down_speed_ -= delta_time;
	return false;
}
bool Scroll::reset(){
	first_cont_flag_ = true;
	down_speed_ = down_wait_ = 0.0f;
	return false;
}

bool Scroll::set_scroll(float delta_time, int wait_max){
	if (down_wait_ <= 0.0f) {
		if (first_cont_flag_) {
			down_speed_ = down_wait_ = wait_max;
			first_cont_flag_ = false;
		}
		else down_wait_ = 0.0f;
		return true;
	}
	else {
		down_wait_ -= delta_time;
		down_speed_ = down_wait_;
	}
	return false;
}