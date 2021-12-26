#include "Utils/WinInclude.hpp"

#include "Console.hpp"
#include "SMReaders/TileReader.hpp"
#include "ObjectDatabase/DatabaseConfig.hpp"
#include "ObjectDatabase/ObjectDatabase.hpp"

#include <locale>

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	std::setlocale(LC_CTYPE, "en_US.UTF-8");
	CreateDebugConsole(L"World Converter Debug Console");

	DatabaseConfig::ReadConfig(L"./Resources/Config.json");
	DatabaseLoader::LoadDatabase();

	Tile* output_tile = TileReader::ReadTile(L"./Tests/Kiosk_64_01.tile");
	output_tile->WriteToFile(L"./OutputTile.obj");
	output_tile->WriteMtlFile(L"./OutputTile.mtl");

	while (true) {};

	return 0;
}