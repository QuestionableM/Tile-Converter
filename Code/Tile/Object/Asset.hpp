#pragma once

#include "Utils/Uuid.hpp"
#include "Utils/Color.hpp"
#include "Tile/Object/TileEntity.hpp"

#include <unordered_map>

class Asset : public TileEntity
{
	SMUuid uuid;
	std::unordered_map<std::string, Color> mColors;
				/*material name,   color*/

public:
	Asset() = default;
	Asset(const Asset&) = delete;
	Asset(Asset&&) = delete;

	SMUuid GetUuid() const
	{
		return uuid;
	}

	void SetUuid(const SMUuid& uuid)
	{
		this->uuid = uuid;
	}

	void AddMaterial(const std::string& mat_name, const Color& color)
	{
		if (mColors.find(mat_name) != mColors.end())
			return;

		mColors.insert(std::make_pair(mat_name, color));
	}

	std::unordered_map<std::string, Color> GetMaterials()
	{
		return this->mColors;
	}
};