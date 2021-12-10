#pragma once

#include <string>
#include "Utils/ByteImpl.hpp"

class Color
{
	Byte bytes[3];

	void SetColorInternal(const std::string& color);
public:
	Color() = default;
	Color(const std::string& color);
	Color(const Byte& r, const Byte& g, const Byte& b);
	Color(const unsigned int& color);

	void operator=(const std::string& color);

	std::string String() const;
	std::string StringNormalized() const;
	std::string StringHex() const;
};