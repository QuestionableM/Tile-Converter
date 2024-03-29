#include "Mod.hpp"

#include "ObjectDatabase\Readers\HarvestableListLoader.hpp"
#include "ObjectDatabase\Readers\DecalsetListReader.hpp"
#include "ObjectDatabase\Readers\ClutterListLoader.hpp"
#include "ObjectDatabase\Readers\AssetListLoader.hpp"
#include "ObjectDatabase\Readers\BlockListLoader.hpp"
#include "ObjectDatabase\Readers\PartListLoader.hpp"
#include "ObjectDatabase\KeywordReplacer.hpp"

#include "ObjectDatabase\Mods\BlocksAndPartsMod.hpp"
#include "ObjectDatabase\Mods\TerrainAssetsMod.hpp"

#include "Utils\String.hpp"
#include "Utils\File.hpp"

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

	for (const auto& pDecal : m_Decals)
		delete pDecal.second;
}

void Mod::ClearModStorage()
{
	Mod::BlockStorage.clear();
	Mod::PartStorage.clear();
	Mod::AssetStorage.clear();
	Mod::HarvestableStorage.clear();
	Mod::ClutterStorage.clear();
	Mod::ClutterVector.clear();
	Mod::DecalStorage.clear();

	for (std::size_t a = 0; a < Mod::ModVector.size(); a++)
		delete Mod::ModVector[a];

	Mod::ModStorage.clear();
	Mod::ModVector.clear();
}

Mod* Mod::LoadFromDescription(const std::wstring& mod_folder, const bool& is_local)
{
	const std::wstring mDescPath = mod_folder + L"/description.json";
	if (!File::Exists(mDescPath)) return nullptr;

	simdjson::dom::document v_doc;
	if (!JsonReader::LoadParseSimdjsonCommentsC(mDescPath, v_doc, simdjson::dom::element_type::OBJECT))
		return nullptr;

	const auto v_root = v_doc.root();

	const auto v_mod_type = v_root["type"];
	if (!v_mod_type.is_string()) return nullptr;

	const auto v_mod_uuid_obj = v_root["localId"];
	const auto v_mod_name_obj = v_root["name"];

	if (!(v_mod_uuid_obj.is_string() && v_mod_name_obj.is_string()))
		return nullptr;

	const SMUuid v_mod_uuid = v_mod_uuid_obj.get_c_str();
	const std::wstring v_mod_name = String::ToWide(v_mod_name_obj.get_c_str());

	const UuidObjMapIterator<Mod*> v_iter = Mod::ModStorage.find(v_mod_uuid);
	if (v_iter != Mod::ModStorage.end())
	{
		const Mod* v_modPtr = v_iter->second;

		if (v_iter->second->m_isLocal && !is_local)
		{
			DebugOutL("Skipped a mod as it was already loaded locally (name: ", v_mod_name, ", uuid: ", v_mod_uuid.ToString(), ")");
			return nullptr;
		}

		if (v_iter->second->m_isLocal == is_local)
		{
			DebugWarningL("Uuid conflict between: ", v_iter->second->m_Name, " and ", v_mod_name, " (uuid: ", v_mod_uuid.ToString(), ")");
			return nullptr;
		}
	}

	Mod* v_newMod;
	const char* v_mod_type_str = v_mod_type.get_c_str();
	if (strcmp(v_mod_type_str, "Blocks and Parts") == 0)
		v_newMod = new BlocksAndPartsMod();
	else if (strcmp(v_mod_type_str, "Terrain Assets") == 0)
		v_newMod = new TerrainAssetsMod();
	else
		return nullptr;

	v_newMod->m_Name = v_mod_name;
	v_newMod->m_Directory = mod_folder;
	v_newMod->m_Uuid = v_mod_uuid;
	v_newMod->m_isLocal = is_local;

	DebugOutL("Mod: ", 0b1101_fg, v_newMod->m_Name, 0b1110_fg, ", Uuid: ", 0b1101_fg, v_newMod->m_Uuid.ToString(), 0b1110_fg, ", Type: ", 0b1101_fg, v_mod_type_str);
	Mod::ModStorage.insert(std::make_pair(v_newMod->m_Uuid, v_newMod));
	Mod::ModVector.push_back(v_newMod);

	return v_newMod;
}

