#pragma once

#include <uuid/uuid.h>
#include <string>

class SMUuid
{
	uuids::uuid uuid_data;

public:
	SMUuid() = default;

	SMUuid(const std::vector<long long>& longs)
	{
		std::vector<std::uint8_t> uuid_bytes(8 * 2);

		std::memcpy(uuid_bytes.data(), longs.data(), 8 * 2);

		this->uuid_data = uuids::uuid(uuid_bytes);
	}

	SMUuid(const std::string& str_uuid)
	{
		this->uuid_data = uuids::uuid::from_string(str_uuid);
	}

	void operator()(const std::vector<long long>& longs)
	{
		std::vector<std::uint8_t> uuid_bytes(8 * 2);

		std::memcpy(uuid_bytes.data(), longs.data(), 8 * 2);

		this->uuid_data = uuids::uuid(uuid_bytes);
	}

	void operator()(const std::string& str_uuid)
	{
		this->uuid_data = uuids::uuid::from_string(str_uuid);
	}

	std::string ToString() const
	{
		return uuids::to_string(this->uuid_data);
	}

	std::wstring ToWstring() const
	{
		return uuids::to_wstring(this->uuid_data);
	}
};