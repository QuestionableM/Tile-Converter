#pragma once

#include "Tile/Object/TileEntity.hpp"

#include "Utils/Uuid.hpp"
#include "Utils/Color.hpp"

#include <unordered_map>

class Asset : public TileEntity
{
	friend class TilePart;
	friend class AssetListReader;
	friend class Tile;
	friend class PrefabFileReader;

	struct Model* pModel;
	class AssetData* pParent;
	std::unordered_map<std::wstring /*material*/, Color> mColors;

public:
	Asset() = default;
	Asset(const Asset&) = delete;
	Asset(Asset&&) = delete;
	~Asset() = default;

	Model* GetModel() const;
	Color GetColor(const std::wstring& color) const;
	std::unordered_map<std::wstring, Color> GetMaterials() const;

	std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const override;
	void FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const override;

	void AddMaterial(const std::wstring& mat_name, const Color& color);
};