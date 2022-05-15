#include "HarvestableListLoader.hpp"
#include "ObjectDatabase/Mod/DefaultLoader.hpp"
#include "ObjectDatabase/Mod/Mod.hpp"
#include "ObjectDatabase/ProgCounter.hpp"

#include "Console.hpp"

void HarvestableListLoader::Load(const nlohmann::json& fHarvestables, Mod* mod)
{
	if (!fHarvestables.is_array()) return;

	ProgCounter::ProgressMax += fHarvestables.size();
	for (const auto& mHarvestable : fHarvestables)
	{
		if (!mHarvestable.is_object()) continue;

		const auto& hUuid = JsonReader::Get(mHarvestable, "uuid");
		if (!hUuid.is_string()) continue;

		SMUuid hvs_uuid = hUuid.get<std::string>();
		if (Mod::HarvestableStorage.find(hvs_uuid) != Mod::HarvestableStorage.end())
		{
			DebugWarningL("Harvestable with the specified uuid already exists! (", hvs_uuid.ToString(), ")");
			continue;
		}

		std::wstring tMesh;
		TextureData tData;
		if (!DefaultLoader::LoadRenderable(mHarvestable, tData, tMesh))
			continue;

		HarvestableData* new_hvs = new HarvestableData();
		new_hvs->Uuid = hvs_uuid;
		new_hvs->Mesh = tMesh;
		new_hvs->Textures = tData;
		new_hvs->pMod = mod;

		const auto new_pair = std::make_pair(new_hvs->Uuid, new_hvs);

		mod->m_Harvestables.insert(new_pair);
		Mod::HarvestableStorage.insert(new_pair);

		ProgCounter::ProgressValue++;
	}
}