#include "Utils/WinInclude.hpp"

#include "Console.hpp"
#include "SMReaders/TileReader.hpp"
#include "ObjectDatabase/DatabaseConfig.hpp"
#include "ObjectDatabase/ObjectDatabase.hpp"

#include <locale>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	std::setlocale(LC_CTYPE, "en_US.UTF-8");
	CreateDebugConsole(L"World Converter Debug Console");

	DatabaseConfig::ReadConfig(L"./Resources/Config.json");
	DatabaseLoader::LoadDatabase();

	Tile* output_tile = TileReader::ReadTile(L"./Tests/Mountain Circle Line.tile");
	output_tile->WriteToFile(L"./OutputTile.obj");

	while (true) {};

	return 0;
}