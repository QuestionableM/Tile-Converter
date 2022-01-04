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

	/*Blueprint* new_blueprint = Blueprint::FromFile(L"./Tests/Car.json");
	if (new_blueprint != nullptr)
	{
		new_blueprint->SetSize({ 0.25f, 0.25f, 0.25f });

		std::ofstream output_bp("./OutputBlueprint.obj");
		if (output_bp.is_open())
		{
			WriterOffsetData new_offset = { 0, 0, 0, 0 };

			new_blueprint->WriteObjectToFile(output_bp, new_offset, glm::mat4(1.0f));

			output_bp.close();
		}
	}*/
	Tile* output_tile = TileReader::ReadTile(L"./Tests/Kiosk_64_01.tile");
	output_tile->WriteToFile(L"./OutputTile.obj");

	while (true) {};

	return 0;
}