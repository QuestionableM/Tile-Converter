#include "Mod.hpp"

#include "Utils/File.hpp"
#include "Utils/String.hpp"

#include "ObjectDatabase\Mod\TerrainAssetsMod.h"
#include "ObjectDatabase\Mod\BlocksAndPartsMod.h"
#include "ObjectDatabase/Mod/AssetListLoader.hpp"
#include "ObjectDatabase/Mod/HarvestableListLoader.hpp"
#include "ObjectDatabase/Mod/PartListLoader.hpp"
#include "ObjectDatabase/Mod/BlockListLoader.hpp"
#include "ObjectDatabase/Mod/ClutterListLoader.hpp"
#include "ObjectDatabase/KeywordReplacer.hpp"

#include "Console.hpp"

#include <filesystem>
namespace fs = std::filesystem;

Mod::~Mod()
{
	for (const auto& pBlock : m_Blocks)
		delete pBlock.second;

	for (const auto& pPart : m_Parts)
		delete pPart.second;

	for (const auto& pAsset : m_Assets)
		delete pAsset.second;

	for (const auto& pHarvestable : m_Harvestables)
		delete pHarvestable.second;

	for (const auto& pClutter : m_Clutter)
		delete pClutter.second;
}

void Mod::ClearModStorage()
{
	Mod::BlockStorage.clear();
	Mod::PartStorage.clear();
	Mod::AssetStorage.clear();
	Mod::HarvestableStorage.clear();
	Mod::ClutterStorage.clear();
	Mod::ClutterVector.clear();

	for (std::size_t a = 0; a < Mod::ModVector.size(); a++)
		delete Mod::ModVector[a];

	Mod::ModStorage.clear();
	Mod::ModVector.clear();
}

Mod* Mod::LoadFromDescription(const std::wstring& mod_folder, const bool& is_local)
{
	const std::wstring mDescPath = mod_folder + L"/description.json";
	if (!File::Exists(mDescPath)) return nullptr;

	nlohmann::json mDesc = JsonReader::LoadParseJson(mDescPath);
	if (!mDesc.is_object()) return nullptr;

	const auto& mType = JsonReader::Get(mDesc, "type");
	if (!mType.is_string()) return nullptr;

	const std::string mTypeStr = mType.get<std::string>();

	const auto& mUuid = JsonReader::Get(mDesc, "localId");
	const auto& mName = JsonReader::Get(mDesc, "name");

	if (!mUuid.is_string() || !mName.is_string()) return nullptr;

	const SMUuid v_modUuid = mUuid.get<std::string>();
	const std::wstring v_modName = String::ToWide(mName.get<std::string>());

	const ModMap::const_iterator v_iter = ModStorage.find(v_modUuid);
	if (v_iter != ModStorage.end())
	{
		const Mod* v_modPtr = v_iter->second;

		if (v_iter->second->m_isLocal && !is_local)
		{
			DebugOutL("Skipped a mod as it was already loaded locally (name: ", v_modName, ", uuid: ", v_modUuid.ToString(), ")");
			return nullptr;
		}

		if (v_iter->second->m_isLocal == is_local)
		{
			DebugWarningL("Uuid conflict between: ", v_iter->second->m_Name, " and ", v_modName, " (uuid: ", v_modUuid.ToString(), ")");
			return nullptr;
		}
	}

	Mod* v_newMod;
	if (mTypeStr == "Blocks and Parts")
		v_newMod = new BlocksAndPartsMod();
	else if (mTypeStr == "Terrain Assets")
		v_newMod = new TerrainAssetsMod();
	else
		return nullptr;

	v_newMod->m_Name = v_modName;
	v_newMod->m_Directory = mod_folder;
	v_newMod->m_Uuid = v_modUuid;
	v_newMod->m_isLocal = is_local;

	DebugOutL("Mod: ", 0b1101_fg, v_newMod->m_Name, 0b1110_fg, ", Uuid: ", 0b1101_fg, v_newMod->m_Uuid.ToString(), 0b1110_fg, ", Type: ", 0b1101_fg, mTypeStr);
	Mod::ModStorage.insert(std::make_pair(v_newMod->m_Uuid, v_newMod));
	Mod::ModVector.push_back(v_newMod);

	return v_newMod;
}