BlockData* Mod::GetGlobalBlock(const SMUuid& uuid)
{
	const UuidObjMapIterator<BlockData*> v_iter = Mod::BlockStorage.find(uuid);
	if (v_iter != Mod::BlockStorage.end())
		return v_iter->second;

	DebugErrorL("Couldn't find a block with the specified uuid: ", uuid.ToString());
	return nullptr;
}

PartData* Mod::GetGlobalPart(const SMUuid& uuid)
{
	const UuidObjMapIterator<PartData*> v_iter = Mod::PartStorage.find(uuid);
	if (v_iter != Mod::PartStorage.end())
		return v_iter->second;

	DebugErrorL("Couldn't find a part with the specified uuid: ", uuid.ToString());
	return nullptr;
}

AssetData* Mod::GetGlobalAsset(const SMUuid& uuid)
{
	const UuidObjMapIterator<AssetData*> v_iter = Mod::AssetStorage.find(uuid);
	if (v_iter != Mod::AssetStorage.end())
		return v_iter->second;

	DebugErrorL("Couldn't find an asset with the specified uuid: ", uuid.ToString());
	return nullptr;
}

HarvestableData* Mod::GetGlobalHarvestbale(const SMUuid& uuid)
{
	const UuidObjMapIterator<HarvestableData*> v_iter = Mod::HarvestableStorage.find(uuid);
	if (v_iter != Mod::HarvestableStorage.end())
		return v_iter->second;

	DebugErrorL("Couldn't find a harvestable with the specified uuid: ", uuid.ToString());
	return nullptr;
}

DecalData* Mod::GetGlobalDecal(const SMUuid& uuid)
{
	const UuidObjMapIterator<DecalData*> v_iter = Mod::DecalStorage.find(uuid);
	if (v_iter != Mod::DecalStorage.end())
		return v_iter->second;

	DebugErrorL("Couldn't find a decal with the specified uuid: ", uuid.ToString());
	return nullptr;
}

ClutterData* Mod::GetGlobalClutter(const SMUuid& uuid)
{
	const UuidObjMapIterator<ClutterData*> v_iter = Mod::ClutterStorage.find(uuid);
	if (v_iter != Mod::ClutterStorage.end())
		return v_iter->second;

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

using DataLoaderMap = std::unordered_map<std::string, void (*)(const simdjson::dom::element&, Mod*)>;
static const DataLoaderMap g_DataLoaders =
{
	{ "assetListRenderable", AssetListLoader::Load       },
	{ "harvestableList",     HarvestableListLoader::Load },
	{ "partList",			 PartListLoader::Load		 },
	{ "blockList",			 BlockListLoader::Load		 },
	{ "clutterList",		 ClutterListLoader::Load     },
	{ "decalSetList",        DecalsetListReader::Load    }
};

void Mod::LoadFile(const std::wstring& path)
{
	simdjson::dom::document v_doc;
	if (!JsonReader::LoadParseSimdjsonCommentsC(path, v_doc, simdjson::dom::element_type::OBJECT))
		return;

	DebugOutL("Loading: ", path);

	for (const auto v_obj : v_doc.root().get_object())
	{
		const std::string v_key_str = std::string(v_obj.key.data(), v_obj.key.size());

		const DataLoaderMap::const_iterator v_iter = g_DataLoaders.find(v_key_str);
		if (v_iter == g_DataLoaders.end())
		{
			DebugErrorL("Couldn't find the loader for: ", v_key_str);
			continue;
		}

		v_iter->second(v_obj.value, this);
	}
}

inline bool IsShapeSetExtensionValid(const std::string& extension)
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