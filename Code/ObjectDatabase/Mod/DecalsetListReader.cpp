#include "DecalsetListReader.hpp"

#include "ObjectDatabase\Mod\DecalsetReader.hpp"
#include "ObjectDatabase\KeywordReplacer.hpp"

#include "Utils\String.hpp"
#include "Console.hpp"

void DecalsetListReader::Load(const nlohmann::json& j_data, Mod* mod)
{
	if (!j_data.is_array()) return;

	for (const auto& v_decalSet : j_data)
	{
		if (!v_decalSet.is_object()) continue;

		const auto& v_setPath = JsonReader::Get(v_decalSet, "set");
		if (!v_setPath.is_string()) continue;

		const std::wstring v_setPathStr = String::ToWide(v_setPath.get_ref<const std::string&>());
		const std::wstring v_setPathReplaced = KeywordReplacer::ReplaceKey(v_setPathStr);

		DecalsetReader::LoadFromFile(v_setPathReplaced, mod);
	}
}