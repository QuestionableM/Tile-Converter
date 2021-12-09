#pragma once

#include <string>
#include <unordered_map>
#include "SMReaders/Data/Json.hpp"

class DatabaseConfig
{
	static std::wstring SMPath;
	static std::unordered_map<std::wstring, std::wstring> KeyReplacements;
	static std::vector<std::wstring> AssetListFiles;

	void ReadConfig(const std::wstring& path)
	{
		nlohmann::json cfgData = JsonReader::LoadParseJson(path);
		if (!cfgData.is_object()) return;

		SMPath.clear();
		KeyReplacements.clear();
		AssetListFiles.clear();
	}
};

std::wstring DatabaseConfig::SMPath = L"";
std::unordered_map<std::wstring, std::wstring> DatabaseConfig::KeyReplacements = {};
std::vector<std::wstring> DatabaseConfig::AssetListFiles = {};