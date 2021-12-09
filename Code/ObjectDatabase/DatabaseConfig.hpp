#pragma once

#include <string>
#include "Utils/Json.hpp"

class DatabaseConfig
{
public:
	static std::wstring GamePath;
	static std::vector<std::wstring> AssetListFolders;
	static std::vector<std::wstring> ModFolders;

private:
	static void ReadUserSettings(const nlohmann::json& config_json);
	static void ReadProgramSettings(const nlohmann::json& config_json);

public:
	static void ReadConfig(const std::wstring& path);
};