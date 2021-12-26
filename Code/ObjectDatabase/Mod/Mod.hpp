#pragma once

#include <string>
#include <unordered_map>

#include "Utils/Uuid.hpp"
#include "Utils/Json.hpp"
#include "ObjectDatabase/ObjectData.hpp"

enum class ModType
{
	BlocksAndParts,
	TerrainAssets,
	GameData
};

class Mod
{
	friend class DatabaseLoader;
	friend class AssetListLoader;
	friend class HarvestableListLoader;
	friend class PartListLoader;
	friend class BlockListLoader;

	static std::unordered_map<SMUuid, Mod*> ModStorage;

	static std::unordered_map<SMUuid, BlockData*> BlockStorage;
	static std::unordered_map<SMUuid, PartData*> PartStorage;
	static std::unordered_map<SMUuid, AssetData*> AssetStorage;
	static std::unordered_map<SMUuid, HarvestableData*> HarvestableStorage;

	static const std::unordered_map<std::string, void (*)(const nlohmann::json&, Mod*)> DataLoaders;

	std::unordered_map<SMUuid, BlockData*> Blocks = {};
	std::unordered_map<SMUuid, PartData*> Parts   = {};
	std::unordered_map<SMUuid, AssetData*> Assets = {};
	std::unordered_map<SMUuid, HarvestableData*> Harvestables = {};

	SMUuid Uuid;
	std::wstring Name;
	std::wstring Directory;
	ModType Type;

public:
	Mod(const std::wstring& name, const std::wstring& dir, const SMUuid& uuid, const ModType& type);
	Mod(const Mod&) = delete;
	Mod(Mod&&) = delete;
	~Mod() = default;

	static Mod* LoadFromDescription(const std::wstring& mod_folder);

	static BlockData* GetGlobalBlock(const SMUuid& uuid);
	static PartData* GetGlobalPart(const SMUuid& uuid);
	static AssetData* GetGlobalAsset(const SMUuid& uuid);
	static HarvestableData* GetGlobalHarvestbale(const SMUuid& uuid);

	std::wstring GetDatabaseDirectory() const;

	BlockData* GetBlock(const SMUuid& uuid) const;
	PartData* GetPart(const SMUuid& uuid) const;
	AssetData* GetAsset(const SMUuid& uuid) const;
	HarvestableData* GetHarvestable(const SMUuid& uuid) const;

private:
	

public:
	void LoadFile(const std::wstring& path);
	void ScanFolder(const std::wstring& folder);

	void LoadObjects();
};