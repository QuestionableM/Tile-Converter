#pragma once

#include "Tile/Object/TileEntity.hpp"
#include "ObjectDatabase/ObjectData.hpp"
#include "ObjectDatabase/ModelStorage.hpp"

class Harvestable : public TileEntity
{
	friend class HarvestableListReader;

	Color color;
	Model* pModel;

public:
	Harvestable() = default;
	Harvestable(const Harvestable&) = delete;
	Harvestable(Harvestable&) = delete;

	Model* GetModel() const
	{
		return this->pModel;
	}

	void SetColor(const Color& color)
	{
		this->color = color;
	}

	Color GetColor() const
	{
		return this->color;
	}

	std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const override
	{
		return uuid.ToString() + " " + color.StringHex() + " " + std::to_string(mIdx + 1);
	}
};