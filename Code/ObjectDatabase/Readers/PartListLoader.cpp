#include "PartListLoader.hpp"

#include "ObjectDatabase\Readers\DefaultLoader.hpp"
#include "ObjectDatabase\ProgCounter.hpp"
#include "ObjectDatabase\Mods\Mod.hpp"

#include "Console.hpp"

const PartListLoader::__CollisionLoaderData PartListLoader::g_collisionDataLoaders[] =
{
	{ "box"     , PartListLoader::LoadBoxCollision      },
	{ "hull"    , PartListLoader::LoadBoxCollision      },
	{ "cylinder", PartListLoader::LoadCylinderCollision },
	{ "sphere"  , PartListLoader::LoadSphereCollision   },
	{ "slant"   , PartListLoader::LoadBoxCollision      }
};

void PartListLoader::LoadBoxCollision(const simdjson::dom::element& collision, glm::vec3& vec_ref)
{
	const auto v_coll_x = collision["x"];
	const auto v_coll_y = collision["y"];
	const auto v_coll_z = collision["z"];

	if (v_coll_x.is_number() && v_coll_y.is_number() && v_coll_z.is_number())
		vec_ref = glm::vec3(JsonReader::GetNumber<float>(v_coll_x), JsonReader::GetNumber<float>(v_coll_y), JsonReader::GetNumber<float>(v_coll_z));
}

void PartListLoader::LoadCylinderCollision(const simdjson::dom::element& collision, glm::vec3& vec_ref)
{
	const auto v_diameter = collision["diameter"];
	const auto v_depth = collision["depth"];

	if (!(v_diameter.is_number() && v_depth.is_number()))
		return;

	char v_axis_char = 'z';

	const auto v_axis = collision["axis"];
	if (v_axis.is_string())
	{
		const std::string_view v_axis_str = v_axis.get_string();
		if (v_axis_str.size() > 0)
			v_axis_char = v_axis_str.data()[0];
	}

	const float v_diameter_f = JsonReader::GetNumber<float>(v_diameter);
	const float v_depth_f = JsonReader::GetNumber<float>(v_depth);

	switch (v_axis_char)
	{
	case 'x': vec_ref = glm::vec3(v_depth_f   , v_diameter_f, v_diameter_f); break;
	case 'y': vec_ref = glm::vec3(v_diameter_f, v_depth_f   , v_diameter_f); break;
	case 'z': vec_ref = glm::vec3(v_diameter_f, v_diameter_f, v_depth_f   ); break;
	}
}

void PartListLoader::LoadSphereCollision(const simdjson::dom::element& collision, glm::vec3& vec_ref)
{
	const auto v_diameter = collision["diameter"];
	if (v_diameter.is_number())
		vec_ref = glm::vec3(JsonReader::GetNumber<float>(v_diameter));
}

glm::vec3 PartListLoader::LoadPartCollision(const simdjson::dom::element& collision)
{
	glm::vec3 v_out_collision(1.0f);

	const unsigned __int8 v_collisionDataLoadersSz = sizeof(g_collisionDataLoaders) / sizeof(__CollisionLoaderData);
	for (unsigned __int8 a = 0; a < v_collisionDataLoadersSz; a++)
	{
		const __CollisionLoaderData& v_curData = g_collisionDataLoaders[a];

		const auto v_collision_data = collision[v_curData.key];
		if (!v_collision_data.is_object())
			continue;

		v_curData.func_ptr(v_collision_data.value_unsafe(), v_out_collision);
		break;
	}

	return v_out_collision;
}

void PartListLoader::Load(const simdjson::dom::element& fParts, Mod* mod)
{
	if (!fParts.is_array()) return;

	const auto v_prt_array = fParts.get_array();
	ProgCounter::ProgressMax += v_prt_array.size();

	for (const auto v_prt : v_prt_array)
	{
		if (!v_prt.is_object()) continue;

		const auto v_uuid = v_prt["uuid"];
		const auto v_color = v_prt["color"];

		if (!v_uuid.is_string()) continue;

		const SMUuid v_prt_uuid = v_uuid.get_c_str();
		if (Mod::PartStorage.find(v_prt_uuid) != Mod::PartStorage.end())
		{
			DebugWarningL("Part with the same uuid already exists! (", v_prt_uuid.ToString(), ")");
			continue;
		}

		std::wstring v_mesh_path;
		TextureData v_tex_data;
		if (!DefaultLoader::LoadRenderable(v_prt, v_tex_data, v_mesh_path))
			continue;

		PartData* v_new_part = new PartData();
		v_new_part->Mesh = v_mesh_path;
		v_new_part->Textures = v_tex_data;
		v_new_part->Uuid = v_prt_uuid;
		v_new_part->pMod = mod;
		v_new_part->DefaultColor = (v_color.is_string() ? v_color.get_c_str() : "375000");
		v_new_part->Bounds = PartListLoader::LoadPartCollision(v_prt);

		const auto v_new_pair = std::make_pair(v_new_part->Uuid, v_new_part);
		Mod::PartStorage.insert(v_new_pair);
		mod->m_Parts.insert(v_new_pair);

		ProgCounter::ProgressValue++;
	}
}