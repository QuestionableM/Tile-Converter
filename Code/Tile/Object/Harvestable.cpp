#include "Harvestable.hpp"

void Harvestable::SetColor(const Color& color)
{
	this->color = color;
}

Color Harvestable::GetColor() const
{
	return this->color;
}

EntityType Harvestable::Type() const
{
	return EntityType::Harvestable;
}

std::string Harvestable::GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const
{
	return uuid.ToString() + " " + color.StringHex() + " " + std::to_string(mIdx + 1);
}

void Harvestable::FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const
{
	for (std::size_t a = 0; a < pModel->subMeshData.size(); a++)
	{
		const std::string mtl_name = this->uuid.ToString() + " " + this->color.StringHex() + " " + std::to_string(a + 1);

		if (tex_map.find(mtl_name) != tex_map.end())
			continue;

		const SubMeshData* pSubMesh = pModel->subMeshData[a];
		const std::wstring tex_name = (pParent->Textures.Type() == TextureDataType::SubMeshList ? std::to_wstring(a) : pSubMesh->MaterialName);

		ObjectTexData oTexData;
		if (pParent->Textures.GetEntry(tex_name, oTexData.Textures))
		{
			oTexData.TexColor = this->color;

			tex_map.insert(std::make_pair(mtl_name, oTexData));
		}
	}
}