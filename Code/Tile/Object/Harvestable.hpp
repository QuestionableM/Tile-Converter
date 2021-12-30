#pragma once

#include "Tile/Object/TileEntity.hpp"

#include "Utils/Color.hpp"
#include "ObjectDatabase/ModelStorage.hpp"

class Harvestable : public TileEntity
{
	friend class HarvestableListReader;
	friend class Tile;

	Color color;
	class HarvestableData* pParent;

public:
	Harvestable() = default;
	Harvestable(const Harvestable&) = delete;
	Harvestable(Harvestable&) = delete;
	~Harvestable() = default;

	void SetColor(const Color& color);
	Color GetColor() const;

	EntityType Type() const;
	std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const override;
	void FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const override;
};