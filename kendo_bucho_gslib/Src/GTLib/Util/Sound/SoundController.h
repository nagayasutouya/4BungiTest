#ifndef SOUND_CONTROLLER_H_
#define SOUND_CONTROLLER_H_

#include "GTLib/GTLib.h"
#include "SoundDefine.h"

//フェードインアウトなど、BGM SEの音量調節を実行するクラス
class SoundController {
public:
	SoundController(){}
	void sound_change(const float option_volume=1.0f, GTUINT id = 0, SoundID sound_type = SoundID::SID_None, const float change_late = 0.0f, const float change_max = 0.0f);
	void update_sc(float delta_time);
private:
	bool volume_chage(float delta_time);
	float reverce_data(float data);
private:
	GTUINT id_{ 0 };
	SoundID sound_type_{SoundID::SID_None};
	float change_late_{ 0.0f };
	float change_max_{ 0.0f };
	bool is_dec_{ false };
	float current_volume_{ 0.0f };
	float option_volume_{ 0.0f };
	bool is_dead_{ false };
};

#endif