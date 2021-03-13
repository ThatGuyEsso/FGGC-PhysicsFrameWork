#include "DebugHelp.h"

void DebugHelp::OutPutValue(const char* outPutText, static float value)
{
	std::string out = outPutText;
	//convert to a string
	std::string conversion = std::to_string(value);
	//Format value  
	out += " ";
	out += conversion;
	out += "\n";
	OutputDebugStringA(out.c_str());

}

void DebugHelp::OutPutValue(const char* outPutText, static int value)
{
	//get value
	std::string out = outPutText;
	//set string  
	out += " ";
	out += "%d";
	out += "\n";
	
	//format string
	char sz[1024] = { 0 };
	sprintf_s(sz, out.c_str(), value);
	OutputDebugStringA(sz);


}

void DebugHelp::OutPutValue(const char* outPutText, Vector3D value)
{
	//get value
	std::string out = outPutText;
	//set string  
	out += " ";
	out += "(";
	out += value.x;
	out += ",";
	out += value.y;
	out += ",";
	out += value.z;
	out += ")";
	out += "\n";

	//format string
	char sz[1024] = { 0 };
	sprintf_s(sz, out.c_str());
	OutputDebugStringA(sz);

}

void DebugHelp::OutPutText(const char* outPutText)
{
	std::string out = outPutText;
	out += "\n";//start new line
	OutputDebugStringA(out.c_str());
}
