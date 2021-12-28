#pragma once

#include "Tile/Object/TileEntity.hpp"

#include "Utils/Color.hpp"
#include "ObjectDatabase/ModelStorage.hpp"

class Part : public TileEntity
{
	friend class Blueprint;

	Color color;
	Model* pModel;
	PartData* pParent;
	int xAxis;
	int zAxis;

public:
	Part() = default;
	Part(const Part&) = delete;
	Part(Part&) = delete;
	~Part() = default;

	Model* GetModel() const;
	Color GetColor() const;

	std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const override;
	void FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const override;
	glm::mat4 GetTransformMatrix() const override;
};