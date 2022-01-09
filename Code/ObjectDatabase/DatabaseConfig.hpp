#pragma once

#include <string>
#include "Utils/Json.hpp"

class DatabaseConfig
{
public:
	constexpr static const std::wstring_view ConfigPath = L"./Resources/Config.json";
	constexpr static const std::wstring_view RotationsPath = L"./Resources/RotationSettings.json";

	static std::wstring GamePath;
	static std::vector<std::wstring> AssetListFolders;
	static std::vector<std::wstring> ModFolders;

private:
	static bool GetSteamPaths(std::wstring& game_path, std::wstring& workshop_path);
	static void ReadUserSettings(nlohmann::json& config_json, bool& should_write);
	static void ReadProgramSettings(const nlohmann::json& config_json);

	static nlohmann::json GetConfigJson(bool* should_write = nullptr);

public:
	static void SaveConfig();
	static void ReadConfig();
};