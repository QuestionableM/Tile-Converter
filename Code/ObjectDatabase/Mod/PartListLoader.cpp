#include "PartListLoader.hpp"

#include "ObjectDatabase/Mod/DefaultLoader.hpp"
#include "ObjectDatabase/Mod/Mod.hpp"
#include "ObjectDatabase/ProgCounter.hpp"

#include "Console.hpp"

const PartListLoader::__CollisionLoaderData PartListLoader::g_collisionDataLoaders[] =
{
	{ "box"     , PartListLoader::LoadBoxCollision      },
	{ "hull"    , PartListLoader::LoadBoxCollision      },
	{ "cylinder", PartListLoader::LoadCylinderCollision },
	{ "sphere"  , PartListLoader::LoadSphereCollision   }
};

void PartListLoader::LoadBoxCollision(const nlohmann::json& collision, glm::vec3& vec_ref)
{
	const auto& v_coll_x = JsonReader::Get(collision, "x");
	const auto& v_coll_y = JsonReader::Get(collision, "y");
	const auto& v_coll_z = JsonReader::Get(collision, "z");

	if (v_coll_x.is_number() && v_coll_y.is_number() && v_coll_z.is_number())
		vec_ref = glm::vec3(v_coll_x.get<float>(), v_coll_y.get<float>(), v_coll_z.get<float>());
}

void PartListLoader::LoadCylinderCollision(const nlohmann::json& collision, glm::vec3& vec_ref)
{
	const auto& v_diameter = JsonReader::Get(collision, "diameter");
	const auto& v_depth = JsonReader::Get(collision, "depth");

	if (!v_diameter.is_number() || !v_depth.is_number())
		return;

	const auto& v_axis = JsonReader::Get(collision, "axis");
	const std::string v_axis_str = (v_axis.is_string() ? v_axis.get_ref<const std::string&>() : "z");

	if (v_axis_str.empty())
		return;

	const float v_diameter_f = v_diameter.get<float>();
	const float v_depth_f = v_depth.get<float>();

	const char v_axis_char = std::tolower(v_axis_str[0]);
	switch (v_axis_char)
	{
	case 'x':
		vec_ref = glm::vec3(v_depth_f, v_diameter_f, v_diameter_f);
		break;
	case 'y':
		vec_ref = glm::vec3(v_diameter_f, v_depth_f, v_diameter_f);
		break;
	case 'z':
		vec_ref = glm::vec3(v_diameter_f, v_diameter_f, v_depth_f);
		break;
	}
}

void PartListLoader::LoadSphereCollision(const nlohmann::json& collision, glm::vec3& vec_ref)
{
	const auto& v_diameter = JsonReader::Get(collision, "diameter");
	if (v_diameter.is_number())
		vec_ref = glm::vec3(v_diameter.get<float>());
}

glm::vec3 PartListLoader::LoadPartCollision(const nlohmann::json& collision)
{
	glm::vec3 v_out_collision(1.0f);

	for (unsigned __int8 a = 0; a < 4; a++)
	{
		const __CollisionLoaderData& v_curData = g_collisionDataLoaders[a];

		const auto& v_collisionData = JsonReader::Get(collision, v_curData.key);
		if (!v_collisionData.is_object())
			continue;

		v_curData.func_ptr(v_collisionData, v_out_collision);
		break;
	}

	return v_out_collision;
}

void PartListLoader::Load(const nlohmann::json& fParts, Mod* mod)
{
	if (!fParts.is_array()) return;

	ProgCounter::ProgressMax += fParts.size();
	for (const auto& fPart : fParts)
	{
		if (!fPart.is_object()) continue;

		const auto& pUuid = JsonReader::Get(fPart, "uuid");
		const auto& pColor = JsonReader::Get(fPart, "color");

		if (!pUuid.is_string()) continue;

		const SMUuid part_uuid(pUuid.get_ref<const std::string&>());
		if (Mod::PartStorage.find(part_uuid) != Mod::PartStorage.end())
		{
			DebugWarningL("Part with the uuid already exists! (", part_uuid.ToString(), ")");
			continue;
		}

		std::wstring mesh_path;
		TextureData tex_data;
		if (!DefaultLoader::LoadRenderable(fPart, tex_data, mesh_path))
			continue;

		PartData* new_part = new PartData();
		new_part->Mesh = mesh_path;
		new_part->Textures = tex_data;
		new_part->Uuid = part_uuid;
		new_part->pMod = mod;
		new_part->DefaultColor = (pColor.is_string() ? pColor.get_ref<const std::string&>() : "375000");
		new_part->Bounds = PartListLoader::LoadPartCollision(fPart);

		const auto new_pair = std::make_pair(new_part->Uuid, new_part);

		Mod::PartStorage.insert(new_pair);
		mod->m_Parts.insert(new_pair);

		ProgCounter::ProgressValue++;
	}
}