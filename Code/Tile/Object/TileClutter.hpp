#pragma once

#include "Tile\Object\TileEntity.hpp"

class TileClutter : public TileEntity
{
	class ClutterData* pParent;
	Color color = 0xffffff;

public:
	inline TileClutter(ClutterData* pParent, Model* pModel)
	{
		this->pParent = pParent;
		this->uuid = pParent->Uuid;
		this->pModel = pModel;
	}

	TileClutter(TileClutter&) = delete;
	TileClutter(const TileClutter&) = delete;
	~TileClutter() = default;

	inline const float& ScaleVariance() const { return pParent->ScaleVariance; }

	inline void SetColor(const Color& color) { this->color = color; }

	inline EntityType Type() const override { return EntityType::Clutter; }
	std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const override;
	void FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const override;
};