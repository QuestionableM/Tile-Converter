#pragma once

#include "ProgramConfig.hpp"

#ifdef WC_ENABLE_DEBUG_CONSOLE
#include "Utils/WinInclude.hpp"
#include <string>
#include <vector>

class ConColor
{
	WORD color_data = 0;

	constexpr ConColor() noexcept = default;

	friend constexpr ConColor operator"" _bg(unsigned long long val) noexcept;
	friend constexpr ConColor operator"" _fg(unsigned long long val) noexcept;

public:
	inline constexpr operator WORD() const noexcept
	{
		return color_data;
	}

	inline constexpr ConColor operator|(const ConColor& rhs) noexcept
	{
		ConColor lOutput;
		lOutput.color_data = this->color_data | rhs.color_data;

		return lOutput;
	}
};

constexpr ConColor operator"" _bg(unsigned long long val) noexcept
{
	ConColor lOutput;

	if ((val & 0b1000) == 0b1000) lOutput.color_data |= BACKGROUND_RED;
	if ((val & 0b0100) == 0b0100) lOutput.color_data |= BACKGROUND_GREEN;
	if ((val & 0b0010) == 0b0010) lOutput.color_data |= BACKGROUND_BLUE;
	if ((val & 0b0001) == 0b0001) lOutput.color_data |= BACKGROUND_INTENSITY;

	return lOutput;
}

constexpr ConColor operator"" _fg(unsigned long long val) noexcept
{
	ConColor lOutput;

	if ((val & 0b1000) == 0b1000) lOutput.color_data |= FOREGROUND_RED;
	if ((val & 0b0100) == 0b0100) lOutput.color_data |= FOREGROUND_GREEN;
	if ((val & 0b0010) == 0b0010) lOutput.color_data |= FOREGROUND_BLUE;
	if ((val & 0b0001) == 0b0001) lOutput.color_data |= FOREGROUND_INTENSITY;

	return lOutput;
}

class __ConsoleOutputHandler;

#define DECLARE_CONSOLE_OUTPUT(type_name) static void Output(const type_name obj)

class DebugConsole
{
	friend __ConsoleOutputHandler;
	static HANDLE Handle;

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

	DECLARE_CONSOLE_OUTPUT(ConColor&);

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
	static bool Create(const wchar_t* title);
	static void Destroy();

	static __ConsoleOutputHandler Out;
};

class __ConsoleOutputHandler
{
	friend DebugConsole;

	template<typename Arg>
	inline void variadic_func(const Arg& arg)
	{
		DebugConsole::Output(arg);
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

#define CreateDebugConsole(ConName) DebugConsole::Create(ConName)
#define DebugOutL(...)              DebugConsole::Out(__VA_ARGS__, 0b1110_fg, "\n")
#define DebugOut(...)               DebugConsole::Out(__VA_ARGS__)
#define DebugErrorL(...)            DebugConsole::Out(0b1001_fg, "ERROR: "  , __FUNCTION__, " (", __LINE__, ") -> ", __VA_ARGS__, 0b1110_fg, "\n")
#define DebugWarningL(...)          DebugConsole::Out(0b1101_fg, "WARNING: ", __FUNCTION__, " (", __LINE__, ") -> ", __VA_ARGS__, 0b1110_fg, "\n")
#else
#define CreateDebugConsole(ConName) ((void*)0)
#define DebugOutL(...)              ((void*)0)
#define DebugOut(...)               ((void*)0)
#define DebugErrorL(...)            ((void*)0)
#define DebugWarningL(...)          ((void*)0)
#endif