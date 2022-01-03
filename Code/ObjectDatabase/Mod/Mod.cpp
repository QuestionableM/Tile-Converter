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

std::unordered_map<SMUuid, Mod*> Mod::ModStorage = {};

std::unordered_map<SMUuid, BlockData*> Mod::BlockStorage = {};
std::unordered_map<SMUuid, PartData*>  Mod::PartStorage  = {};
std::unordered_map<SMUuid, AssetData*> Mod::AssetStorage = {};
std::unordered_map<SMUuid, HarvestableData*> Mod::HarvestableStorage = {};
std::unordered_map<SMUuid, ClutterData*> Mod::ClutterStorage = {};
std::vector<ClutterData*> Mod::ClutterVector = {};

const std::unordered_map<std::string, void (*)(const nlohmann::json&, Mod*)> Mod::DataLoaders =
{
	{ "assetListRenderable", AssetListLoader::Load       },
	{ "harvestableList",     HarvestableListLoader::Load },
	{ "partList",			 PartListLoader::Load		 },
	{ "blockList",			 BlockListLoader::Load		 },
	{ "clutterList",		 ClutterListLoader::Load     }
};

Mod::Mod(const std::wstring& name, const std::wstring& dir, const SMUuid& uuid, const ModType& type)
{
	this->Name = name;
	this->Directory = dir;
	this->Uuid = uuid;
	this->Type = type;
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

		DebugErrorL("Uuid conflict between 2 items: ", old_mod->Name, " and ", mod_name);
		return nullptr;
	}

	Mod* nMod = new Mod(mod_name, mod_folder, mod_uuid, mod_type);

	DebugOutL("Mod: ", ConCol::YELLOW_INT, nMod->Name, ConCol::WHITE, ", Uuid: ", ConCol::YELLOW_INT, nMod->Uuid.ToString());
	ModStorage.insert(std::make_pair(nMod->Uuid, nMod));

	return nMod;
}

BlockData* Mod::GetGlobalBlock(const SMUuid& uuid)
{
	if (BlockStorage.find(uuid) != BlockStorage.end())
		return BlockStorage.at(uuid);

	return nullptr;
}

PartData* Mod::GetGlobalPart(const SMUuid& uuid)
{
	if (PartStorage.find(uuid) != PartStorage.end())
		return PartStorage.at(uuid);

	return nullptr;
}

AssetData* Mod::GetGlobalAsset(const SMUuid& uuid)
{
	if (AssetStorage.find(uuid) != AssetStorage.end())
		return AssetStorage.at(uuid);

	return nullptr;
}

HarvestableData* Mod::GetGlobalHarvestbale(const SMUuid& uuid)
{
	if (HarvestableStorage.find(uuid) != HarvestableStorage.end())
		return HarvestableStorage.at(uuid);

	return nullptr;
}

ClutterData* Mod::GetGlobalClutter(const SMUuid& uuid)
{
	if (ClutterStorage.find(uuid) != ClutterStorage.end())
		return ClutterStorage.at(uuid);

	return nullptr;
}

std::wstring Mod::GetDatabaseDirectory() const
{
	switch (this->Type)
	{
	case ModType::BlocksAndParts:return this->Directory + L"/Objects/Database/ShapeSets";
	case ModType::TerrainAssets: return this->Directory + L"/Database/AssetSets";
	}

	return L"UNKNOWN";
}

BlockData* Mod::GetBlock(const SMUuid& uuid) const
{
	if (this->Blocks.find(uuid) != this->Blocks.end())
		return this->Blocks.at(uuid);

	return nullptr;
}

PartData* Mod::GetPart(const SMUuid& uuid) const
{
	if (this->Parts.find(uuid) != this->Parts.end())
		return this->Parts.at(uuid);
	
	return nullptr;
}

AssetData* Mod::GetAsset(const SMUuid& uuid) const
{
	if (this->Assets.find(uuid) != this->Assets.end())
		return this->Assets.at(uuid);

	return nullptr;
}

HarvestableData* Mod::GetHarvestable(const SMUuid& uuid) const
{
	if (this->Harvestables.find(uuid) != this->Harvestables.end())
		return this->Harvestables.at(uuid);

	return nullptr;
}

ClutterData* Mod::GetClutter(const SMUuid& uuid) const
{
	if (this->Clutter.find(uuid) != this->Clutter.end())
		return this->Clutter.at(uuid);

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

void Mod::LoadFile(const std::wstring& path)
{
	nlohmann::json file_json = JsonReader::LoadParseJson(path);
	if (!file_json.is_object()) return;

	DebugOutL("Loading: ", path);

	for (const auto& fKey : file_json.items())
	{
		const std::string key_str = fKey.key();

		if (DataLoaders.find(key_str) != DataLoaders.end())
			DataLoaders.at(key_str)(fKey.value(), this);
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
	KeywordReplacer::SetModData(this->Uuid, this->Directory);

	const std::wstring data_dir = this->GetDatabaseDirectory();

	this->ScanFolder(data_dir);
}