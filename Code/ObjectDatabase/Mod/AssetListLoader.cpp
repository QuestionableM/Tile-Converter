#include "AssetListLoader.hpp"
#include "ObjectDatabase/Mod/DefaultLoader.hpp"

#include "Utils/String.hpp"
#include "ObjectDatabase/Mod/Mod.hpp"
#include "ObjectDatabase/ProgCounter.hpp"

#include "Console.hpp"

void AssetListLoader::LoadDefaultColors(const nlohmann::json& jAsset, std::unordered_map<std::wstring, Color>& def_colors)
{
	const auto& aDefColors = JsonReader::Get(jAsset, "defaultColors");
	if (!aDefColors.is_object()) return;

	for (const auto& def_color : aDefColors.items())
	{
		if (!def_color.value().is_string()) continue;

		std::string color_val = def_color.value().get<std::string>();
		if (color_val.size() < 6)
			color_val = "000000";

		const std::wstring key_str = String::ToWide(def_color.key());

		def_colors.insert(std::make_pair(key_str, color_val));
	}
}

void AssetListLoader::Load(const nlohmann::json& fAssets, Mod* mod)
{
	if (!fAssets.is_array()) return;

	ProgCounter::ProgressMax += fAssets.size();
	for (const auto& mAsset : fAssets)
	{
		if (!mAsset.is_object()) continue;

		const auto& aUuid = JsonReader::Get(mAsset, "uuid");
		if (!aUuid.is_string()) continue;

		SMUuid asset_uuid = aUuid.get<std::string>();
		if (Mod::AssetStorage.find(asset_uuid) != Mod::AssetStorage.end())
		{
			DebugWarningL("Asset with the same uuid already exists! (", asset_uuid.ToString(), ")");
			continue;
		}

		std::wstring tMesh;
		TextureData tData;
		if (!DefaultLoader::LoadRenderable(mAsset, tData, tMesh))
			continue;

		AssetData* new_asset = new AssetData();
		new_asset->Uuid = asset_uuid;
		new_asset->Mesh = tMesh;
		AssetListLoader::LoadDefaultColors(mAsset, new_asset->DefaultColors);
		new_asset->Textures = tData;
		new_asset->pMod = mod;

		const auto new_pair = std::make_pair(new_asset->Uuid, new_asset);

		mod->Assets.insert(new_pair);
		Mod::AssetStorage.insert(new_pair);

		ProgCounter::ProgressValue++;
	}
}