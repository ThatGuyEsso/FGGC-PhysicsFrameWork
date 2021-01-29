#pragma
#include <Windows.h>
#include <WinUser.h>
class InputManager
{
	
public:
	static InputManager& instance()
	{
		static InputManager* instance = new InputManager();
		return *instance;
	}

public:
	bool GetKey(int key);
	bool GetKeyIsDown(int key);


	void Update();

private:
	InputManager() {}

private:
	byte keys[256];


};