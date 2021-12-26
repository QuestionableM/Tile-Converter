#pragma once

#include "Utils/Json.hpp"

class Mod;
class TextureList;

class BlockListLoader
{
	BlockListLoader() = default;

	static bool GetBlockTextures(const nlohmann::json& block, TextureList& tex);
public:
	static void Load(const nlohmann::json& fBlocks, Mod* mod);
};