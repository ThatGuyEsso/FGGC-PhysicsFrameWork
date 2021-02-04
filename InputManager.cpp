#include "InputManager.h"

bool InputManager::GetKey(int key)
{
	// checks If key is currently free
	if (GetAsyncKeyState(key) & 0x0001) {
		return true;
	}
	else {
		return false;
	}
}

bool InputManager::GetKeyIsDown(int key)
{
	if(GetAsyncKeyState(key) & 0x8000) {

		
		return true;
	}
	return false;
}

void InputManager::Update()
{
	GetKeyboardState(keys);
	
}