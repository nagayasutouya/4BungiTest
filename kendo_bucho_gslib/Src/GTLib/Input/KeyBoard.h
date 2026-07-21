#ifndef KEY_BOARD_H_
#define KEY_BOARD_H_

#include<Windows.h>
#include<array>
#include<vector>

class KeyBoard {
public:
	KeyBoard(HWND hwnd);
	~KeyBoard();
	void update();
	bool get_key(UINT key_code);//押してる間
	bool get_key_down(UINT key_code);//押された瞬間
	bool get_key_up(UINT key_code);//離した瞬間
	KeyBoard(const KeyBoard&) = default;
	KeyBoard& operator=(const KeyBoard&) = default;
private:
	static LRESULT CALLBACK subclass_proc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp,UINT_PTR id,DWORD_PTR ref_data);
	void process_raw_input(HRAWINPUT raw_input);
private:
	HWND hwnd_;//ウィンドウハンドル
	using KeyState = std::array < bool, 256 >;//全てのキーの状態
	KeyState keys_{};
	KeyState current_keys_{};
	KeyState previous_keys_{};
	std::vector<BYTE>raw_input_buffer_;
};

#endif