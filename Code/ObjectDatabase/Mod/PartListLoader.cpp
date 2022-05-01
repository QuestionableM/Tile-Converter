#include "PartListLoader.hpp"

#include "ObjectDatabase/Mod/DefaultLoader.hpp"
#include "ObjectDatabase/Mod/Mod.hpp"
#include "ObjectDatabase/ProgCounter.hpp"

#include "Console.hpp"

glm::vec3 PartListLoader::LoadPartCollision(const nlohmann::json& collision)
{
	glm::vec3 out_coll(1.0f);

	bool isBoxCol = collision.contains("box");
	bool isHullCol = collision.contains("hull");
	if (isBoxCol || isHullCol)
	{
		const auto& b_Col = collision.at(isBoxCol ? "box" : "hull");

		if (b_Col.is_object())
		{
			const auto& xPos = JsonReader::Get(b_Col, "x");
			const auto& yPos = JsonReader::Get(b_Col, "y");
			const auto& zPos = JsonReader::Get(b_Col, "z");

			if (xPos.is_number() && yPos.is_number() && zPos.is_number())
				out_coll = glm::vec3(xPos.get<float>(), yPos.get<float>(), zPos.get<float>());
		}
	}
	else
	{
		const auto& cyl_col = JsonReader::Get(collision, "cylinder");
		if (cyl_col.is_object())
		{
			const auto& c_Diameter = JsonReader::Get(cyl_col, "diameter");
			const auto& c_Depth = JsonReader::Get(cyl_col, "depth");

			if (c_Diameter.is_number() && c_Depth.is_number())
			{
				float f_Diameter = c_Diameter.get<float>();
				float f_Depth = c_Depth.get<float>();

				const auto& c_Axis = JsonReader::Get(cyl_col, "axis");
				std::string c_AxisStr = (c_Axis.is_string() ? c_Axis.get<std::string>() : "z");

				if (c_AxisStr == "x" || c_AxisStr == "X")
					out_coll = glm::vec3(f_Depth, f_Diameter, f_Diameter);
				else if (c_AxisStr == "y" || c_AxisStr == "Y")
					out_coll = glm::vec3(f_Diameter, f_Depth, f_Diameter);
				else if (c_AxisStr == "z" || c_AxisStr == "Z")
					out_coll = glm::vec3(f_Diameter, f_Diameter, f_Depth);
			}
		}
		else
		{
			const auto& sphere_col = JsonReader::Get(collision, "sphere");
			if (sphere_col.is_object())
			{
				const auto& s_Diameter = JsonReader::Get(sphere_col, "diameter");
				if (s_Diameter.is_number())
					out_coll = glm::vec3(s_Diameter.get<float>());
			}
		}
	}

	return out_coll;
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

		SMUuid part_uuid = pUuid.get<std::string>();
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
		new_part->DefaultColor = pColor.get<std::string>();
		new_part->Bounds = PartListLoader::LoadPartCollision(fPart);

		const auto new_pair = std::make_pair(new_part->Uuid, new_part);

		Mod::PartStorage.insert(new_pair);
		mod->Parts.insert(new_pair);

		ProgCounter::ProgressValue++;
	}
}