#pragma once

#include <vector>
#include <string>
#include "Utils/ByteImpl.hpp"

namespace File
{
	std::vector<Byte> ReadFileBytes(const std::wstring& path);
	bool Exists(const std::wstring& path);
}