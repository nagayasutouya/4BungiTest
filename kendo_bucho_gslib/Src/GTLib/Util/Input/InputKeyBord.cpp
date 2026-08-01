#include <windows.h>
#include<iostream>
#include"InputKeyBord.h"

void InputKeyBord::Initialize(HWND hwnd){
	key_bord_ = new KeyBoard(hwnd);
}

//どのキー押されてるか
unsigned int InputKeyBord::GetWitchKey() {
	unsigned int result = 0;
	return result;
}

//どのキー押されてるか
std::vector<unsigned int> InputKeyBord::GetWitchKeys() {
	std::vector<unsigned int> result;
	return result;
}