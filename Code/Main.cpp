#include "Utils/WinInclude.hpp"

#include "Console.hpp"
#include "SMReaders/TileReader.hpp"
#include "ObjectDatabase/DatabaseConfig.hpp"
#include "ObjectDatabase/ObjectDatabase.hpp"
#include "ObjectDatabase/Mod/ObjectRotations.hpp"

#include <locale>

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	std::setlocale(LC_CTYPE, "en_US.UTF-8");
	CreateDebugConsole(L"World Converter Debug Console");

	Rotations::InitializeRotations();

	DatabaseConfig::ReadConfig(L"./Resources/Config.json");
	DatabaseLoader::LoadDatabase();

	Tile* output_tile = TileReader::ReadTile(L"./Tests/Clutter Test2.tile");
	output_tile->WriteToFile(L"./OutputTile.obj");

	while (true) {};

	return 0;
}