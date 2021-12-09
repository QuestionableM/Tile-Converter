#pragma once

#include "SMReaders/Data/Uuid.hpp"

class TextureList
{
public:
	std::wstring dif;
	std::wstring asg;
	std::wstring nor;

	std::wstring material;

	std::wstring& GetStringRef(const std::size_t& idx)
	{
		return ((std::wstring*)&this->dif)[idx];
	}
};

enum class TextureDataType
{
	SubMeshList,
	SubMeshMap,
	None
};

class TextureData
{
	std::unordered_map<std::wstring, TextureList> MaterialMap = {};

public:
	void AddEntry(const std::wstring& name, const TextureList& tex_list)
	{
		if (MaterialMap.find(name) != MaterialMap.end())
			return;

		MaterialMap.insert(std::make_pair(name, tex_list));
	}

	bool GetEntry(const std::wstring& name, TextureList& list_ref) const
	{
		if (MaterialMap.find(name) == MaterialMap.end())
			return false;

		list_ref = MaterialMap.at(name);
		return true;
	}
};

class AssetData
{
public:
	SMUuid Uuid;
	std::unordered_map<std::string, int> DefaultColors;
	TextureData Textures;
	std::wstring Mesh;
};