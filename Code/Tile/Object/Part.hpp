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
	inline Part(PartData* pParent, Model* pModel, const Color& color, const int& xAxis, const int& zAxis)
	{
		this->pParent = pParent;
		this->uuid = pParent->Uuid;
		this->pModel = pModel;
		this->color = color;
		this->xAxis = xAxis;
		this->zAxis = zAxis;
	}

	Part(const Part&) = delete;
	Part(Part&) = delete;
	~Part() = default;

	inline Color GetColor() const { return this->color; }

	inline EntityType Type() const override { return EntityType::Part; }
	std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const override;
	void FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const override;
	glm::mat4 GetTransformMatrix() const override;
};