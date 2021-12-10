#include "Color.hpp"

#include "Utils/String.hpp"
#include "Console.hpp"

#include <sstream>
#include <iomanip>

void Color::SetColorInternal(const std::string& color)
{
	bytes[0] = (Byte)std::stoi(color.substr(0, 2), nullptr, 16);
	bytes[1] = (Byte)std::stoi(color.substr(2, 2), nullptr, 16);
	bytes[2] = (Byte)std::stoi(color.substr(4, 2), nullptr, 16);
}

Color::Color(const std::string& color)
{
	this->SetColorInternal(color);
}

Color::Color(const Byte& r, const Byte& g, const Byte& b)
{
	bytes[0] = r;
	bytes[1] = g;
	bytes[2] = b;
}

Color::Color(const unsigned int& color)
{
	bytes[0] = (color & 0xff);
	bytes[1] = ((color >> 8) & 0xff);
	bytes[2] = ((color >> 16) & 0xff);
}

void Color::operator=(const std::string& color)
{
	this->SetColorInternal(color);
}

std::string Color::String() const
{
	std::string output = std::to_string(bytes[0]) + " " + std::to_string(bytes[1]) + " " + std::to_string(bytes[2]);
	
	return output;
}

std::string Color::StringHex() const
{
	std::stringstream sstream;

	int full_str = bytes[2] | bytes[1] << 8 | bytes[0] << 16;
	sstream << std::hex << std::setfill('0') << std::setw(6) << full_str;

	return sstream.str();
}

std::string Color::StringNormalized() const
{
	const float norm_r = (float)bytes[0] / 255.0f;
	const float norm_g = (float)bytes[1] / 255.0f;
	const float norm_b = (float)bytes[2] / 255.0f;

	std::string output = std::to_string(norm_r) + " " + std::to_string(norm_g) + " " + std::to_string(norm_b);
	return output;
}