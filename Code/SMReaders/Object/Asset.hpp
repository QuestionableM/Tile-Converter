#pragma once

#include "SMReaders/Data/Uuid.hpp"
#include "SMReaders/Object/TileEntity.hpp"

class Asset : public TileEntity
{
	SMUuid uuid;

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
};