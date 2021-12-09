#include "ObjectDatabase/ObjectDatabase.hpp"

#include "ObjectDatabase/DatabaseConfig.hpp"
#include "ObjectDatabase/KeywordReplacer.hpp"
#include "Utils/Json.hpp"
#include "Utils/String.hpp"
#include "Console.hpp"

#include <filesystem>

namespace fs = std::filesystem;

std::wstring& TextureList::GetStringRef(const std::size_t& idx)
{
	return ((std::wstring*)&this->dif)[idx];
}

void TextureData::AddEntry(const std::wstring& name, const TextureList& tex_list)
{
	if (MaterialMap.find(name) != MaterialMap.end())
		return;

	MaterialMap.insert(std::make_pair(name, tex_list));
}

bool TextureData::GetEntry(const std::wstring& name, TextureList& list_ref) const
{
	if (MaterialMap.find(name) == MaterialMap.end())
		return false;

	list_ref = MaterialMap.at(name);
	return true;
}

bool DatabaseLoader::LoadRenderableData(const nlohmann::json& jRenderable, TextureData& tData, std::wstring& mesh)
{
	const auto& aLodList = JsonReader::Get(jRenderable, "lodList");
	if (!aLodList.is_array()) return false;

	const auto& aLodList0 = JsonReader::Get(aLodList, 0);
	if (!aLodList0.is_object()) return false;



	const auto& aMeshPath = JsonReader::Get(aLodList0, "mesh");
	if (!aMeshPath.is_string()) return false;

	mesh = String::ToWide(aMeshPath.get<std::string>());
	KeywordReplacer::ReplaceKeyR(mesh);

	DebugOutL("Mesh: ", mesh);

	return false;
}

bool DatabaseLoader::LoadRenderable(const nlohmann::json& jAsset, TextureData& tData, std::wstring& mesh)
{
	const auto& aRenderable = JsonReader::Get(jAsset, "renderable");
	if (aRenderable.is_string())
	{
		std::wstring aRenderablePath = String::ToWide(aRenderable.get<std::string>());
		KeywordReplacer::ReplaceKeyR(aRenderablePath);

		nlohmann::json aRendData = JsonReader::LoadParseJson(aRenderablePath);
		if (!aRendData.is_object()) return false;

		return DatabaseLoader::LoadRenderableData(aRendData, tData, mesh);
	}
	else if (aRenderable.is_object())
	{
		return DatabaseLoader::LoadRenderableData(aRenderable, tData, mesh);
	}

	return false;
}

void DatabaseLoader::LoadDefaultColors(const nlohmann::json& jAsset, AssetData* data)
{

}

void DatabaseLoader::LoadFile(const std::wstring& path)
{
	nlohmann::json file_json = JsonReader::LoadParseJson(path);
	if (!file_json.is_object()) return;

	const auto& assetList = JsonReader::Get(file_json, "assetListRenderable");
	if (!assetList.is_array()) return;

	for (const auto& mAsset : assetList)
	{
		if (!mAsset.is_object()) continue;

		const auto& aUuid = JsonReader::Get(mAsset, "uuid");
		const auto& aDefColors = JsonReader::Get(mAsset, "defaultColors");
		if (!aUuid.is_string() || !aDefColors.is_object()) continue;
		
		std::wstring tMesh;
		TextureData tData;
		if (!DatabaseLoader::LoadRenderable(mAsset, tData, tMesh))
			continue;

		AssetData* new_asset = new AssetData();
		new_asset->Uuid = aUuid.get<std::string>();
		new_asset->Mesh = tMesh;
		//DatabaseLoader::LoadDefaultColors();
		/*
		 "uuid": "a32235cb-ea6f-46cf-82fe-df704a096d9e",
            "name": "env_building_pipe01",
            "col": "$SURVIVAL_DATA/Terrain/Collision/Building/env_building_pipe01_col.obj",
            "defaultColors": {
                "plastic": "2b2b2b",
                "shinymetal2": "9197ad",
                "shinymetal": "5a5a5a"
            },
            "physics": {
                "mass": 1000,
                "material": "Metal"
            },
            "renderable": "$SURVIVAL_DATA/Terrain/Renderable/Building/env_building_pipe01.rend"   
		*/
	}
}

void DatabaseLoader::ScanFolder(const std::wstring& folder)
{
	try
	{
		fs::recursive_directory_iterator rDirIter(folder, fs::directory_options::skip_permission_denied);
		for (const auto& dir : rDirIter)
		{
			if (!dir.is_regular_file()) continue;

			const fs::path& dPath = dir.path();

			if (dPath.has_extension() && dPath.extension() == ".json")
				DatabaseLoader::LoadFile(dPath.wstring());
		}
	}
	catch (...) {}
}

void DatabaseLoader::LoadGameDatabase()
{
	for (const std::wstring& db_dir : DatabaseConfig::AssetListFolders)
		DatabaseLoader::ScanFolder(db_dir);
}

void DatabaseLoader::LoadModDatabase()
{

}

void DatabaseLoader::LoadDatabase()
{
	DatabaseLoader::LoadGameDatabase();
	DatabaseLoader::LoadModDatabase();
}