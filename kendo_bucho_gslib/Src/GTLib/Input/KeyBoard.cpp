#include"KeyBoard.h"
#include<commctrl.h>
#include<vector>

#pragma comment(lib,"comctl32.lib")

static constexpr UINT_PTR  SUBCLASS_ID = 1;//サブクラスID　クラスそのものに持たせるID

KeyBoard::KeyBoard(HWND hwnd) :hwnd_{hwnd} {
	RAWINPUTDEVICE rid{};
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x06;
	rid.dwFlags = RIDEV_INPUTSINK;
	rid.hwndTarget = hwnd_;
	RegisterRawInputDevices(&rid, 1, sizeof(rid));
	//メインとなるウィンドウから制御権を一時的に奪える
	SetWindowSubclass(hwnd_,subclass_proc,SUBCLASS_ID,reinterpret_cast<DWORD_PTR>(this));//ウィンドウをサブクラスに設定
}

KeyBoard::~KeyBoard(){
	RemoveWindowSubclass(hwnd_, subclass_proc, SUBCLASS_ID);
}

void KeyBoard::update(){
	previous_keys_ = current_keys_;
	current_keys_ = keys_;
}

//押してる間
bool KeyBoard::get_key(UINT key_code) {
	return current_keys_[key_code];
}

//押された瞬間
bool KeyBoard::get_key_down(UINT key_code) {
	return current_keys_[key_code] && !previous_keys_[key_code];
}

//離した瞬間
bool KeyBoard::get_key_up(UINT key_code) {
	return current_keys_[key_code] && previous_keys_[key_code];
}

LRESULT CALLBACK KeyBoard::subclass_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR id, DWORD_PTR ref_data){
	KeyBoard* self = reinterpret_cast<KeyBoard*>(ref_data);
	if (msg == WM_INPUT) {
		self->process_raw_input(reinterpret_cast<HRAWINPUT>(lp));
	}
	return DefSubclassProc(hwnd,msg,wp,lp);
}

void KeyBoard::process_raw_input(HRAWINPUT raw_input){
	UINT size = 0;
	GetRawInputData(raw_input, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));
	if (size == 0)return;
	if (raw_input_buffer_.size() < size) {
		raw_input_buffer_.resize(size);
	}
	if (GetRawInputData(raw_input, RID_INPUT, raw_input_buffer_.data(), &size, sizeof(RAWINPUTHEADER)) != size) {
		return;
	}
	RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(raw_input_buffer_.data());
	if (raw->header.dwType == RIM_TYPEKEYBOARD) {
		USHORT vkey = raw->data.keyboard.VKey;
		USHORT flags = raw->data.keyboard.Flags;
		if (vkey < keys_.size()) {
			if (flags & RI_KEY_BREAK) {
				keys_[vkey] = false;
			}
			else {
				keys_[vkey] = true;
			}
		}
	}
}