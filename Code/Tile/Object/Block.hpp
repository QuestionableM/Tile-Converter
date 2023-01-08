#pragma once

#include "Tile\Object\TileEntity.hpp"

class Block : public TileEntity
{
	Color color;
	BlockData* pParent;
	glm::vec3 Bounds;
	int xAxis;
	int zAxis;

public:
	inline Block(BlockData* pParent, const glm::vec3& bounds, const Color& color, const int& xAxis, const int& zAxis)
	{
		this->pParent = pParent;
		this->uuid = pParent->Uuid;
		this->Bounds = bounds;
		this->color = color;
		this->xAxis = xAxis;
		this->zAxis = zAxis;
	}

	Block(const Block&) = delete;
	Block(Block&) = delete;
	~Block() = default;

	inline EntityType Type() const override { return EntityType::Block; }
	std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const override;
	void FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const override;
	void WriteObjectToFile(std::ofstream& file, WriterOffsetData& mOffset, const glm::mat4& transform_matrix) const override;
	glm::mat4 GetTransformMatrix() const override;
};