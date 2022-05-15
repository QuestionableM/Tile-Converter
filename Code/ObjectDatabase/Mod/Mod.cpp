#include "Mod.hpp"

#include "Utils/File.hpp"
#include "Utils/String.hpp"

#include "ObjectDatabase/Mod/AssetListLoader.hpp"
#include "ObjectDatabase/Mod/HarvestableListLoader.hpp"
#include "ObjectDatabase/Mod/PartListLoader.hpp"
#include "ObjectDatabase/Mod/BlockListLoader.hpp"
#include "ObjectDatabase/Mod/ClutterListLoader.hpp"
#include "ObjectDatabase/KeywordReplacer.hpp"

#include "Console.hpp"

#include <filesystem>
namespace fs = std::filesystem;

Mod::Mod(const std::wstring& name, const std::wstring& dir, const SMUuid& uuid, const ModType& type)
{
	this->m_Name = name;
	this->m_Directory = dir;
	this->m_Uuid = uuid;
	this->m_Type = type;
}

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

Mod* Mod::LoadFromDescription(const std::wstring& mod_folder)
{
	const std::wstring mDescPath = mod_folder + L"/description.json";
	if (!File::Exists(mDescPath)) return nullptr;

	nlohmann::json mDesc = JsonReader::LoadParseJson(mDescPath);
	if (!mDesc.is_object()) return nullptr;

	const auto& mType = JsonReader::Get(mDesc, "type");
	if (!mType.is_string()) return nullptr;

	const std::string mTypeStr = mType.get<std::string>();
	ModType mod_type;

	if (mTypeStr == "Blocks and Parts")
		mod_type = ModType::BlocksAndParts;
	else if (mTypeStr == "Terrain Assets")
		mod_type = ModType::TerrainAssets;
	else
		return nullptr;

	const auto& mUuid = JsonReader::Get(mDesc, "localId");
	const auto& mName = JsonReader::Get(mDesc, "name");

	if (!mUuid.is_string() || !mName.is_string()) return nullptr;

	const SMUuid mod_uuid = mUuid.get<std::string>();
	const std::wstring mod_name = String::ToWide(mName.get<std::string>());

	if (ModStorage.find(mod_uuid) != ModStorage.end())
	{
		const Mod* old_mod = ModStorage.at(mod_uuid);

		DebugErrorL("Uuid conflict between 2 items: ", old_mod->m_Name, " and ", mod_name);
		return nullptr;
	}

	Mod* pNewMod = new Mod(mod_name, mod_folder, mod_uuid, mod_type);

	DebugOutL("Mod: ", 0b1101_fg, pNewMod->m_Name, 0b1110_fg, ", Uuid: ", 0b1101_fg, pNewMod->m_Uuid.ToString());
	Mod::ModStorage.insert(std::make_pair(pNewMod->m_Uuid, pNewMod));
	Mod::ModVector.push_back(pNewMod);

	return pNewMod;
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
	return ModStorage.size();
}

std::wstring Mod::GetDatabaseDirectory() const
{
	switch (m_Type)
	{
	case ModType::BlocksAndParts:return m_Directory + L"/Objects/Database/ShapeSets";
	case ModType::TerrainAssets: return m_Directory + L"/Database/AssetSets";
	}

	return L"UNKNOWN";
}

BlockData* Mod::GetBlock(const SMUuid& uuid) const
{
	if (m_Blocks.find(uuid) != m_Blocks.end())
		return m_Blocks.at(uuid);

	return nullptr;
}

PartData* Mod::GetPart(const SMUuid& uuid) const
{
	if (m_Parts.find(uuid) != m_Parts.end())
		return m_Parts.at(uuid);
	
	return nullptr;
}

AssetData* Mod::GetAsset(const SMUuid& uuid) const
{
	if (m_Assets.find(uuid) != m_Assets.end())
		return m_Assets.at(uuid);

	return nullptr;
}

HarvestableData* Mod::GetHarvestable(const SMUuid& uuid) const
{
	if (m_Harvestables.find(uuid) != m_Harvestables.end())
		return m_Harvestables.at(uuid);

	return nullptr;
}

ClutterData* Mod::GetClutter(const SMUuid& uuid) const
{
	if (m_Clutter.find(uuid) != m_Clutter.end())
		return m_Clutter.at(uuid);

	return nullptr;
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

void Mod::ScanFolder(const std::wstring& folder)
{
	std::error_code rError;
	fs::recursive_directory_iterator rDirIter(folder, fs::directory_options::skip_permission_denied, rError);

	for (const auto& dir : rDirIter)
	{
		if (rError || !dir.is_regular_file()) continue;

		const fs::path& dPath = dir.path();

		if (dPath.has_extension() && dPath.extension() == ".json")
			this->LoadFile(dPath.wstring());
	}
}

void Mod::LoadObjects()
{
	KeywordReplacer::SetModData(m_Directory, m_Uuid);

	const std::wstring data_dir = this->GetDatabaseDirectory();

	this->ScanFolder(data_dir);
}