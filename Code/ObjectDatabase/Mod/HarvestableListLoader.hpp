#pragma once

#include "Utils/Json.hpp"

class Mod;

class HarvestableListLoader
{
	HarvestableListLoader() = default;
public:
	static void Load(const nlohmann::json& fHarvestables, Mod* mod);
};