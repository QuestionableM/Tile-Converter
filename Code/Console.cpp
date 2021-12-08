#ifdef _DEBUG
#include "Console.hpp"

#define CONSOLE_NUMBER_OUTPUT_DEFINITION(type) void Console::Output(const type obj) { Console::Output(std::to_string(obj)); }

__ConsoleOutputHandler Console::Out = __ConsoleOutputHandler();
HANDLE Console::ConHandle = NULL;

void Console::Output(const char* obj)
{
	WriteConsoleA(Console::ConHandle, obj, strlen(obj), 0, 0);
}

void Console::Output(const wchar_t* obj)
{
	WriteConsoleW(Console::ConHandle, obj, wcslen(obj), 0, 0);
}

void Console::Output(const std::string& obj)
{
	Console::Output(obj.c_str());
}

void Console::Output(const std::wstring& obj)
{
	Console::Output(obj.c_str());
}

CONSOLE_NUMBER_OUTPUT_DEFINITION(unsigned char&)
CONSOLE_NUMBER_OUTPUT_DEFINITION(char&)
CONSOLE_NUMBER_OUTPUT_DEFINITION(unsigned short&)
CONSOLE_NUMBER_OUTPUT_DEFINITION(short&)
CONSOLE_NUMBER_OUTPUT_DEFINITION(unsigned int&)
CONSOLE_NUMBER_OUTPUT_DEFINITION(int&)
CONSOLE_NUMBER_OUTPUT_DEFINITION(unsigned long&)
CONSOLE_NUMBER_OUTPUT_DEFINITION(long&)
CONSOLE_NUMBER_OUTPUT_DEFINITION(unsigned long long&)
CONSOLE_NUMBER_OUTPUT_DEFINITION(long long&)

CONSOLE_NUMBER_OUTPUT_DEFINITION(float&)
CONSOLE_NUMBER_OUTPUT_DEFINITION(double&)

void Console::Output(const ConCol& obj)
{
	SetConsoleTextAttribute(Console::ConHandle, static_cast<DWORD>(obj));
}

#endif