#pragma once

#include "Utils/Json.hpp"

class HarvestableListLoader
{
public:
	static void Load(const nlohmann::json& fHarvestables);
};