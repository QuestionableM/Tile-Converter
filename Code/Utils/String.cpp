#include "String.hpp"

#include "Utils/WinInclude.hpp"
#include <cwctype>
#include <iomanip>
#include <sstream>

std::string String::ToUtf8(const std::wstring& wstr)
{
	int count = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string str(count, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], count, NULL, NULL);
	return str;
}

std::wstring String::ToWide(const std::string& str)
{
	int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), NULL, 0);
	std::wstring wstr(count, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstr[0], count);
	return wstr;
}

void String::ToLowerR(std::wstring& str)
{
	for (wchar_t& cChar : str)
		cChar = std::towlower(cChar);
}

std::wstring String::ToLower(const std::wstring& str)
{
	std::wstring str_copy = str;
	String::ToLowerR(str_copy);

	return str_copy;
}

void String::ReplaceAllR(std::wstring& str, const wchar_t& to_replace, const wchar_t& replacer)
{
	std::size_t cIdx = 0;
	while ((cIdx = str.find(to_replace)) != std::wstring::npos)
		str[cIdx] = replacer;
}

std::string String::BytesToHexString(const std::vector<Byte>& bytes, const int& maxLength, const int& lineLength)
{
	std::string output_string;
	int a = 1;

	for (std::size_t i = 0; i < std::min(bytes.size(), (std::size_t)maxLength); i++)
	{
		std::stringstream sstream;
		sstream << std::setfill('0') << std::setw(sizeof(Byte) * 2) << std::hex << (int)bytes[i];

		output_string.append(sstream.str());

		if ((a++) % lineLength == 0) output_string.append("\n\t\t");
	}

	return output_string;
}

std::string String::FloatVecToString(const float* f, const std::size_t& amount, const std::string separator)
{
	std::string out;

	for (std::size_t a = 0; a < amount; a++)
	{
		if (a > 0) out.append(separator);

		out.append(std::to_string(f[a]));
	}

	return out;
}

std::wstring String::ReadRegistryKey(const std::wstring& main_key, const std::wstring& sub_key)
{
	wchar_t _Data[255] = {};
	DWORD _BufSz = 8196;

	LSTATUS _Status = RegGetValueW(
		HKEY_CURRENT_USER,
		main_key.c_str(),
		sub_key.c_str(),
		RRF_RT_REG_SZ,
		NULL,
		(PVOID)&_Data,
		&_BufSz
	);

	if (_Status == ERROR_SUCCESS)
		return std::wstring(_Data);

	return L"";
}