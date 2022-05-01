#include "Console.hpp"

#ifdef WC_ENABLE_DEBUG_CONSOLE

#define CONSOLE_NUMBER_OUTPUT_DEFINITION(type) void DebugConsole::Output(const type obj) { DebugConsole::Output(std::to_string(obj)); }

__ConsoleOutputHandler DebugConsole::Out = __ConsoleOutputHandler();
HANDLE DebugConsole::Handle = NULL;

void DebugConsole::Output(const char* obj)
{
	WriteConsoleA(DebugConsole::Handle, obj, (DWORD)strlen(obj), 0, 0);
}

void DebugConsole::Output(const wchar_t* obj)
{
	WriteConsoleW(DebugConsole::Handle, obj, (DWORD)wcslen(obj), 0, 0);
}

void DebugConsole::Output(const std::string& obj)
{
	DebugConsole::Output(obj.c_str());
}

void DebugConsole::Output(const std::wstring& obj)
{
	DebugConsole::Output(obj.c_str());
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

void DebugConsole::Output(const ConColor& obj)
{
	SetConsoleTextAttribute(DebugConsole::Handle, static_cast<WORD>(obj));
}

bool DebugConsole::Create(const wchar_t* title)
{
	if (DebugConsole::Handle != NULL) return false;

	BOOL is_created = AllocConsole();
	if (!is_created) return false;

	SetConsoleOutputCP(CP_UTF8);
	SetConsoleTitleW(title);

	DebugConsole::Handle = GetStdHandle(STD_OUTPUT_HANDLE);

	return true;
}

void DebugConsole::Destroy()
{
	if (DebugConsole::Handle == NULL) return;

	FreeConsole();
	DebugConsole::Handle = NULL;
}

#endif