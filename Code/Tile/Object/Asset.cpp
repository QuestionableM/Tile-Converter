#include "Asset.hpp"

#include "ObjectDatabase/ObjectData.hpp"
#include "ObjectDatabase/ModelStorage.hpp"

Asset::Asset(AssetData* pParent, Model* pModel, const std::unordered_map<std::wstring, Color>& color_map)
{
	this->pParent = pParent;
	this->uuid = pParent->Uuid;
	this->pModel = pModel;
	this->mColors = color_map;
}

Color Asset::GetColor(const std::wstring& color) const
{
	if (mColors.find(color) != mColors.end())
		return mColors.at(color);

	if (pParent->DefaultColors.find(color) != pParent->DefaultColors.end())
		return pParent->DefaultColors.at(color);

	return 0x000000;
}

std::unordered_map<std::wstring, Color> Asset::GetMaterials() const
{
	return this->mColors;
}

EntityType Asset::Type() const
{
	return EntityType::Asset;
}

std::string Asset::GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const
{
	const std::wstring mName = (pParent->Textures.Type() == TextureDataType::SubMeshList ? std::to_wstring(mIdx) : mat_name);

	Color sColor = 0x000000;

	TextureList tList;
	if (pParent->Textures.GetEntry(mName, tList))
	{
		sColor = this->GetColor(tList.def_color_idx);
	}

	return uuid.ToString() + " " + sColor.StringHex() + " " + std::to_string(mIdx + 1);
}

void Asset::FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const
{
	for (std::size_t a = 0; a < pModel->subMeshData.size(); a++)
	{
		const SubMeshData* pSubMesh = pModel->subMeshData[a];
		const std::wstring tex_name = (pParent->Textures.Type() == TextureDataType::SubMeshList ? std::to_wstring(a) : pSubMesh->MaterialName);

		ObjectTexData oTexData;
		if (pParent->Textures.GetEntry(tex_name, oTexData.Textures))
		{
			oTexData.TexColor = this->GetColor(oTexData.Textures.def_color_idx);

			const std::string mtl_name = this->uuid.ToString() + " " + oTexData.TexColor.StringHex() + " " + std::to_string(a + 1);

			if (tex_map.find(mtl_name) != tex_map.end())
				continue;

			tex_map.insert(std::make_pair(mtl_name, oTexData));
		}
	}
}