#pragma once

#include <unordered_map>

#include "Utils/Json.hpp"
#include "Utils/Color.hpp"

class Mod;

class AssetListLoader
{
	AssetListLoader() = default;

	static void LoadDefaultColors(const simdjson::dom::element& jAsset, std::unordered_map<std::wstring, Color>& def_colors);
public:
	static void Load(const simdjson::dom::element& fAssets, Mod* mod);
};