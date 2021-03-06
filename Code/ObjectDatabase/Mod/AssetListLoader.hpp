#pragma once

#include <unordered_map>

#include "Utils/Json.hpp"
#include "Utils/Color.hpp"

class Mod;

class AssetListLoader
{
	AssetListLoader() = default;

	static void LoadDefaultColors(const nlohmann::json& jAsset, std::unordered_map<std::wstring, Color>& def_colors);
public:
	static void Load(const nlohmann::json& fAssets, Mod* mod);
};