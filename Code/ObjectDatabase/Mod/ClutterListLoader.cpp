#include "ClutterListLoader.hpp"

#include "ObjectDatabase/KeywordReplacer.hpp"
#include "ObjectDatabase/Mod/Mod.hpp"
#include "ObjectDatabase/ProgCounter.hpp"

#include "Utils/String.hpp"
#include "Console.hpp"

bool ClutterListLoader::LoadTextureData(const nlohmann::json& fClutter, TextureList& tList, std::wstring& mesh)
{
	const auto& clDif = JsonReader::Get(fClutter, "texture");
	const auto& clMaterial = JsonReader::Get(fClutter, "material");
	if (!clDif.is_string()) return false;

	const auto& clMesh = JsonReader::Get(fClutter, "mesh");
	if (!clMesh.is_string()) return false;
	
	const std::wstring clDifWide = String::ToWide(clDif.get<std::string>());
	tList.dif = KeywordReplacer::ReplaceKey(clDifWide);
	tList.material = (clMaterial.is_string() ? String::ToWide(clMaterial.get<std::string>()) : L"GroundClutter");

	const std::wstring mPathWide = String::ToWide(clMesh.get<std::string>());
	mesh = KeywordReplacer::ReplaceKey(mPathWide);

	return true;
}

void ClutterListLoader::LoadClutterData(const nlohmann::json& fClutter, ClutterData* pClutter)
{
	const auto& clHeight = JsonReader::Get(fClutter, "height");
	const auto& clScaleVariance = JsonReader::Get(fClutter, "scaleVariance");
	const auto& clGroundNormal = JsonReader::Get(fClutter, "groundNormal");

	pClutter->Height = (clHeight.is_number() ? clHeight.get<float>() : 1.0f);
	pClutter->ScaleVariance = (clScaleVariance.is_number() ? clScaleVariance.get<float>() : 0.0f);
	pClutter->GroundNormal = (clGroundNormal.is_boolean() ? clGroundNormal.get<bool>() : false);
}

void ClutterListLoader::Load(const nlohmann::json& fClutter, Mod* mod)
{
	if (!fClutter.is_array()) return;

	ProgCounter::ProgressMax += fClutter.size();
	for (const auto& cl_item : fClutter)
	{
		if (!cl_item.is_object()) continue;

		const auto& clUuid = JsonReader::Get(cl_item, "uuid");
		if (!clUuid.is_string()) continue;

		SMUuid clutter_uuid = clUuid.get<std::string>();
		if (Mod::ClutterStorage.find(clutter_uuid) != Mod::ClutterStorage.end())
		{
			DebugErrorL("Clutter with the specified uuid already exists! (", clutter_uuid.ToString(), ")");
			continue;
		}

		TextureList tList;
		std::wstring clMesh;
		if (!ClutterListLoader::LoadTextureData(cl_item, tList, clMesh))
		{
			DebugErrorL("Couldn't load the texture data for: ", clutter_uuid.ToString());
			continue;
		}

		ClutterData* new_clutter = new ClutterData();
		new_clutter->Uuid = clutter_uuid;
		new_clutter->Mesh = clMesh;
		new_clutter->Textures = tList;
		new_clutter->pMod = mod;

		ClutterListLoader::LoadClutterData(cl_item, new_clutter);

		const auto new_pair = std::make_pair(new_clutter->Uuid, new_clutter);

		mod->m_Clutter.insert(new_pair);
		Mod::ClutterStorage.insert(new_pair);
		Mod::ClutterVector.push_back(new_clutter);

		ProgCounter::ProgressValue++;
	}
}