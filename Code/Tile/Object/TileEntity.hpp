#pragma once

#include <glm.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/transform.hpp>

#include <string>
#include "Utils/Uuid.hpp"
#include "Utils/Color.hpp"

#include "ObjectDatabase/ObjectData.hpp"
#include "Tile/WriterOffset.hpp"

struct ObjectTexData
{
	TextureList Textures;
	Color TexColor;
};

struct Model;

enum class EntityType
{
	Harvestable,
	Blueprint,
	Prefab,
	Asset,
	Block,
	Joint,
	Part
};

class TileEntity
{
protected:
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::quat rotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f);

	SMUuid uuid;
	Model* pModel;

public:
	virtual ~TileEntity() = default;

	glm::vec3 GetPosition() const;
	glm::quat GetRotation() const;
	glm::vec3 GetSize() const;
	SMUuid GetUuid() const;
	Model* GetModel() const;
	virtual glm::mat4 GetTransformMatrix() const;

	virtual EntityType Type() const = 0;
	virtual std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const = 0;
	virtual void FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const  = 0;
	virtual void WriteObjectToFile(std::ofstream& file, WriterOffsetData& mOffset, const glm::mat4& transform_matrix) const;

	void SetPosition(const glm::vec3& pos);
	void SetRotation(const glm::quat& rot);
	void SetSize(const glm::vec3& size);
	void SetUuid(const SMUuid& uuid);
	void SetModel(Model* model);
};