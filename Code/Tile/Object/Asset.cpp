#include "Asset.hpp"

#include "ObjectDatabase/ObjectData.hpp"
#include "ObjectDatabase/ModelStorage.hpp"
#include "ObjectDatabase/Mod/MaterialManager.hpp"

Color Asset::GetColor(const std::wstring& color) const
{
	{
		const ColorMap::const_iterator v_iter = m_Colors.find(color);
		if (v_iter != m_Colors.end())
			return v_iter->second;
	}

	{
		const ColorMap::const_iterator v_iter = pParent->DefaultColors.find(color);
		if (v_iter != pParent->DefaultColors.end())
			return v_iter->second;
	}

	return 0x000000;
}

std::string Asset::GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const
{
	const std::wstring mName = (pParent->Textures.Type() == TextureDataType::SubMeshList ? std::to_wstring(mIdx) : mat_name);

	Color sColor = 0x000000;

	std::string mat_idx = "m1";

	TextureList tList;
	if (pParent->Textures.GetEntry(mName, tList))
	{
		sColor = this->GetColor(tList.def_color_idx);
		mat_idx = MaterialManager::GetMaterialA(tList.material);
	}

	return uuid.ToString() + " " + sColor.StringHex() + " " + std::to_string(mIdx + 1) + " " + mat_idx;
}

void Asset::FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const
{
	const std::string mtl_first_part = this->uuid.ToString() + " ";
	for (std::size_t a = 0; a < pModel->subMeshData.size(); a++)
	{
		const SubMeshData* pSubMesh = pModel->subMeshData[a];
		const std::wstring tex_name = (pParent->Textures.Type() == TextureDataType::SubMeshList ? std::to_wstring(a) : pSubMesh->m_MaterialName);

		ObjectTexData oTexData;
		if (pParent->Textures.GetEntry(tex_name, oTexData.Textures))
		{
			oTexData.TexColor = this->GetColor(oTexData.Textures.def_color_idx);

			const std::string mat_idx = MaterialManager::GetMaterialA(oTexData.Textures.material);
			const std::string mtl_name = mtl_first_part + oTexData.TexColor.StringHex() + " " + std::to_string(a + 1) + " " + mat_idx;

			if (tex_map.find(mtl_name) != tex_map.end())
				continue;

			tex_map.insert(std::make_pair(mtl_name, oTexData));
		}
	}
}