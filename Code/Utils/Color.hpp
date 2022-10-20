#pragma once

#include "Utils/ByteImpl.hpp"

#include <sstream>
#include <iomanip>
#include <string>

class Color
{
public:
	inline Color() { std::memset(m_bytes, 0, 3); }

	inline Color(const std::string& color) { this->FromString(color); }
	inline Color(const std::wstring& color) { this->FromWstring(color); }

	inline Color(const Byte& r, const Byte& g, const Byte& b)
	{
		m_bytes[0] = r;
		m_bytes[1] = g;
		m_bytes[2] = b;
	}

	inline Color(const unsigned int& color)
	{
		m_bytes[0] = static_cast<Byte>(color);
		m_bytes[1] = static_cast<Byte>(color >> 8);
		m_bytes[2] = static_cast<Byte>(color >> 16);
	}


	inline void operator=(const std::string& color) { this->FromString(color); }
	inline void operator=(const std::wstring& color) { this->FromWstring(color); }

	inline std::string String() const
	{
		return std::to_string(m_bytes[0]) + " " + std::to_string(m_bytes[1]) + " " + std::to_string(m_bytes[2]);
	}

	inline std::string StringNormalized() const
	{
		const float v_norm_r = static_cast<float>(m_bytes[0]) / 255.0f;
		const float v_norm_g = static_cast<float>(m_bytes[1]) / 255.0f;
		const float v_norm_b = static_cast<float>(m_bytes[2]) / 255.0f;

		return std::to_string(v_norm_r) + " " + std::to_string(v_norm_g) + " " + std::to_string(v_norm_b);
	}

	inline std::string StringHex() const
	{
		std::stringstream sstream;

		const int v_full_color = m_bytes[2] | (m_bytes[1] << 8) | (m_bytes[0] << 16);
		sstream << std::hex << std::setfill('0') << std::setw(6) << v_full_color;

		return sstream.str();
	}

private:
	inline void FromString(const std::string& color)
	{
		if (color.size() < 6)
		{
			std::memset(m_bytes, 0, 3);
			return;
		}

		if (color[0] == '#')
		{
			if (color.size() < 7)
			{
				std::memset(m_bytes, 0, 3);
				return;
			}

			m_bytes[0] = static_cast<Byte>(std::stoi(color.substr(1, 2), nullptr, 16));
			m_bytes[1] = static_cast<Byte>(std::stoi(color.substr(3, 2), nullptr, 16));
			m_bytes[2] = static_cast<Byte>(std::stoi(color.substr(5, 2), nullptr, 16));
		}
		else
		{
			m_bytes[0] = static_cast<Byte>(std::stoi(color.substr(0, 2), nullptr, 16));
			m_bytes[1] = static_cast<Byte>(std::stoi(color.substr(2, 2), nullptr, 16));
			m_bytes[2] = static_cast<Byte>(std::stoi(color.substr(4, 2), nullptr, 16));
		}
	}

	inline void FromWstring(const std::wstring& color)
	{
		if (color.size() < 6)
		{
			std::memset(m_bytes, 0, 3);
			return;
		}

		if (color[0] == '#')
		{
			if (color.size() < 7)
			{
				std::memset(m_bytes, 0, 3);
				return;
			}

			m_bytes[0] = static_cast<Byte>(std::stoi(color.substr(1, 2), nullptr, 16));
			m_bytes[1] = static_cast<Byte>(std::stoi(color.substr(3, 2), nullptr, 16));
			m_bytes[2] = static_cast<Byte>(std::stoi(color.substr(5, 2), nullptr, 16));
		}
		else
		{
			m_bytes[0] = static_cast<Byte>(std::stoi(color.substr(0, 2), nullptr, 16));
			m_bytes[1] = static_cast<Byte>(std::stoi(color.substr(2, 2), nullptr, 16));
			m_bytes[2] = static_cast<Byte>(std::stoi(color.substr(4, 2), nullptr, 16));
		}
	}

	Byte m_bytes[3];
};