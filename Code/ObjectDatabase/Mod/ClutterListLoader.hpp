#pragma once

#include "Utils/Json.hpp"

class Mod;
class ClutterData;
class TextureList;

class ClutterListLoader
{
	ClutterListLoader() = default;

	static bool LoadTextureData(const nlohmann::json& fClutter, TextureList& tList, std::wstring& mesh);
	static void LoadClutterData(const nlohmann::json& fClutter, ClutterData* pClutter);
public:
	static void Load(const nlohmann::json& fClutter, Mod* mod);
};