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
	friend class ClutterListLoader;

	inline static std::unordered_map<SMUuid, Mod*> ModStorage = {};
	inline static std::vector<Mod*> ModVector = {};

	inline static std::unordered_map<SMUuid, BlockData*> BlockStorage             = {};
	inline static std::unordered_map<SMUuid, PartData*> PartStorage               = {};
	inline static std::unordered_map<SMUuid, AssetData*> AssetStorage             = {};
	inline static std::unordered_map<SMUuid, HarvestableData*> HarvestableStorage = {};

	inline static std::unordered_map<SMUuid, ClutterData*> ClutterStorage = {};
	inline static std::vector<ClutterData*> ClutterVector                 = {};

	std::unordered_map<SMUuid, BlockData*> m_Blocks = {};
	std::unordered_map<SMUuid, PartData*> m_Parts   = {};
	std::unordered_map<SMUuid, AssetData*> m_Assets = {};
	std::unordered_map<SMUuid, HarvestableData*> m_Harvestables = {};
	std::unordered_map<SMUuid, ClutterData*> m_Clutter = {};

	SMUuid m_Uuid;
	std::wstring m_Name;
	std::wstring m_Directory;
	ModType m_Type;

public:
	Mod(const std::wstring& name, const std::wstring& dir, const SMUuid& uuid, const ModType& type);
	Mod(const Mod&) = delete;
	Mod(Mod&&)      = delete;
	Mod(Mod&)       = delete;
	~Mod();

	static void ClearModStorage();
	static Mod* LoadFromDescription(const std::wstring& mod_folder);

	static BlockData* GetGlobalBlock(const SMUuid& uuid);
	static PartData* GetGlobalPart(const SMUuid& uuid);
	static AssetData* GetGlobalAsset(const SMUuid& uuid);
	static HarvestableData* GetGlobalHarvestbale(const SMUuid& uuid);
	static ClutterData* GetGlobalClutter(const SMUuid& uuid);
	static ClutterData* GetGlobalClutterById(const std::size_t& idx);

	static std::size_t GetAmountOfObjects();
	static std::size_t GetAmountOfMods();

	BlockData* GetBlock(const SMUuid& uuid) const;
	PartData* GetPart(const SMUuid& uuid) const;
	AssetData* GetAsset(const SMUuid& uuid) const;
	HarvestableData* GetHarvestable(const SMUuid& uuid) const;
	ClutterData* GetClutter(const SMUuid& uuid) const;

	void LoadFile(const std::wstring& path);

	void ScanDatabaseFolderRecursive(const std::wstring& folder);
	void ScanDatabaseFolder(const std::wstring& folder);

	static void LoadShapeSetDatabase(const std::wstring& path, Mod* pMod);
	static void LoadAssetSetDatabase(const std::wstring& path, Mod* pMod);
	void LoadObjectDatabase();
};