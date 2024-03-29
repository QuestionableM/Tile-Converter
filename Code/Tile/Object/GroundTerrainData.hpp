#pragma once

#include "TileEntity.hpp"

class GroundTerrainData : public TileEntity
{
public:
	inline EntityType Type() const override { return EntityType::GroundTerrain; }

	inline std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const { return "TileGroundTerrain"; }
	inline void FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const {}
};