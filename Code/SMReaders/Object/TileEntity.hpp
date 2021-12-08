#pragma once

#include <array>

class TileEntity
{
	std::array<float, 3> position;
	std::array<float, 4> rotation;
	std::array<float, 3> size;

public:

	std::array<float, 3> GetPosition() const
	{
		return position;
	}

	std::array<float, 4> GetRotation() const
	{
		return rotation;
	}

	std::array<float, 3> GetSize() const
	{
		return size;
	}

	void SetPosition(const std::array<float, 3>& pos)
	{
		this->position = pos;
	}

	void SetRotation(const std::array<float, 4>& rot)
	{
		this->rotation = rot;
	}

	void SetSize(const std::array<float, 3>& size)
	{
		this->size = size;
	}
};