#pragma once

#include "SMReaders/Data/Uuid.hpp"
#include "Tile/Object/TileEntity.hpp"

#include <unordered_map>

class Asset : public TileEntity
{
	SMUuid uuid;
	std::unordered_map<std::string, int> mColors;
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

	void AddMaterial(const std::string& mat_name, const int& color)
	{
		if (mColors.find(mat_name) != mColors.end())
			return;

		mColors.insert(std::make_pair(mat_name, color));
	}

	std::unordered_map<std::string, int> GetMaterials()
	{
		return this->mColors;
	}
};