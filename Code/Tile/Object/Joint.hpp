#pragma once

#include "Tile/Object/TileEntity.hpp"

#include "Utils/Color.hpp"
#include "ObjectDatabase/ModelStorage.hpp"

class Joint : public TileEntity
{
	friend class Blueprint;

	Color color;
	PartData* pParent;
	int xAxis;
	int zAxis;

public:
	Joint() = default;
	Joint(const Joint&) = delete;
	Joint(Joint&) = delete;
	~Joint() = default;

	Color GetColor() const;

	EntityType Type() const override;
	std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const override;
	void FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const override;
	glm::mat4 GetTransformMatrix() const override;
};