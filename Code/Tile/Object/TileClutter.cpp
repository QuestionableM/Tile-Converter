#include "TileClutter.h"

EntityType TileClutter::Type() const
{
	return EntityType::Clutter;
}

std::string	TileClutter::GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const
{
	return pParent->Uuid.ToString() + " " + std::to_string(mIdx + 1);
}

void TileClutter::FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const
{
	const std::string mtl_name = pParent->Uuid.ToString() + " " + std::to_string(1);

	if (tex_map.find(mtl_name) != tex_map.end())
		return;

	ObjectTexData oTexData;
	oTexData.Textures = pParent->Textures;
	oTexData.TexColor = 0x00ff00; //Make a proper color implementation later

	tex_map.insert(std::make_pair(mtl_name, oTexData));
}