#pragma once

#include "Utils/Json.hpp"
#include <glm.hpp>

class Mod;

class PartListLoader
{
	PartListLoader() = default;
	~PartListLoader() = default;

	static void LoadBoxCollision(const nlohmann::json& collision, glm::vec3& vec_ref);
	static void LoadCylinderCollision(const nlohmann::json& collision, glm::vec3& vec_ref);
	static void LoadSphereCollision(const nlohmann::json& collision, glm::vec3& vec_ref);

	static glm::vec3 LoadPartCollision(const nlohmann::json& collision);
public:
	static void Load(const nlohmann::json& fParts, Mod* mod);

private:
	struct __CollisionLoaderData
	{
		std::string key;
		void (*func_ptr)(const nlohmann::json&, glm::vec3&);
	};

	static const __CollisionLoaderData g_collisionDataLoaders[];
};