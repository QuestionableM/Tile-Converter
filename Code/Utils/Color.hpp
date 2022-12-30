#pragma once

#include "Utils/ByteImpl.hpp"

#include <sstream>
#include <iomanip>
#include <string>

class Color
{
public:
	inline Color() { m_color = 0x000000ff; }

	inline Color(const std::string& color)  { this->FromString<std::string>(color);  }
	inline Color(const std::wstring& color) { this->FromString<std::wstring>(color); }

	inline Color(const Byte& r, const Byte& g, const Byte& b)
	{
		m_bytes[0] = r;
		m_bytes[1] = g;
		m_bytes[2] = b;
		m_bytes[3] = 255;
	}

	inline Color(const unsigned int& color)
	{
		m_bytes[0] = static_cast<Byte>(color);
		m_bytes[1] = static_cast<Byte>(color >> 8);
		m_bytes[2] = static_cast<Byte>(color >> 16);
		m_bytes[3] = static_cast<Byte>(color >> 24);
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
		char v_buffer[7];
		sprintf_s(v_buffer, "%02x%02x%02x", m_bytes[2], m_bytes[1], m_bytes[0]);

		return std::string(v_buffer);
	}

	//Convert to string with alpha component
	inline std::string StringHexAlpha() const
	{
		char v_buffer[9];
		sprintf(v_buffer, "%02x%02x%02x%02x", m_bytes[3], m_bytes[2], m_bytes[1], m_bytes[0]);

		return std::string(v_buffer);
	}

private:
	template<typename T>
	inline void FromString(const T& color)
	{
		static_assert(std::is_same_v<T, std::string> || std::is_same_v<T, std::wstring>, "Color::FromString can only be used with std::string, std::wstring");

		if (color.empty())
		{
			m_color = 0x000000ff;
			return;
		}

		if (color[0] == '#')
		{
			if (color.size() < 7)
			{
				m_color = 0x000000ff;
				return;
			}

			m_bytes[0] = static_cast<Byte>(std::stoi(color.substr(1, 2), nullptr, 16));
			m_bytes[1] = static_cast<Byte>(std::stoi(color.substr(3, 2), nullptr, 16));
			m_bytes[2] = static_cast<Byte>(std::stoi(color.substr(5, 2), nullptr, 16));

			if (color.size() >= 9)
				m_bytes[3] = static_cast<Byte>(std::stoi(color.substr(7, 2), nullptr, 16));
			else
				m_bytes[3] = 255;
		}
		else
		{
			if (color.size() < 6)
			{
				m_color = 0x000000ff;
				return;
			}

			m_bytes[0] = static_cast<Byte>(std::stoi(color.substr(0, 2), nullptr, 16));
			m_bytes[1] = static_cast<Byte>(std::stoi(color.substr(2, 2), nullptr, 16));
			m_bytes[2] = static_cast<Byte>(std::stoi(color.substr(4, 2), nullptr, 16));

			if (color.size() >= 8)
				m_bytes[3] = static_cast<Byte>(std::stoi(color.substr(6, 2), nullptr, 16));
			else
				m_bytes[3] = 255;
		}
	}

	union
	{
		Byte m_bytes[4];
		unsigned int m_color;
	};
};