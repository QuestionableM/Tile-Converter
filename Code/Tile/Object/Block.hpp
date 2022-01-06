#pragma once

#include "Tile/Object/TileEntity.hpp"

class Block : public TileEntity
{
	Color color;
	BlockData* pParent;
	glm::vec3 Bounds;
	int xAxis;
	int zAxis;

public:
	Block(BlockData* pParent, const glm::vec3& bounds, const Color& color, const int& xAxis, const int& zAxis);
	Block(const Block&) = delete;
	Block(Block&) = delete;
	~Block() = default;

	EntityType Type() const override;
	std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const override;
	void FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const override;
	void WriteObjectToFile(std::ofstream& file, WriterOffsetData& mOffset, const glm::mat4& transform_matrix) const override;
	glm::mat4 GetTransformMatrix() const override;
};