#include "Asset.hpp"

#include "ObjectDatabase/ObjectData.hpp"

Model* Asset::GetModel() const
{
	return pModel;
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

void Asset::AddMaterial(const std::wstring& mat_name, const Color& color)
{
	if (mColors.find(mat_name) != mColors.end())
		return;

	mColors.insert(std::make_pair(mat_name, color));
}