#pragma once

#include "Tile/Object/TileEntity.hpp"

class TileClutter : public TileEntity
{
	friend class ClutterReader;
	friend class Tile;

	class ClutterData* pParent;
public:
	TileClutter() = default;
	TileClutter(TileClutter&) = delete;
	TileClutter(const TileClutter&) = delete;
	~TileClutter() = default;

	EntityType Type() const override;
	std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const override;
	void FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const override;
};