#include "AssetListLoader.hpp"
#include "ObjectDatabase/ObjectDatabase.hpp"

#include "Console.hpp"

void AssetListLoader::LoadDefaultColors(const nlohmann::json& jAsset, std::unordered_map<std::string, Color>& def_colors)
{
	const auto& aDefColors = JsonReader::Get(jAsset, "defaultColors");
	if (!aDefColors.is_object()) return;

	for (const auto& def_color : aDefColors.items())
	{
		if (!def_color.value().is_string()) continue;

		std::string color_val = def_color.value().get<std::string>();
		if (color_val.size() < 6)
			color_val = "000000";

		const std::string key_str = def_color.key();
		const Color key_color(color_val);

		def_colors.insert(std::make_pair(key_str, key_color));
	}
}

void AssetListLoader::Load(const nlohmann::json& fAssets)
{
	if (!fAssets.is_array()) return;
	DebugOutL("Loading AssetList...");

	for (const auto& mAsset : fAssets)
	{
		if (!mAsset.is_object()) continue;

		const auto& aUuid = JsonReader::Get(mAsset, "uuid");
		if (!aUuid.is_string()) continue;

		std::wstring tMesh;
		TextureData tData;
		if (!DatabaseLoader::LoadRenderable(mAsset, tData, tMesh))
			continue;

		AssetData* new_asset = new AssetData();
		new_asset->Uuid = aUuid.get<std::string>();
		new_asset->Mesh = tMesh;
		AssetListLoader::LoadDefaultColors(mAsset, new_asset->DefaultColors);
		new_asset->Textures = tData;

		DatabaseLoader::Assets.insert(std::make_pair(new_asset->Uuid, new_asset));
	}
}