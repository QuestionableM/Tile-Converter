#pragma once

#include <glm.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/transform.hpp>

#include "Utils/Uuid.hpp"
#include "ObjectDatabase/ModelStorage.hpp"

class TileEntity
{
protected:

	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 size;
	SMUuid uuid;

public:

	glm::vec3 GetPosition() const
	{
		return position;
	}

	glm::quat GetRotation() const
	{
		return rotation;
	}

	glm::vec3 GetSize() const
	{
		return size;
	}

	SMUuid GetUuid() const
	{
		return uuid;
	}

	glm::mat4 GetTransformMatrix() const
	{
		glm::mat4 transform(1.0f);

		transform *= glm::translate(this->position);
		transform *= glm::toMat4(this->rotation);
		transform *= glm::scale(this->size);

		return transform;
	}

	virtual std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const = 0;


	void SetPosition(const glm::vec3& pos)
	{
		this->position = pos;
	}

	void SetRotation(const glm::quat& rot)
	{
		this->rotation = rot;
	}

	void SetSize(const glm::vec3& size)
	{
		this->size = size;
	}

	void SetUuid(const SMUuid& uuid)
	{
		this->uuid = uuid;
	}
};