#pragma once

#include "Tile/Object/TileEntity.hpp"

class TileClutter : public TileEntity
{
	class ClutterData* pParent;

public:
	TileClutter(ClutterData* pParent, Model* pModel);
	TileClutter(TileClutter&) = delete;
	TileClutter(const TileClutter&) = delete;
	~TileClutter() = default;

	const float& ScaleVariance() const;

	EntityType Type() const override;
	std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const override;
	void FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const override;
};