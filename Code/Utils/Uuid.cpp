#include "Uuid.hpp"

#include "Utils\String.hpp"

#include <iomanip>
#include <sstream>

void SMUuid::FromString(const std::string& uuid)
{
	if (uuid.size() != 36) //if the string size doesn't match, then create a null uuid
	{
		std::memset(&m_Data8, 0, 16);
		return;
	}

	std::string lUuidCopy = uuid;

	std::size_t lIdx = 0;
	while ((lIdx = lUuidCopy.find('-')) != std::string::npos)
		lUuidCopy.erase(lUuidCopy.begin() + lIdx);

	for (std::size_t a = 0; a < 16; a++)
		m_Data8[a] = (unsigned char)std::stoi(lUuidCopy.substr(a * 2, 2), nullptr, 16);
}

SMUuid::SMUuid()
{
	std::memset(&m_Data8, 0, 16);
}

SMUuid::SMUuid(const std::string& uuid)
{
	this->FromString(uuid);
}

SMUuid::SMUuid(const std::vector<long long>& longs)
{
	std::memcpy(&m_Data64, longs.data(), 16);
}

SMUuid::SMUuid(const long long& first, const long long& second, const bool& isBigEndian)
{
	m_Data64[0] = first;
	m_Data64[1] = second;

	if (isBigEndian)
	{
		std::reverse(m_Data8, m_Data8 + 16);
	}
}

std::size_t SMUuid::Hash() const
{
	return (std::hash<unsigned long long>{}(m_Data64[0]) >> 1) ^ (std::hash<unsigned long long>{}(m_Data64[1]) << 2);
}

std::string SMUuid::ToString() const
{
	std::stringstream lStream;

	lStream << std::hex << std::setfill('0')
		<< std::setw(2) << (int)m_Data8[0]
		<< std::setw(2) << (int)m_Data8[1]
		<< std::setw(2) << (int)m_Data8[2]
		<< std::setw(2) << (int)m_Data8[3]
		<< '-'
		<< std::setw(2) << (int)m_Data8[4]
		<< std::setw(2) << (int)m_Data8[5]
		<< '-'
		<< std::setw(2) << (int)m_Data8[6]
		<< std::setw(2) << (int)m_Data8[7]
		<< '-'
		<< std::setw(2) << (int)m_Data8[8]
		<< std::setw(2) << (int)m_Data8[9]
		<< '-'
		<< std::setw(2) << (int)m_Data8[10]
		<< std::setw(2) << (int)m_Data8[11]
		<< std::setw(2) << (int)m_Data8[12]
		<< std::setw(2) << (int)m_Data8[13]
		<< std::setw(2) << (int)m_Data8[14]
		<< std::setw(2) << (int)m_Data8[15];

	return lStream.str();
}

std::wstring SMUuid::ToWstring() const
{
	return String::ToWide(this->ToString());
}

bool operator==(const SMUuid& lhs, const SMUuid& rhs) noexcept
{
	return (
		lhs.m_Data64[0] == rhs.m_Data64[0] &&
		lhs.m_Data64[1] == rhs.m_Data64[1]
	);
}

bool operator!=(const SMUuid& lhs, const SMUuid& rhs) noexcept
{
	return (
		lhs.m_Data64[0] != rhs.m_Data64[0] ||
		lhs.m_Data64[1] != rhs.m_Data64[1]
	);
}

bool operator< (const SMUuid& lhs, const SMUuid& rhs) noexcept
{
	return (
		lhs.m_Data64[0] < rhs.m_Data64[0] ||
		lhs.m_Data64[1] < rhs.m_Data64[1]
	);
}