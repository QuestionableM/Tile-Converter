#pragma once

#include "Utils/WinInclude.hpp"

#include <vector>
#include <string>

class SMUuid
{
	union
	{
		unsigned __int8  m_Data8[16];
		unsigned __int32 m_Data32[4];
		unsigned __int64 m_Data64[2];
	};

	friend bool operator==(const SMUuid& lhs, const SMUuid& rhs) noexcept;
	friend bool operator!=(const SMUuid& lhs, const SMUuid& rhs) noexcept;
	friend bool operator< (const SMUuid& lhs, const SMUuid& rhs) noexcept;

	void FromString(const std::string& uuid);

public:
	SMUuid();
	SMUuid(const std::string& uuid);
	SMUuid(const std::vector<long long>& longs);
	SMUuid(const long long& first, const long long& second, const bool& isBigEndian = false);

	std::size_t  Hash()      const;
	std::string  ToString()  const;
	std::wstring ToWstring() const;
};

bool operator==(const SMUuid& lhs, const SMUuid& rhs) noexcept;
bool operator!=(const SMUuid& lhs, const SMUuid& rhs) noexcept;
bool operator< (const SMUuid& lhs, const SMUuid& rhs) noexcept;

namespace std
{
	template<>
	struct hash<SMUuid>
	{
		using argument_type = SMUuid;
		using result_type = std::size_t;

		result_type operator()(argument_type const& uuid) const
		{
			return uuid.Hash();
		}
	};
}