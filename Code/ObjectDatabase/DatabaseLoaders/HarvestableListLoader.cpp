#include "HarvestableListLoader.hpp"
#include "ObjectDatabase/ObjectDatabase.hpp"

#include "Console.hpp"

void HarvestableListLoader::Load(const nlohmann::json& fHarvestables)
{
	if (!fHarvestables.is_array()) return;
	DebugOutL("Loading Harvestable List...");

	for (const auto& mHarvestable : fHarvestables)
	{
		if (!mHarvestable.is_object()) continue;

		const auto& hUuid = JsonReader::Get(mHarvestable, "uuid");
		if (!hUuid.is_string()) continue;

		std::wstring tMesh;
		TextureData tData;
		if (!DatabaseLoader::LoadRenderable(mHarvestable, tData, tMesh))
			continue;

		HarvestableData* new_hvs = new HarvestableData();
		new_hvs->Uuid = hUuid.get<std::string>();
		new_hvs->Mesh = tMesh;
		new_hvs->Textures = tData;

		DatabaseLoader::Harvestables.insert(std::make_pair(new_hvs->Uuid, new_hvs));
	}
}