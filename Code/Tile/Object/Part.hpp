#pragma once

#include "Tile/Object/TileEntity.hpp"

#include "Utils/Color.hpp"
#include "ObjectDatabase/ModelStorage.hpp"

class Part : public TileEntity
{
	Color color;
	PartData* pParent;
	int xAxis;
	int zAxis;

public:
	Part(PartData* pParent, Model* pModel, const Color& color, const int& xAxis, const int& zAxis);
	Part(const Part&) = delete;
	Part(Part&) = delete;
	~Part() = default;

	Color GetColor() const;

	EntityType Type() const override;
	std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const override;
	void FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const override;
	glm::mat4 GetTransformMatrix() const override;
};