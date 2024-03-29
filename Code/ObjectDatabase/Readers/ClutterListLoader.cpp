#include "ClutterListLoader.hpp"

#include "ObjectDatabase\KeywordReplacer.hpp"
#include "ObjectDatabase\ProgCounter.hpp"
#include "ObjectDatabase\Mods\Mod.hpp"

#include "Utils\String.hpp"
#include "Console.hpp"

bool ClutterListLoader::LoadTextureData(const simdjson::dom::element& fClutter, TextureList& tList, std::wstring& mesh)
{
	const auto v_cl_dif = fClutter["texture"];
	if (!v_cl_dif.is_string()) return false;

	const auto v_cl_mesh = fClutter["mesh"];
	if (!v_cl_mesh.is_string()) return false;

	const std::wstring v_cl_dif_wstr = String::ToWide(v_cl_dif.get_string());
	tList.dif = KeywordReplacer::ReplaceKey(v_cl_dif_wstr);

	const auto v_cl_material = fClutter["material"];
	tList.material = (v_cl_material.is_string() ? String::ToWide(v_cl_material.get_string()) : L"GroundVegetation");

	const std::wstring v_cl_mesh_wstr = String::ToWide(v_cl_mesh.get_string());
	mesh = KeywordReplacer::ReplaceKey(v_cl_mesh_wstr);

	return true;
}

void ClutterListLoader::LoadClutterData(const simdjson::dom::element& fClutter, ClutterData* pClutter)
{
	const auto v_cl_height = fClutter["height"];
	const auto v_cl_scale_variance = fClutter["scaleVariance"];
	const auto v_cl_ground_normal = fClutter["groundNormal"];

	pClutter->Height = (v_cl_height.is_number() ? JsonReader::GetNumber<float>(v_cl_height) : 1.0f);
	pClutter->ScaleVariance = (v_cl_scale_variance.is_number() ? JsonReader::GetNumber<float>(v_cl_scale_variance) : 0.0f);
	pClutter->GroundNormal = (v_cl_ground_normal.is_bool() ? v_cl_ground_normal.get_bool() : false);
}

void ClutterListLoader::Load(const simdjson::dom::element& fClutter, Mod* mod)
{
	if (!fClutter.is_array()) return;

	const auto v_clutter_array = fClutter.get_array();
	ProgCounter::ProgressMax += v_clutter_array.size();

	for (const auto v_clutter : v_clutter_array)
	{
		if (!v_clutter.is_object()) continue;

		const auto v_uuid = v_clutter["uuid"];
		if (!v_uuid.is_string()) continue;

		const SMUuid v_clutter_uuid = v_uuid.get_c_str();
		if (Mod::ClutterStorage.find(v_clutter_uuid) != Mod::ClutterStorage.end())
		{
			DebugErrorL("Clutter with the specified uuid already exists! (", v_clutter_uuid.ToString(), ")");
			continue;
		}

		TextureList v_tList;
		std::wstring v_tMesh;
		if (!ClutterListLoader::LoadTextureData(v_clutter, v_tList, v_tMesh))
		{
			DebugErrorL("Couldn't load the texture data for: ", v_clutter_uuid.ToString());
			continue;
		}

		ClutterData* v_new_clutter = new ClutterData();
		v_new_clutter->Uuid = v_clutter_uuid;
		v_new_clutter->Mesh = v_tMesh;
		v_new_clutter->Textures = v_tList;
		v_new_clutter->pMod = mod;

		ClutterListLoader::LoadClutterData(v_clutter, v_new_clutter);

		const auto v_new_pair = std::make_pair(v_new_clutter->Uuid, v_new_clutter);
		mod->m_Clutter.insert(v_new_pair);
		Mod::ClutterStorage.insert(v_new_pair);
		Mod::ClutterVector.push_back(v_new_clutter);

		ProgCounter::ProgressValue++;
	}
}