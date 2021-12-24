#pragma once

#include "Tile/Object/TileEntity.hpp"

#include "Utils/Color.hpp"
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

	void SetColor(const Color& color);

	Model* GetModel() const;
	Color GetColor() const;
	std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const override;
};