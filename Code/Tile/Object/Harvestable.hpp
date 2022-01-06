#pragma once

#include "Tile/Object/TileEntity.hpp"

#include "Utils/Color.hpp"
#include "ObjectDatabase/ModelStorage.hpp"

class Harvestable : public TileEntity
{
	Color color;
	class HarvestableData* pParent;

public:
	Harvestable(HarvestableData* pParent, Model* pModel, const Color& color);
	Harvestable(const Harvestable&) = delete;
	Harvestable(Harvestable&) = delete;
	~Harvestable() = default;

	Color GetColor() const;

	EntityType Type() const;
	std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const override;
	void FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const override;
};