BlockData* Mod::GetGlobalBlock(const SMUuid& uuid)
{
	if (BlockStorage.find(uuid) != BlockStorage.end())
		return BlockStorage.at(uuid);

	DebugErrorL("Couldn't find a block with the specified uuid: ", uuid.ToString());
	return nullptr;
}

PartData* Mod::GetGlobalPart(const SMUuid& uuid)
{
	if (PartStorage.find(uuid) != PartStorage.end())
		return PartStorage.at(uuid);

	DebugErrorL("Couldn't find a part with the specified uuid: ", uuid.ToString());
	return nullptr;
}

AssetData* Mod::GetGlobalAsset(const SMUuid& uuid)
{
	if (AssetStorage.find(uuid) != AssetStorage.end())
		return AssetStorage.at(uuid);

	DebugErrorL("Couldn't find an asset with the specified uuid: ", uuid.ToString());
	return nullptr;
}

HarvestableData* Mod::GetGlobalHarvestbale(const SMUuid& uuid)
{
	if (HarvestableStorage.find(uuid) != HarvestableStorage.end())
		return HarvestableStorage.at(uuid);

	DebugErrorL("Couldn't find a harvestable with the specified uuid: ", uuid.ToString());
	return nullptr;
}

ClutterData* Mod::GetGlobalClutter(const SMUuid& uuid)
{
	if (ClutterStorage.find(uuid) != ClutterStorage.end())
		return ClutterStorage.at(uuid);

	DebugErrorL("Couldn't find clutter with the specified uuid: ", uuid.ToString());
	return nullptr;
}

ClutterData* Mod::GetGlobalClutterById(const std::size_t& idx)
{
	if (Mod::ClutterVector.size() <= idx)
	{
		DebugErrorL("The clutter index is out of bounds! (Size: ", Mod::ClutterVector.size(), ", Index: ", idx, ")");
		return nullptr;
	}

	return Mod::ClutterVector[idx];
}

std::size_t Mod::GetAmountOfObjects()
{
	return (BlockStorage.size() + PartStorage.size() + AssetStorage.size() + HarvestableStorage.size() + ClutterStorage.size());
}

std::size_t Mod::GetAmountOfMods()
{
	return Mod::ModVector.size();
}

static const std::unordered_map<std::string, void (*)(const nlohmann::json&, Mod*)> g_DataLoaders =
{
	{ "assetListRenderable", AssetListLoader::Load       },
	{ "harvestableList",     HarvestableListLoader::Load },
	{ "partList",			 PartListLoader::Load		 },
	{ "blockList",			 BlockListLoader::Load		 },
	{ "clutterList",		 ClutterListLoader::Load     }
};

void Mod::LoadFile(const std::wstring& path)
{
	nlohmann::json file_json = JsonReader::LoadParseJson(path);
	if (!file_json.is_object()) return;

	DebugOutL("Loading: ", path);

	for (const auto& fKey : file_json.items())
	{
		const std::string key_str = fKey.key();

		if (g_DataLoaders.find(key_str) != g_DataLoaders.end())
		{
			g_DataLoaders.at(key_str)(fKey.value(), this);
		}
		else
		{
			DebugErrorL("Couldn't find the loader for: ", key_str);
		}
	}
}

bool IsShapeSetExtensionValid(const std::string& extension)
{
	if (extension == ".json") return true;
	if (extension == ".shapeset") return true;
	if (extension == ".assetset") return true;

	return false;
}

void Mod::ScanDatabaseFolderRecursive(const std::wstring& folder)
{
	std::error_code rError;
	fs::recursive_directory_iterator rDirIter(folder, fs::directory_options::skip_permission_denied, rError);

	for (const auto& dir : rDirIter)
	{
		if (rError || !dir.is_regular_file()) continue;

		const fs::path& dPath = dir.path();

		if (dPath.has_extension() && IsShapeSetExtensionValid(dPath.extension().string()))
			this->LoadFile(dPath.wstring());
	}
}

void Mod::ScanDatabaseFolder(const std::wstring& folder)
{
	std::error_code rError;
	fs::directory_iterator rDirIter(folder, fs::directory_options::skip_permission_denied, rError);

	for (const auto& cur_dir : rDirIter)
	{
		if (rError || !cur_dir.is_regular_file()) continue;

		const fs::path& dir_path = cur_dir.path();

		if (dir_path.has_extension() && IsShapeSetExtensionValid(dir_path.extension().string()))
			this->LoadFile(dir_path.wstring());
	}
}