#include "DecalsetReader.hpp"

#include "ObjectDatabase\KeywordReplacer.hpp"

#include "Utils\String.hpp"
#include "Utils\Json.hpp"

#include "Console.hpp"

inline void GetWstringFromDecalset(const simdjson::dom::element& obj, const std::string& key, std::wstring& r_output)
{
	const auto v_json_str = obj[key];
	if (v_json_str.is_string())
	{
		r_output = String::ToWide(v_json_str.get_string());
		KeywordReplacer::ReplaceKeyR(r_output);
	}
}

void DecalsetReader::LoadFromFile(const std::wstring& path, Mod* mod)
{
	simdjson::dom::document v_json_doc;
	if (!JsonReader::LoadParseSimdjsonCommentsC(path, v_json_doc, simdjson::dom::element_type::OBJECT))
		return;

	const auto v_root = v_json_doc.root();

	const auto v_decal_list = v_root["decalList"];
	if (!v_decal_list.is_object())
		return;

	TextureList v_texList;
	GetWstringFromDecalset(v_root, "difSheet", v_texList.dif);
	GetWstringFromDecalset(v_root, "asgSheet", v_texList.asg);
	GetWstringFromDecalset(v_root, "norSheet", v_texList.nor);

	for (const auto v_decal : v_decal_list.get_object())
	{
		if (!v_decal.value.is_object()) continue;

		const auto v_uuid = v_decal.value["uuid"];
		const auto v_material = v_decal.value["material"];

		if (!(v_uuid.is_string() && v_material.is_string()))
			continue;

		const auto v_decal_region = v_decal.value["region"];
		if (!v_decal_region.is_array())
			continue;

		const auto v_decal_reg_array = v_decal_region.get_array();
		if (v_decal_reg_array.size() != 4)
			continue;

		const SMUuid v_decal_uuid = v_uuid.get_c_str();
		if (Mod::DecalStorage.find(v_decal_uuid) != Mod::DecalStorage.end())
		{
			DebugWarningL("A decal with the same uuid already exists! (uuid: ", v_decal_uuid.ToString(), ")");
			continue;
		}

		v_texList.material = String::ToWide(v_material.get_string());

		DecalData* v_new_decal = new DecalData();
		v_new_decal->m_name = std::string(v_decal.key.data(), v_decal.key.size());
		v_new_decal->m_uuid = v_decal_uuid;
		v_new_decal->m_textures = v_texList;
		v_new_decal->m_mod = mod;

		for (std::size_t a = 0; a < 4; a++)
			v_new_decal->m_ranges[a] = v_decal_reg_array.at(a);

		const auto v_decal_pair = std::make_pair(v_new_decal->m_uuid, v_new_decal);
		Mod::DecalStorage.insert(v_decal_pair);
		mod->m_Decals.insert(v_decal_pair);
	}
}