#include "TileClutter.h"

#include "ObjectDatabase/Mod/MaterialManager.hpp"

std::string	TileClutter::GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const
{
	const std::string tex_mat = MaterialManager::GetMaterialA(pParent->Textures.material);

	return this->uuid.ToString() + " " + this->color.StringHex() + " " + std::to_string(mIdx + 1) + " " + tex_mat;
}

void TileClutter::FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const
{
	const std::string mtl_name = this->GetMtlName(L"", 0);

	if (tex_map.find(mtl_name) != tex_map.end())
		return;

	ObjectTexData oTexData;
	oTexData.Textures = pParent->Textures;
	oTexData.TexColor = this->color;

	tex_map.insert(std::make_pair(mtl_name, oTexData));
}