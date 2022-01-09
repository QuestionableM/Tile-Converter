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
	std::string FloatVecToString(const float* f, const std::size_t& amount, const std::string separator = " ");

	std::wstring ReadRegistryKey(const std::wstring& main_key, const std::wstring& sub_key);
}