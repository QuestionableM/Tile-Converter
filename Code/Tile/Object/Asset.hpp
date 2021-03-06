#pragma once

#include "Tile/Object/TileEntity.hpp"

#include "Utils/Uuid.hpp"
#include "Utils/Color.hpp"

#include <unordered_map>

class Asset : public TileEntity
{
	class AssetData* pParent;
	std::unordered_map<std::wstring /*material*/, Color> mColors;

public:
	Asset(AssetData* pParent, Model* pModel, const std::unordered_map<std::wstring, Color>& color_map);
	Asset(const Asset&) = delete;
	Asset(Asset&&) = delete;
	~Asset() = default;

	Color GetColor(const std::wstring& color) const;
	std::unordered_map<std::wstring, Color> GetMaterials() const;

	EntityType Type() const override;
	std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const override;
	void FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const override;
};