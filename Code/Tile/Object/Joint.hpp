#pragma once

#include "ObjectDatabase\ModelStorage.hpp"
#include "Tile\Object\TileEntity.hpp"
#include "Utils\Color.hpp"

class Joint : public TileEntity
{
	Color color;
	PartData* pParent;
	int xAxis;
	int zAxis;

public:
	inline Joint(PartData* pParent, Model* pModel, const Color& color, const int& xAxis, const int& zAxis)
	{
		this->pParent = pParent;
		this->uuid = pParent->Uuid;
		this->pModel = pModel;
		this->color = color;
		this->xAxis = xAxis;
		this->zAxis = zAxis;
	}

	Joint(const Joint&) = delete;
	Joint(Joint&) = delete;
	~Joint() = default;

	inline Color GetColor() const { return this->color; }

	inline EntityType Type() const override { return EntityType::Joint; }
	std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const override;
	void FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const override;
	glm::mat4 GetTransformMatrix() const override;
};