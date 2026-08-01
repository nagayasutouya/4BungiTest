#include"SoundController.h"

void SoundController::sound_change(const float option_volume, GTUINT id, SoundID sound_type, const float change_late, const float change_max){
	option_volume_= option_volume ;
	id_= id ;
	sound_type_= sound_type ;
	change_late_= change_late ;
	change_max_= change_max ;
	current_volume_ = 0.0f;
	if (change_late_ < 0.0f) {
		is_dec_ = true;
		current_volume_ = 1.0f;
	}
	is_dead_ = false;
}

void SoundController::update_sc(float delta_time){
	if (!is_dead_) {
		if (volume_chage(delta_time)) {
			switch (sound_type_) {
			case SoundID::SID_BGM_Change:
				sound_type_ = SoundID::SID_BGM;
				change_late_ = -change_late_;
				if (change_late_ < 0.0f) {
					is_dec_ = true;
				}
				Sound::play(id_);
				change_max_ = reverce_data(change_max_);
				break;
			default:
				is_dead_ = true;
				break;
			}
		}
	}
}

//-1`1‚É•ÏŠ·‚µ‚Ä”½“]
float SoundController::reverce_data(float data){
	float result = -((change_max_ - 0.5f) * 2.0f);
	return (result * 0.5f) + 0.5f;
}

bool SoundController::volume_chage(float delta_time){
	current_volume_ += change_late_ * delta_time;
	switch (sound_type_) {
	case SoundID::SID_BGM:
	case SoundID::SID_BGM_Change:
		Sound::set_bgm_volume(option_volume_ * current_volume_);
		break;
	case SoundID::SID_SE:
		Sound::set_se_volume(option_volume_ * current_volume_);
		break;
	default:
		break;
	}
	if ((is_dec_ && current_volume_ <= change_max_) || (!is_dec_ && current_volume_ >= change_max_))return true;
	return false;
}