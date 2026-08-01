#ifndef INPUT_KEYBORD_H_
#define INPUT_KEYBORD_H_

#include "GTLib/Input/KeyBoard.h"
#include <vector>

class InputKeyBord {
public:
	InputKeyBord() {}
	~InputKeyBord() { printf("キーボードクラス削除\n"); delete key_bord_; }
	static InputKeyBord* Instance() {
		static InputKeyBord ikb;
		return &ikb;
	}
	void Initialize(HWND hwnd);
	void Update() { key_bord_->update(); }
	bool GetKey(unsigned int key_code) { return key_bord_->get_key(key_code); }//押してる間
	bool GetKeyDown(unsigned int key_code) { return key_bord_->get_key_down(key_code); }//押された瞬間
	bool GetKeyUp(unsigned int key_code) { return key_bord_->get_key_up(key_code); }//離した瞬間
	unsigned int GetWitchKey();//どのキー押されてるか
	std::vector<unsigned int> GetWitchKeys();//どのキー押されてるか
private:
	KeyBoard* key_bord_{nullptr};
};

#endif