#pragma once

#include <glm.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/transform.hpp>

#include <string>
#include "Utils/Uuid.hpp"

#include "ObjectDatabase/ObjectData.hpp"
#include "Utils/Color.hpp"

struct ObjectTexData
{
	TextureList Textures;
	Color TexColor;
};

class TileEntity
{
protected:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 size;
	SMUuid uuid;

public:
	glm::vec3 GetPosition() const;
	glm::quat GetRotation() const;
	glm::vec3 GetSize() const;
	SMUuid GetUuid() const;
	glm::mat4 GetTransformMatrix() const;

	virtual std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const = 0;
	virtual void FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const = 0;

	void SetPosition(const glm::vec3& pos);
	void SetRotation(const glm::quat& rot);
	void SetSize(const glm::vec3& size);
	void SetUuid(const SMUuid& uuid);
};