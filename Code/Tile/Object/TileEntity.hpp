#pragma once

#include "ObjectDatabase\ObjectData.hpp"
#include "Tile\WriterOffset.hpp"

#include "Utils\Color.hpp"
#include "Utils\Uuid.hpp"

#include <string>

#include <gtx\quaternion.hpp>
#include <gtx\transform.hpp>
#include <glm.hpp>

struct ObjectTexData
{
	TextureList Textures;
	Color TexColor;
};

struct Model;

enum class EntityType : unsigned short
{
	Harvestable = (1 << 0),
	Blueprint   = (1 << 1),
	Clutter     = (1 << 2),
	Prefab      = (1 << 3),
	Asset       = (1 << 4),
	Block       = (1 << 5),
	Joint       = (1 << 6),
	Part        = (1 << 7),
	Decal       = (1 << 8),

	//Is not used anywhere except the GroundTerrainData class
	GroundTerrain = 0
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

	inline glm::vec3 GetPosition() const { return this->position; }
	inline glm::quat GetRotation() const { return this->rotation; }
	inline glm::vec3 GetSize() const { return this->size; }
	inline SMUuid GetUuid() const { return this->uuid; }
	inline Model* GetModel() const { return this->pModel; }
	inline virtual glm::mat4 GetTransformMatrix() const
	{
		glm::mat4 transform(1.0f);

		transform *= glm::translate(this->position);
		transform *= glm::toMat4(this->rotation);
		transform *= glm::scale(this->size);

		return transform;
	}

	virtual EntityType Type() const = 0;
	virtual std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const = 0;
	virtual void FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const  = 0;
	virtual void WriteObjectToFile(std::ofstream& file, WriterOffsetData& mOffset, const glm::mat4& transform_matrix) const;
	inline virtual std::size_t GetAmountOfObjects() const { return 1; }

	inline void SetPosition(const glm::vec3& pos) { this->position = pos; }
	inline void SetRotation(const glm::quat& rot) { this->rotation = rot; }
	inline void SetSize(const glm::vec3& size) { this->size = size; }
	inline void SetUuid(const SMUuid& uuid) { this->uuid = uuid; }
	inline void SetModel(Model* model) { this->pModel = model; }
};