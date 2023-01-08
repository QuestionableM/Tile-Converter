#include "Harvestable.hpp"

#include "ObjectDatabase\MaterialManager.hpp"

std::string Harvestable::GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const
{
	const SubMeshData* pSubMesh = pModel->subMeshData[mIdx];
	const std::wstring tex_name = (pParent->Textures.Type() == TextureDataType::SubMeshList ? std::to_wstring(mIdx) : pSubMesh->m_MaterialName);

	std::string material_idx = "m1";

	ObjectTexData oTexData;
	if (pParent->Textures.GetEntry(tex_name, oTexData.Textures))
		material_idx = MaterialManager::GetMaterialA(oTexData.Textures.material);

	return uuid.ToString() + " " + color.StringHex() + " " + std::to_string(mIdx + 1) + " " + material_idx;
}

void Harvestable::FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const
{
	const std::string mtl_first_part = this->uuid.ToString() + " " + this->color.StringHex() + " ";
	for (std::size_t a = 0; a < pModel->subMeshData.size(); a++)
	{
		const SubMeshData* pSubMesh = pModel->subMeshData[a];
		const std::wstring tex_name = (pParent->Textures.Type() == TextureDataType::SubMeshList ? std::to_wstring(a) : pSubMesh->m_MaterialName);

		ObjectTexData oTexData;
		if (pParent->Textures.GetEntry(tex_name, oTexData.Textures))
		{
			oTexData.TexColor = this->color;

			const std::string mat_idx = MaterialManager::GetMaterialA(oTexData.Textures.material);
			const std::string mat_name = mtl_first_part + std::to_string(a + 1) + " " + mat_idx;

			if (tex_map.find(mat_name) != tex_map.end())
				continue;

			tex_map.insert(std::make_pair(mat_name, oTexData));
		}
	}
}