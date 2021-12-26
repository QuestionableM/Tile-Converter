#pragma once

#include "Utils/Json.hpp"
#include <glm.hpp>

class Mod;

class PartListLoader
{
	PartListLoader() = default;

	static glm::vec3 LoadPartCollision(const nlohmann::json& collision);
public:
	static void Load(const nlohmann::json& fParts, Mod* mod);
};