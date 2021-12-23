#pragma once

#include "Utils/Json.hpp"
#include "ObjectDatabase/ObjectData.hpp"

class DatabaseLoader
{
	friend class AssetListLoader;
	friend class HarvestableListLoader;

	static std::unordered_map<SMUuid, AssetData*> Assets;
	static std::unordered_map<SMUuid, HarvestableData*> Harvestables;
	static const std::unordered_map<std::string, void (*)(const nlohmann::json&)> FuncPointers;

	static TextureList LoadTextureList(const nlohmann::json& texList);
	static void AddSubMesh(const nlohmann::json& subMesh, TextureData& tData, const std::wstring& idx);
	static bool LoadTextureData(const nlohmann::json& jLodList, TextureData& tData);
	static bool LoadRenderableData(const nlohmann::json& jRenderable, TextureData& tData, std::wstring& mesh);
	static bool LoadRenderable(const nlohmann::json& jAsset, TextureData& tData, std::wstring& mesh);

	static void LoadFile(const std::wstring& path);
	static void ScanFolder(const std::wstring& folder);
	static void LoadGameDatabase();
	static void LoadModDatabase();

public:
	static AssetData* GetAsset(const SMUuid& uuid);
	static HarvestableData* GetHarvestable(const SMUuid& uuid);

	static void LoadDatabase();
};