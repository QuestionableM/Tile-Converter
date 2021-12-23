#pragma once

#include "Utils/Uuid.hpp"
#include "Utils/Color.hpp"
#include "Tile/Object/TileEntity.hpp"
#include "ObjectDatabase/ModelStorage.hpp"
#include "ObjectDatabase/ObjectData.hpp"

#include "Console.hpp"

#include <unordered_map>

class Asset : public TileEntity
{
	friend class TilePart;
	friend class AssetListReader;

	Model* pModel;
	AssetData* pParent;
	std::unordered_map<std::wstring /*material*/, Color> mColors;

public:
	Asset() = default;
	Asset(const Asset&) = delete;
	Asset(Asset&&) = delete;

	Model* GetModel() const
	{
		return pModel;
	}

	void AddMaterial(const std::wstring& mat_name, const Color& color)
	{
		if (mColors.find(mat_name) != mColors.end())
			return;

		mColors.insert(std::make_pair(mat_name, color));
	}

	std::unordered_map<std::wstring, Color> GetMaterials() const
	{
		return this->mColors;
	}

	Color GetColor(const std::wstring& color) const
	{
		if (mColors.find(color) != mColors.end())
			return mColors.at(color);

		if (pParent->DefaultColors.find(color) != pParent->DefaultColors.end())
			return pParent->DefaultColors.at(color);

		return 0x000000;
	}

	std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const override
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
};