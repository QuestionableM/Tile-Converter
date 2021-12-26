#include "ObjectDatabase/ObjectDatabase.hpp"
#include "ObjectDatabase/DatabaseConfig.hpp"
#include "ObjectDatabase/Mod/Mod.hpp"

#include "Utils/Json.hpp"
#include "Console.hpp"

#include <filesystem>
namespace fs = std::filesystem;

void DatabaseLoader::LoadGameDatabase()
{
	Mod* vanilla_items = new Mod(L"Vanilla Data", DatabaseConfig::GamePath, SMUuid::Null(), ModType::GameData);

	for (const std::wstring& db_dir : DatabaseConfig::AssetListFolders)
		vanilla_items->ScanFolder(db_dir);

	Mod::ModStorage.insert(std::make_pair(vanilla_items->Uuid, vanilla_items));
}

void DatabaseLoader::LoadModDatabase()
{
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
	DatabaseLoader::LoadGameDatabase();
	DatabaseLoader::LoadModDatabase();
}