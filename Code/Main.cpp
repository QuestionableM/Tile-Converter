#include "Utils/WinInclude.hpp"
#include "Gui/MainGui.h"
#include "Console.hpp"

#include <locale>

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	std::setlocale(LC_CTYPE, "en_US.UTF-8");
	CreateDebugConsole(L"World Converter Debug Console");

	TileConverter::Application::EnableVisualStyles();
	TileConverter::Application::SetCompatibleTextRenderingDefault(false);
	TileConverter::Application::Run(gcnew TileConverter::MainGui);

	return 0;
}