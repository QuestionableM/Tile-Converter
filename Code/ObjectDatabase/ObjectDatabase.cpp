#include "ObjectDatabase/ObjectDatabase.hpp"
#include "ObjectDatabase/DatabaseConfig.hpp"
#include "ObjectDatabase/ProgCounter.hpp"

#include "ObjectDatabase/Mod/GameDataMod.h"
#include "ObjectDatabase/Mod/ObjectRotations.hpp"
#include "ObjectDatabase/Mod/MaterialManager.hpp"
#include "ObjectDatabase/GroundTextureDatabase.hpp"

#include "Utils/Json.hpp"
#include "Console.hpp"

#include <filesystem>
namespace fs = std::filesystem;

void DatabaseLoader::LoadGameDatabase()
{
	ProgCounter::SetState(ProgState::LoadingVanilla, 0);
	DebugOutL(0b0010_fg, "Loading game data...");

	Mod* pGameData = new GameDataMod();
	pGameData->LoadObjectDatabase();

	Mod::ModVector.push_back(pGameData);
}

void DatabaseLoader::LoadModDatabase()
{
	ProgCounter::SetState(ProgState::LoadingModded);
	DebugOutL(0b0100_fg, "Loading mod data...");

	for (const std::wstring& mod_dir : DatabaseConfig::ModFolders)
	{
		std::error_code rError;
		fs::directory_iterator lDirIterator(mod_dir, fs::directory_options::skip_permission_denied, rError);

		for (const auto& dir : lDirIterator)
		{
			if (rError || !dir.is_directory()) continue;

			Mod* pNewMod = Mod::LoadFromDescription(dir.path().wstring());
			if (!pNewMod) continue;

			pNewMod->LoadObjectDatabase();
		}
	}
}

void DatabaseLoader::LoadDatabase()
{
	Mod::ClearModStorage();

	DatabaseLoader::LoadGameDatabase();
	DatabaseLoader::LoadModDatabase();

	DebugOutL(0b0101_fg, "Finished! (Blocks: ", Mod::BlockStorage.size(), ", Parts: ", Mod::PartStorage.size(), ", Harvestables: ", Mod::HarvestableStorage.size(), ", Assets: ", Mod::AssetStorage.size(), ")");
}

void DatabaseLoader::InitializeDatabase()
{
	DatabaseConfig::ReadConfig();
	MaterialManager::Initialize();
	GroundTextureDatabase::Initialize();
	Rotations::InitializeRotations();
}