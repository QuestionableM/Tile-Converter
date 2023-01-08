#pragma once

#include "Tile\Object\TileEntity.hpp"

#include "Utils\Color.hpp"
#include "Utils\Uuid.hpp"

#include <unordered_map>

class Asset : public TileEntity
{
	class AssetData* pParent;

	using ColorMap = std::unordered_map<std::wstring, Color>;
	ColorMap m_Colors;

public:
	inline Asset(AssetData* pParent, Model* pModel, const std::unordered_map<std::wstring, Color>& color_map)
	{
		this->pParent = pParent;
		this->uuid = pParent->Uuid;
		this->pModel = pModel;
		this->m_Colors = color_map;
	}

	Asset(const Asset&) = delete;
	Asset(Asset&&) = delete;
	~Asset() = default;

	Color GetColor(const std::wstring& color) const;
	inline std::unordered_map<std::wstring, Color> GetMaterials() const { return this->m_Colors; }

	inline EntityType Type() const override { return EntityType::Asset; }
	std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const override;
	void FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const override;
};