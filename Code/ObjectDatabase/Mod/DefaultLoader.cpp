#include "DefaultLoader.hpp"

#include "Utils/String.hpp"
#include "ObjectDatabase/KeywordReplacer.hpp"

TextureList DefaultLoader::LoadTextureList(const nlohmann::json& texList)
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

			wstr_path = String::ToWide(cur_item.get_ref<const std::string&>());
			KeywordReplacer::ReplaceKeyR(wstr_path);
		}
	}

	return new_list;
}

void DefaultLoader::AddSubMesh(const nlohmann::json& subMesh, TextureData& tData, const std::wstring& idx)
{
	const auto& sTexList = JsonReader::Get(subMesh, "textureList");
	if (!sTexList.is_array()) return;

	TextureList new_tList = DefaultLoader::LoadTextureList(sTexList);

	const auto& sCustomProp = JsonReader::Get(subMesh, "custom");
	if (sCustomProp.is_object())
	{
		const auto& def_color_idx = JsonReader::Get(sCustomProp, "color");
		if (def_color_idx.is_string())
			new_tList.def_color_idx = String::ToWide(def_color_idx.get_ref<const std::string&>());
	}

	const auto& sTexName = JsonReader::Get(subMesh, "material");
	if (sTexName.is_string())
		new_tList.material = String::ToWide(sTexName.get_ref<const std::string&>());

	tData.AddEntry(idx, new_tList);
}

bool DefaultLoader::LoadTextureData(const nlohmann::json& jLodList, TextureData& tData)
{
	const auto& jSubMeshList = JsonReader::Get(jLodList, "subMeshList");
	if (jSubMeshList.is_array())
	{
		std::size_t _idx = 0;
		tData = TextureData(TextureDataType::SubMeshList);

		for (const auto& subMeshItem : jSubMeshList)
		{
			if (!subMeshItem.is_object()) continue;

			const auto& sIdx = JsonReader::Get(subMeshItem, "idx");
			std::size_t cur_idx = (sIdx.is_number() ? sIdx.get<std::size_t>() : _idx);

			DefaultLoader::AddSubMesh(subMeshItem, tData, std::to_wstring(cur_idx));
			_idx++;
		}

		return true;
	}

	const auto& jSubMeshMap = JsonReader::Get(jLodList, "subMeshMap");
	if (jSubMeshMap.is_object())
	{
		tData = TextureData(TextureDataType::SubMeshMap);

		for (const auto& subMeshItem : jSubMeshMap.items())
		{
			if (!subMeshItem.value().is_object()) continue;

			std::wstring subMeshName = String::ToWide(subMeshItem.key());
			DefaultLoader::AddSubMesh(subMeshItem.value(), tData, subMeshName);
		}

		return true;
	}

	return false;
}

bool DefaultLoader::LoadRenderableData(const nlohmann::json& jRenderable, TextureData& tData, std::wstring& mesh)
{
	const auto& aLodList = JsonReader::Get(jRenderable, "lodList");
	if (!aLodList.is_array()) return false;

	const auto& aLodList0 = JsonReader::Get(aLodList, 0);
	if (!aLodList0.is_object()) return false;

	if (!DefaultLoader::LoadTextureData(aLodList0, tData))
		return false;

	const auto& aMeshPath = JsonReader::Get(aLodList0, "mesh");
	if (!aMeshPath.is_string()) return false;

	mesh = String::ToWide(aMeshPath.get_ref<const std::string&>());
	KeywordReplacer::ReplaceKeyR(mesh);

	return true;
}

bool DefaultLoader::LoadRenderable(const nlohmann::json& jAsset, TextureData& tData, std::wstring& mesh)
{
	const auto& v_rendJson = JsonReader::Get(jAsset, "renderable");
	switch (v_rendJson.type())
	{
	case nlohmann::json::value_t::string:
		{
			std::wstring v_renderablePath = String::ToWide(v_rendJson.get_ref<const std::string&>());
			KeywordReplacer::ReplaceKeyR(v_renderablePath);

			const nlohmann::json v_rendData = JsonReader::LoadParseJson(v_renderablePath);
			if (!v_rendData.is_object())
				return false;

			return DefaultLoader::LoadRenderableData(v_rendData, tData, mesh);
		}
	case nlohmann::json::value_t::object:
		return DefaultLoader::LoadRenderableData(v_rendJson, tData, mesh);
	default:
		return false;
	}
}