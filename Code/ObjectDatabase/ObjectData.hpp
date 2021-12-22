#pragma once

#include <string>
#include <unordered_map>
#include "Utils/Uuid.hpp"
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