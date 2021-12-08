#pragma once

#ifdef _DEBUG
#include <Windows.h>
#include <string>
#include <vector>

enum class ConCol : WORD
{
	RED    = FOREGROUND_RED,
	GREEN  = FOREGROUND_GREEN,
	BLUE   = FOREGROUND_BLUE,
	YELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
	PINK   = FOREGROUND_RED | FOREGROUND_BLUE,
	WHITE  = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,

	RED_INT    = FOREGROUND_RED   | FOREGROUND_INTENSITY,
	GREEN_INT  = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	BLUE_INT   = FOREGROUND_BLUE  | FOREGROUND_INTENSITY,
	YELLOW_INT = FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	PINK_INT   = FOREGROUND_RED   | FOREGROUND_BLUE  | FOREGROUND_INTENSITY,
	WHITE_INT  = FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,

	RED_BG    = BACKGROUND_RED,
	GREEN_BG  = BACKGROUND_GREEN,
	BLUE_BG   = BACKGROUND_BLUE,
	YELLOW_BG = BACKGROUND_RED | BACKGROUND_GREEN,
	PINK_BG   = BACKGROUND_RED | BACKGROUND_BLUE,
	WHITE_BG  = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,

	RED_BG_INT    = BACKGROUND_RED   | BACKGROUND_INTENSITY,
	GREEN_BG_INT  = BACKGROUND_GREEN | BACKGROUND_INTENSITY,
	BLUE_BG_INT   = BACKGROUND_BLUE  | BACKGROUND_INTENSITY,
	YELLOW_BG_INT = BACKGROUND_RED   | BACKGROUND_GREEN | BACKGROUND_INTENSITY,
	PINK_BG_INT   = BACKGROUND_RED   | BACKGROUND_BLUE  | BACKGROUND_INTENSITY,
	WHITE_BG_INT  = BACKGROUND_RED   | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY
};

class __ConsoleOutputHandler;

#define DECLARE_CONSOLE_OUTPUT(type_name) static void Output(const type_name obj)

class Console
{
	friend __ConsoleOutputHandler;
	static HANDLE ConHandle;

	DECLARE_CONSOLE_OUTPUT(char*);
	DECLARE_CONSOLE_OUTPUT(wchar_t*);

	DECLARE_CONSOLE_OUTPUT(std::string&);
	DECLARE_CONSOLE_OUTPUT(std::wstring&);

	DECLARE_CONSOLE_OUTPUT(unsigned char&);
	DECLARE_CONSOLE_OUTPUT(char&);
	DECLARE_CONSOLE_OUTPUT(unsigned short&);
	DECLARE_CONSOLE_OUTPUT(short&);
	DECLARE_CONSOLE_OUTPUT(unsigned int&);
	DECLARE_CONSOLE_OUTPUT(int&);
	DECLARE_CONSOLE_OUTPUT(unsigned long&);
	DECLARE_CONSOLE_OUTPUT(long&);
	DECLARE_CONSOLE_OUTPUT(unsigned long long&);
	DECLARE_CONSOLE_OUTPUT(long long&);

	DECLARE_CONSOLE_OUTPUT(float&);
	DECLARE_CONSOLE_OUTPUT(double&);

	DECLARE_CONSOLE_OUTPUT(ConCol&);

	template<typename ArrayObject>
	static inline void Output(const std::vector<ArrayObject>& obj)
	{
		Console::Output("{ ");
		for (std::size_t a = 0; a < obj.size(); a++)
		{
			if (a > 0) Console::Output(", ");

			Console::Output(obj[a]);
		}
		Console::Output(" }");
	}

public:

	static inline bool Create(const wchar_t* title)
	{
		if (Console::ConHandle != NULL) return false;

		BOOL is_created = AllocConsole();
		if (!is_created) return false;

		SetConsoleOutputCP(CP_UTF8);
		SetConsoleTitleW(title);

		Console::ConHandle = GetStdHandle(STD_OUTPUT_HANDLE);

		return true;
	}

	static inline void Destroy()
	{
		if (Console::ConHandle == NULL) return;

		FreeConsole();
		Console::ConHandle = NULL;
	}

	static __ConsoleOutputHandler Out;
};

class __ConsoleOutputHandler
{
	friend Console;

	template<typename Arg>
	inline void variadic_func(const Arg& arg)
	{
		Console::Output(arg);
	}

	template<typename Arg, typename ...ArgList>
	inline void variadic_func(const Arg& arg, const ArgList& ...argList)
	{
		this->variadic_func(arg);
		this->variadic_func(argList...);
	}

public:
	template<typename ...ArgList>
	inline void operator()(const ArgList& ...argList)
	{
		this->variadic_func(argList...);
	}

	__ConsoleOutputHandler() = default;
	~__ConsoleOutputHandler() = default;
};

#define CreateDebugConsole(ConName) Console::Create(ConName)
#define DebugOutL(...) Console::Out(__VA_ARGS__, "\n")
#else
#define CreateDebugConsole(ConName) ((void*)0)
#define DebugOutL(...) ((void*)0)
#endif