#pragma once

#include "Utils/WinInclude.hpp"
#include <uuid/uuid.h>
#include <string>

class SMUuid
{
	uuids::uuid uuid_data;

	friend bool operator==(const SMUuid& lhs, const SMUuid& rhs) noexcept;
public:
	SMUuid() = default;
	SMUuid(const std::vector<long long>& longs);
	SMUuid(const std::string& str_uuid);

	void operator=(const std::vector<long long>& longs);
	void operator=(const std::string& str_uuid);

	std::string ToString() const;
	std::wstring ToWstring() const;
};

bool operator==(const SMUuid& lhs, const SMUuid& rhs) noexcept;
bool operator!=(const SMUuid& lhs, const SMUuid& rhs) noexcept;

namespace std
{
	template<>
	struct hash<SMUuid>
	{
		using argument_type = SMUuid;
		using result_type = std::size_t;

		result_type operator()(argument_type const& uuid) const
		{
			std::hash<std::string> hasher;
			return static_cast<result_type>(hasher(uuid.ToString()));
		}
	};
}