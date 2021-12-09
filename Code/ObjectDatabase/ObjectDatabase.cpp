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

TextureList DatabaseLoader::LoadTextureList(const nlohmann::json& texList)
{
	TextureList new_list;

	const int arr_sz = (int)texList.size();
	const int list_sz = (arr_sz > 3 ? 3 : arr_sz);

	for (int a = 0; a < list_sz; a++)
	{
		const auto& cur_item = texList.at(a);

		if (cur_item.is_string())
		{
			std::wstring& wstr_path = new_list.GetStringRef(a);

			wstr_path = String::ToWide(cur_item.get<std::string>());
			KeywordReplacer::ReplaceKeyR(wstr_path);
		}
	}

	return new_list;
}

void DatabaseLoader::AddSubMesh(const nlohmann::json& subMesh, TextureData& tData, const std::wstring& idx)
{
	const auto& sTexList = JsonReader::Get(subMesh, "textureList");
	if (!sTexList.is_array()) return;

	TextureList new_tList = DatabaseLoader::LoadTextureList(sTexList);

	const auto& sTexName = JsonReader::Get(subMesh, "material");
	if (sTexName.is_string())
		new_tList.material = String::ToWide(sTexName.get<std::string>());

	tData.AddEntry(idx, new_tList);
}

bool DatabaseLoader::LoadTextureData(const nlohmann::json& jLodList, TextureData& tData)
{
	const auto& jSubMeshList = JsonReader::Get(jLodList, "subMeshList");
	if (jSubMeshList.is_array())
	{
		std::size_t _idx = 0;
		
		for (const auto& subMeshItem : jSubMeshList)
		{
			if (!subMeshItem.is_object()) continue;

			const auto& sIdx = JsonReader::Get(subMeshItem, "idx");
			std::size_t cur_idx = (sIdx.is_number() ? sIdx.get<std::size_t>() : _idx);

			DatabaseLoader::AddSubMesh(subMeshItem, tData, std::to_wstring(cur_idx));
			_idx++;
		}

		return true;
	}

	const auto& jSubMeshMap = JsonReader::Get(jLodList, "subMeshMap");
	if (jSubMeshMap.is_object())
	{
		for (const auto& subMeshItem : jSubMeshMap.items())
		{
			if (!subMeshItem.value().is_object()) continue;

			std::wstring subMeshName = String::ToWide(subMeshItem.key());
			DatabaseLoader::AddSubMesh(subMeshItem.value(), tData, subMeshName);
		}

		return true;
	}

	return false;
}

bool DatabaseLoader::LoadRenderableData(const nlohmann::json& jRenderable, TextureData& tData, std::wstring& mesh)
{
	const auto& aLodList = JsonReader::Get(jRenderable, "lodList");
	if (!aLodList.is_array()) return false;

	const auto& aLodList0 = JsonReader::Get(aLodList, 0);
	if (!aLodList0.is_object()) return false;

	if (!DatabaseLoader::LoadTextureData(aLodList0, tData))
		return false;

	const auto& aMeshPath = JsonReader::Get(aLodList0, "mesh");
	if (!aMeshPath.is_string()) return false;

	mesh = String::ToWide(aMeshPath.get<std::string>());
	KeywordReplacer::ReplaceKeyR(mesh);

	return true;
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

void DatabaseLoader::LoadDefaultColors(const nlohmann::json& jAsset, std::unordered_map<std::string, int>& def_colors)
{
	const auto& aDefColors = JsonReader::Get(jAsset, "defaultColors");
	if (!aDefColors.is_object()) return;

	//TODO: Add a completely separate class for colors
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
		if (!aUuid.is_string()) continue;
		
		std::wstring tMesh;
		TextureData tData;
		if (!DatabaseLoader::LoadRenderable(mAsset, tData, tMesh))
			continue;

		AssetData* new_asset = new AssetData();
		new_asset->Uuid = aUuid.get<std::string>();
		new_asset->Mesh = tMesh;
		DatabaseLoader::LoadDefaultColors(mAsset, new_asset->DefaultColors);
		new_asset->Textures = tData;
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