#include "ObjectDatabase/ObjectDatabase.hpp"
#include "ObjectDatabase/DatabaseConfig.hpp"
#include "ObjectDatabase/ProgCounter.hpp"

#include "ObjectDatabase/Mod/Mod.hpp"
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
	DebugOutL(ConCol::BLUE, "Loading game data...");

	Mod* vanilla_items = new Mod(L"Vanilla Data", DatabaseConfig::GamePath, SMUuid::Null(), ModType::GameData);

	for (const std::wstring& db_dir : DatabaseConfig::AssetListFolders)
	{
		std::error_code rError;

		if (fs::is_regular_file(db_dir, rError))
		{
			if (rError) continue;

			vanilla_items->LoadFile(db_dir);
		}
		else
		{
			vanilla_items->ScanFolder(db_dir);
		}
	}

	Mod::ModStorage.insert(std::make_pair(vanilla_items->Uuid, vanilla_items));
}

void DatabaseLoader::LoadModDatabase()
{
	ProgCounter::SetState(ProgState::LoadingModded);
	DebugOutL(ConCol::GREEN, "Loading mod data...");

	for (const std::wstring& mod_dir : DatabaseConfig::ModFolders)
	{
		std::error_code rError;
		fs::directory_iterator rDirIter(mod_dir, fs::directory_options::skip_permission_denied, rError);

		for (const auto& dir : rDirIter)
		{
			if (rError || !dir.is_directory()) continue;

			Mod* new_mod = Mod::LoadFromDescription(dir.path().wstring());
			if (!new_mod) continue;

			new_mod->LoadObjects();
		}
	}
}

void DatabaseLoader::LoadDatabase()
{
	Mod::ClearModStorage();

	DatabaseLoader::LoadGameDatabase();
	DatabaseLoader::LoadModDatabase();

	DebugOutL(ConCol::GREEN_INT, "Finished! (Blocks: ", Mod::BlockStorage.size(), ", Parts: ", Mod::PartStorage.size(), ", Harvestables: ", Mod::HarvestableStorage.size(), ", Assets: ", Mod::AssetStorage.size(), ")");
}

void DatabaseLoader::InitializeDatabase()
{
	DatabaseConfig::ReadConfig();
	MaterialManager::Initialize();
	GroundTextureDatabase::Initialize();
	Rotations::InitializeRotations();
}