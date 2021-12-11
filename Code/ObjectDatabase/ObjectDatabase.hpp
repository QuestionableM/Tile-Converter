#pragma once

#include "Utils/Uuid.hpp"
#include "Utils/Json.hpp"
#include "Utils/Color.hpp"

enum class TextureDataType
{
	SubMeshList,
	SubMeshMap,
	None
};

class TextureList
{
public:
	std::wstring dif;
	std::wstring asg;
	std::wstring nor;

	std::wstring material;
	std::wstring def_color_idx;

	std::wstring& GetStringRef(const std::size_t& idx);
};

class TextureData
{
	std::unordered_map<std::wstring, TextureList> MaterialMap = {};

public:
	void AddEntry(const std::wstring& name, const TextureList& tex_list);
	bool GetEntry(const std::wstring& name, TextureList& list_ref) const;
};

class AssetData
{
public:
	SMUuid Uuid;
	std::unordered_map<std::string, Color> DefaultColors;
	TextureData Textures;
	std::wstring Mesh;

	AssetData() = default;
	AssetData(const AssetData&) = delete;
	AssetData(AssetData&&) = delete;
};

class DatabaseLoader
{
	static std::unordered_map<SMUuid, AssetData*> Assets;

	static TextureList LoadTextureList(const nlohmann::json& texList);
	static void AddSubMesh(const nlohmann::json& subMesh, TextureData& tData, const std::wstring& idx);
	static bool LoadTextureData(const nlohmann::json& jLodList, TextureData& tData);
	static bool LoadRenderableData(const nlohmann::json& jRenderable, TextureData& tData, std::wstring& mesh);
	static bool LoadRenderable(const nlohmann::json& jAsset, TextureData& tData, std::wstring& mesh);
	static void LoadDefaultColors(const nlohmann::json& jAsset, std::unordered_map<std::string, Color>& def_colors);
	static void LoadFile(const std::wstring& path);
	static void ScanFolder(const std::wstring& folder);
	static void LoadGameDatabase();
	static void LoadModDatabase();

public:
	static AssetData* GetAsset(const SMUuid& uuid);

	static void LoadDatabase();
};