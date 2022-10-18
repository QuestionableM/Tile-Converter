#include "TerrainAssetsMod.hpp"

#include "ObjectDatabase\KeywordReplacer.hpp"

#include "Utils\String.hpp"
#include "Utils\File.hpp"
#include "Console.hpp"

static const std::wstring g_AssetSetDirectoryPaths[2] = { L"/Terrain/Database/", L"/Database/" };
bool TerrainAssetsMod::GetValidAssetDatabaseFolder(const std::wstring& mod_path, std::wstring& r_asset_db)
{
	for (__int8 a = 0; a < 2; a++)
	{
		const std::wstring asset_db_path = mod_path + g_AssetSetDirectoryPaths[a];

		if (File::Exists(asset_db_path))
		{
			r_asset_db = asset_db_path;
			return true;
		}
	}

	return false;
}

static const std::wstring g_AssetSetDbExtensions[2] = { L"json", L"assetdb" };
bool TerrainAssetsMod::GetAssetSetDatabaseFile(const std::wstring& asset_db_dir, std::wstring& r_asset_set)
{
	const std::wstring near_full_path = asset_db_dir + L"assetsets.";

	for (__int8 a = 0; a < 2; a++)
	{
		const std::wstring full_db_path = near_full_path + g_AssetSetDbExtensions[a];

		if (File::Exists(full_db_path))
		{
			r_asset_set = full_db_path;
			return true;
		}
	}

	return false;
}

void TerrainAssetsMod::LoadAssetSetDatabase(const std::wstring& path, Mod* pMod)
{
	const nlohmann::json assetset_db_json = JsonReader::LoadParseJson(path);
	if (!assetset_db_json.is_object()) return;

	const auto& asset_set_list = JsonReader::Get(assetset_db_json, "assetSetList");
	if (!asset_set_list.is_array()) return;

	for (const auto& cur_asset_set : asset_set_list)
	{
		if (!cur_asset_set.is_object()) continue;

		const auto& asset_set_str = JsonReader::Get(cur_asset_set, "assetSet");
		if (!asset_set_str.is_string()) continue;

		const std::wstring asset_set_wide = String::ToWide(asset_set_str.get<std::string>());
		const std::wstring asset_set_path = KeywordReplacer::ReplaceKey(asset_set_wide);

		pMod->LoadFile(asset_set_path);
	}
}

void TerrainAssetsMod::LoadObjectDatabase()
{
	KeywordReplacer::SetModData(m_Directory, m_Uuid);

	std::wstring l_AssetDatabaseDir;
	if (!TerrainAssetsMod::GetValidAssetDatabaseFolder(m_Directory, l_AssetDatabaseDir))
	{
		DebugErrorL("Couldn't find a valid asset database directory!");
		return;
	}

	std::wstring l_AssetSetDatabasePath;
	if (TerrainAssetsMod::GetAssetSetDatabaseFile(l_AssetDatabaseDir, l_AssetSetDatabasePath))
	{
		TerrainAssetsMod::LoadAssetSetDatabase(l_AssetSetDatabasePath, this);
	}
	else
	{
		const std::wstring l_AssetSetsFolder = l_AssetDatabaseDir + L"AssetSets";
		if (!File::Exists(l_AssetSetsFolder)) return;

		this->ScanDatabaseFolder(l_AssetSetsFolder);
	}
}