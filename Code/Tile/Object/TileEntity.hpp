#pragma once

#include <glm.hpp>
#include <gtc/quaternion.hpp>

class TileEntity
{
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 size;

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
};