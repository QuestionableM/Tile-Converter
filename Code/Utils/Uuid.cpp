#include "Uuid.hpp"

SMUuid::SMUuid(const std::vector<long long>& longs)
{
	std::vector<std::uint8_t> uuid_bytes(8 * 2);

	std::memcpy(uuid_bytes.data(), longs.data(), 8 * 2);

	this->uuid_data = uuids::uuid(uuid_bytes);
}

SMUuid::SMUuid(const std::string& str_uuid)
{
	this->uuid_data = uuids::uuid::from_string(str_uuid);
}

void SMUuid::operator=(const std::vector<long long>& longs)
{
	std::vector<std::uint8_t> uuid_bytes(8 * 2);

	std::memcpy(uuid_bytes.data(), longs.data(), 8 * 2);

	this->uuid_data = uuids::uuid(uuid_bytes);
}

void SMUuid::operator=(const std::string& str_uuid)
{
	this->uuid_data = uuids::uuid::from_string(str_uuid);
}

std::string SMUuid::ToString() const
{
	return uuids::to_string(this->uuid_data);
}

std::wstring SMUuid::ToWstring() const
{
	return uuids::to_wstring(this->uuid_data);
}

bool operator==(const SMUuid& lhs, const SMUuid& rhs) noexcept
{
	return (lhs.uuid_data == rhs.uuid_data);
}

bool operator!=(const SMUuid& lhs, const SMUuid& rhs) noexcept
{
	return !(lhs == rhs);
}