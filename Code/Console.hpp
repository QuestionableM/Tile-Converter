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

inline constexpr ConColor operator"" _bg(unsigned long long val) noexcept
{
	ConColor lOutput;

	if ((val & 0b1000) == 0b1000) lOutput.color_data |= BACKGROUND_RED;
	if ((val & 0b0100) == 0b0100) lOutput.color_data |= BACKGROUND_GREEN;
	if ((val & 0b0010) == 0b0010) lOutput.color_data |= BACKGROUND_BLUE;
	if ((val & 0b0001) == 0b0001) lOutput.color_data |= BACKGROUND_INTENSITY;

	return lOutput;
}

inline constexpr ConColor operator"" _fg(unsigned long long val) noexcept
{
	ConColor lOutput;

	if ((val & 0b1000) == 0b1000) lOutput.color_data |= FOREGROUND_RED;
	if ((val & 0b0100) == 0b0100) lOutput.color_data |= FOREGROUND_GREEN;
	if ((val & 0b0010) == 0b0010) lOutput.color_data |= FOREGROUND_BLUE;
	if ((val & 0b0001) == 0b0001) lOutput.color_data |= FOREGROUND_INTENSITY;

	return lOutput;
}

class __ConsoleOutputHandler;

#define DECLARE_INTEGER_OUTPUT(type) inline static void Output(const type number) { DebugConsole::Output(std::to_string(number)); }

class DebugConsole
{
	friend __ConsoleOutputHandler;
	static HANDLE Handle;

	inline static void Output(const char* c_str)
	{
		WriteConsoleA(DebugConsole::Handle, c_str, static_cast<DWORD>(strlen(c_str)), NULL, NULL);
	}

	inline static void Output(const wchar_t* wc_str)
	{
		WriteConsoleW(DebugConsole::Handle, wc_str, static_cast<DWORD>(wcslen(wc_str)), NULL, NULL);
	}

	inline static void Output(const std::string& str)
	{
		WriteConsoleA(DebugConsole::Handle, str.data(), static_cast<DWORD>(str.size()), NULL, NULL);
	}

	inline static void Output(const std::wstring& wstr)
	{
		WriteConsoleW(DebugConsole::Handle, wstr.data(), static_cast<DWORD>(wstr.size()), NULL, NULL);
	}

	DECLARE_INTEGER_OUTPUT(unsigned char&);
	DECLARE_INTEGER_OUTPUT(char&);
	DECLARE_INTEGER_OUTPUT(unsigned short&);
	DECLARE_INTEGER_OUTPUT(short&);
	DECLARE_INTEGER_OUTPUT(unsigned int&);
	DECLARE_INTEGER_OUTPUT(int&);
	DECLARE_INTEGER_OUTPUT(unsigned long&);
	DECLARE_INTEGER_OUTPUT(long&);
	DECLARE_INTEGER_OUTPUT(unsigned long long&);
	DECLARE_INTEGER_OUTPUT(long long&);

	DECLARE_INTEGER_OUTPUT(float&);
	DECLARE_INTEGER_OUTPUT(double&);

	inline static void Output(const ConColor& con_color)
	{
		SetConsoleTextAttribute(DebugConsole::Handle, static_cast<WORD>(con_color));
	}

	template<typename ArrayObject>
	static inline void Output(const std::vector<ArrayObject>& obj)
	{
		DebugConsole::Output("{ ");
		if (!obj.empty())
		{
			DebugConsole::Output(obj[0]);
			for (std::size_t a = 1; a < obj.size(); a++)
			{
				DebugConsole::Output(", ");
				DebugConsole::Output(obj[a]);
			}
		}
		DebugConsole::Output(" }");
	}

public:
	inline static bool Create(const wchar_t* title)
	{
		if (DebugConsole::Handle != NULL) return false;

		BOOL is_created = AllocConsole();
		if (!is_created) return false;

		SetConsoleOutputCP(CP_UTF8);
		SetConsoleTitleW(title);

		DebugConsole::Handle = GetStdHandle(STD_OUTPUT_HANDLE);

		return true;
	}

	inline static void Destroy()
	{
		if (DebugConsole::Handle == NULL) return;

		FreeConsole();
		DebugConsole::Handle = NULL;
	}

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

HANDLE DebugConsole::Handle = NULL;
__ConsoleOutputHandler DebugConsole::Out = __ConsoleOutputHandler();

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