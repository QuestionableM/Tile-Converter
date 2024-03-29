#include "DecalsetListReader.hpp"

#include "ObjectDatabase\Readers\DecalsetReader.hpp"
#include "ObjectDatabase\KeywordReplacer.hpp"

#include "Utils\String.hpp"
#include "Console.hpp"

void DecalsetListReader::Load(const simdjson::dom::element& j_data, Mod* mod)
{
	if (!j_data.is_array()) return;

	for (const auto v_decal_set : j_data.get_array())
	{
		if (!v_decal_set.is_object()) continue;

		const auto v_set_path = v_decal_set["set"];
		if (!v_set_path.is_string()) continue;

		std::wstring v_set_path_str = String::ToWide(v_set_path.get_string());
		KeywordReplacer::ReplaceKeyR(v_set_path_str);

		DecalsetReader::LoadFromFile(v_set_path_str, mod);
	}
}