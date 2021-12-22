#include "ObjectDatabase/ObjectDatabase.hpp"

#include "ObjectDatabase/DatabaseConfig.hpp"
#include "ObjectDatabase/KeywordReplacer.hpp"
#include "Utils/Json.hpp"
#include "Utils/String.hpp"
#include "Console.hpp"

#include "ObjectDatabase/DatabaseLoaders/AssetListLoader.hpp"
#include "ObjectDatabase/DatabaseLoaders/HarvestableListLoader.hpp"

#include <filesystem>

namespace fs = std::filesystem;

std::unordered_map<SMUuid, AssetData*> DatabaseLoader::Assets = {};
const std::unordered_map<std::string, void (*)(const nlohmann::json&)> DatabaseLoader::FuncPointers =
{
	{ "assetListRenderable", AssetListLoader::Load		 },
	{ "harvestableList",     HarvestableListLoader::Load }
};

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

	const auto& sCustomProp = JsonReader::Get(subMesh, "custom");
	if (sCustomProp.is_object())
	{
		const auto& def_color_idx = JsonReader::Get(sCustomProp, "color");
		if (def_color_idx.is_string())
			new_tList.def_color_idx = String::ToWide(def_color_idx.get<std::string>());
	}

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

void DatabaseLoader::LoadFile(const std::wstring& path)
{
	nlohmann::json file_json = JsonReader::LoadParseJson(path);
	if (!file_json.is_object()) return;

	DebugOutL("Loading: ", path);

	for (const auto& fKey : file_json.items())
	{
		const std::string key_str = fKey.key();

		if (FuncPointers.find(key_str) != FuncPointers.end())
			FuncPointers.at(key_str)(fKey.value());
	}
}

void DatabaseLoader::ScanFolder(const std::wstring& folder)
{
	std::error_code rError;
	fs::recursive_directory_iterator rDirIter(folder, fs::directory_options::skip_permission_denied, rError);
	for (const auto& dir : rDirIter)
	{
		if (rError || !dir.is_regular_file()) continue;

		const fs::path& dPath = dir.path();

		if (dPath.has_extension() && dPath.extension() == ".json")
			DatabaseLoader::LoadFile(dPath.wstring());
	}
}

void DatabaseLoader::LoadGameDatabase()
{
	for (const std::wstring& db_dir : DatabaseConfig::AssetListFolders)
		DatabaseLoader::ScanFolder(db_dir);
}

void DatabaseLoader::LoadModDatabase()
{
	for (const std::wstring& mod_dir : DatabaseConfig::ModFolders)
	{
		std::error_code rError;
		fs::directory_iterator rDirIter(mod_dir, fs::directory_options::skip_permission_denied, rError);

		for (const auto& dir : rDirIter)
		{
			if (rError || !dir.is_directory()) continue;

			const fs::path& dPath = dir.path();
			const std::wstring mDescPath = dPath.wstring() + L"/description.json";

			if (!fs::exists(mDescPath)) continue;

			nlohmann::json mDesc = JsonReader::LoadParseJson(mDescPath);
			if (!mDesc.is_object()) continue;

			const auto& mUuid = JsonReader::Get(mDesc, "localId");
			const auto& mType = JsonReader::Get(mDesc, "type");
			const auto& mName = JsonReader::Get(mDesc, "name");

			if (!mUuid.is_string() || !(mType.is_string() && mType.get<std::string>() == "Terrain Assets") || !mName.is_string()) continue;

			const std::wstring dbDirectory = dPath.wstring() + L"/Database/AssetSets";
			if (!fs::exists(dbDirectory)) continue;

			SMUuid mod_uuid = mUuid.get<std::string>();

			KeywordReplacer::SetModData(mod_uuid, dPath.wstring());

			DatabaseLoader::ScanFolder(dbDirectory);

			DebugOutL("Mod: ", ConCol::YELLOW_INT, mName.get<std::string>(), ConCol::WHITE, ", Uuid: ", ConCol::YELLOW_INT, mUuid.get<std::string>());
		}
	}
}

AssetData* DatabaseLoader::GetAsset(const SMUuid& uuid)
{
	if (DatabaseLoader::Assets.find(uuid) != DatabaseLoader::Assets.end())
		return DatabaseLoader::Assets.at(uuid);

	return nullptr;
}

void DatabaseLoader::LoadDatabase()
{
	DatabaseLoader::LoadGameDatabase();
	DatabaseLoader::LoadModDatabase();
}