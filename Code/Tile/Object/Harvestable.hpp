#pragma once

#include "Tile/Object/TileEntity.hpp"

#include "Utils/Color.hpp"
#include "ObjectDatabase/ModelStorage.hpp"

class Harvestable : public TileEntity
{
	Color color;
	class HarvestableData* pParent;

public:
	inline Harvestable(HarvestableData* pParent, Model* pModel, const Color& color)
	{
		this->pParent = pParent;
		this->uuid = pParent->Uuid;
		this->pModel = pModel;
		this->color = color;
	}

	Harvestable(const Harvestable&) = delete;
	Harvestable(Harvestable&) = delete;
	~Harvestable() = default;

	inline Color GetColor() const { return this->color; }

	inline EntityType Type() const { return EntityType::Harvestable; }
	std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const override;
	void FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const override;
};