#pragma once

#include <string>
#include <vector>

#include "Utils/ByteImpl.hpp"

namespace String
{
	std::string ToUtf8(const std::wstring& wstr);
	std::wstring ToWide(const std::string& str);

	void ToLowerR(std::wstring& str);
	std::wstring ToLower(const std::wstring& str);

	void ReplaceAllR(std::wstring& str, const wchar_t& to_replace, const wchar_t& replacer);

	std::string BytesToHexString(const std::vector<Byte>& bytes, const int& maxLength, const int& lineLength);
}