#include "TileClutter.h"

#include "ObjectDatabase/Mod/MaterialManager.hpp"

TileClutter::TileClutter(ClutterData* pParent, Model* pModel)
{
	this->pParent = pParent;
	this->uuid = pParent->Uuid;
	this->pModel = pModel;
}

const float& TileClutter::ScaleVariance() const
{
	return pParent->ScaleVariance;
}

void TileClutter::SetColor(const Color& color)
{
	this->color = color;
}

EntityType TileClutter::Type() const
{
	return EntityType::Clutter;
}

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