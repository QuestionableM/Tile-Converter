#include "HarvestableListLoader.hpp"

#include "Console.hpp"

void HarvestableListLoader::Load(const nlohmann::json& fHarvestables)
{
	if (!fHarvestables.is_object()) return;
	DebugOutL("Loading Harvestable List...");


}