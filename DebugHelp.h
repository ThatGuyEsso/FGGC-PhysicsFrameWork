#pragma once
#include <string>
#include <Windows.h>
#include <iostream>

static class DebugHelp {


public:

	static void OutPutValue(const char* outPutText, static float value);
	static void OutPutValue(const char* outPutText, static int value);
	static void OutPutText(const char* outPutText);
};