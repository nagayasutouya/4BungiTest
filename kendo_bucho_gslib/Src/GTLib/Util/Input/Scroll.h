#ifndef SCROLL_H_
#define SCROLL_H_

namespace DefaultScrollSpeed {
	static const float wait_max_=30.0f;
	static const float dead_zone_=0.1f;
};

class Scroll {
public:
	Scroll(){}
	bool select_change(int &current,int change, float delta_time, int max, float wait_max);
	bool select_change_inf(int& current, int change, float delta_time, int max, float wait_max);
	bool wait_and_scroll(bool change,float delta_time,float wait_max);//待機スクロールだけ
	bool wait(bool change, float delta_time, float wait_max);//待機だけ
	bool wait_dec(bool change, float delta_time, float wait_max,float dec_speed);//待機時間徐々に減る
private:
	bool reset();
	bool set_scroll(float delta_time,int wait_max);
private:
	float down_speed_{ 0.0f };//長押しで下に移動するスピード
	float down_wait_{ 0.0f };//長押しの待機時間
	bool first_cont_flag_{ true };//最初の一回の入力か
	float before_change_{ 0.0f };//前回の変化
};

#endif