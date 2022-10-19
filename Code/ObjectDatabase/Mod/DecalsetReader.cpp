#include "DecalsetReader.hpp"

#include "ObjectDatabase\KeywordReplacer.hpp"

#include "Utils\String.hpp"
#include "Utils\Json.hpp"

#include "Console.hpp"

inline void GetWstringFromDecalset(const nlohmann::json& obj, const std::string& key, std::wstring& r_output)
{
	const auto& v_jsonString = JsonReader::Get(obj, key);
	if (v_jsonString.is_string())
	{
		r_output = String::ToWide(v_jsonString.get_ref<const std::string&>());
		KeywordReplacer::ReplaceKeyR(r_output);
	}
}

void DecalsetReader::LoadFromFile(const std::wstring& path, Mod* mod)
{
	const nlohmann::json v_decalJson = JsonReader::LoadParseJson(path);
	if (!v_decalJson.is_object())
		return;

	const auto& v_decalList = JsonReader::Get(v_decalJson, "decalList");
	if (!v_decalList.is_object())
		return;

	TextureList v_texList;
	GetWstringFromDecalset(v_decalJson, "difSheet", v_texList.dif);
	GetWstringFromDecalset(v_decalJson, "asgSheet", v_texList.asg);
	GetWstringFromDecalset(v_decalJson, "norSheet", v_texList.nor);

	for (const auto& v_curDecal : v_decalList.items())
	{
		const auto& v_curVal = v_curDecal.value();
		if (!v_curVal.is_object()) continue;

		const auto& v_decalUuidStr = JsonReader::Get(v_curVal, "uuid");
		const auto& v_decalMaterial = JsonReader::Get(v_curVal, "material");

		if (!v_decalUuidStr.is_string() || !v_decalMaterial.is_string())
			continue;

		const auto& v_decalRegion = JsonReader::Get(v_curVal, "region");
		if (!v_decalRegion.is_array() || v_decalRegion.size() != 4)
			continue;

		const SMUuid v_decalUuid(v_decalUuidStr.get_ref<const std::string&>());
		Mod::UuidObjMapIterator<DecalData*> v_iter = Mod::DecalStorage.find(v_decalUuid);
		if (v_iter != Mod::DecalStorage.end())
		{
			DebugWarningL("A decal with the same uuid already exists! (uuid: ", v_decalUuid.ToString(), ")");
			continue;
		}

		v_texList.material = String::ToWide(v_decalMaterial.get_ref<const std::string&>());

		DecalData* v_newDecal = new DecalData();
		v_newDecal->m_name = v_curDecal.key();
		v_newDecal->m_uuid = v_decalUuid;
		v_newDecal->m_textures = v_texList;
		v_newDecal->m_mod = mod;
		v_newDecal->m_ranges[0] = v_decalRegion[0];
		v_newDecal->m_ranges[1] = v_decalRegion[1];
		v_newDecal->m_ranges[2] = v_decalRegion[2];
		v_newDecal->m_ranges[3] = v_decalRegion[3];

		const auto v_decalPair = std::make_pair(v_newDecal->m_uuid, v_newDecal);

		Mod::DecalStorage.insert(v_decalPair);
		mod->m_Decals.insert(v_decalPair);
	}
}