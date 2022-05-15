#pragma once

#include <string>
#include "Utils/Json.hpp"

class DatabaseConfig
{
public:
	constexpr static const std::wstring_view ConfigPath = L"./Resources/Config.json";
	constexpr static const std::wstring_view RotationsPath = L"./Resources/RotationSettings.json";
	constexpr static const std::wstring_view MaterialMapPath = L"./Resources/MaterialIds.json";

	inline static std::wstring GamePath                      = L"";
	inline static std::vector<std::wstring> AssetListFolders = {};
	inline static std::vector<std::wstring> ModFolders       = {};

private:
	static void JsonStrArrayToVector(const nlohmann::json& pJson, const std::string& pKey, std::vector<std::wstring>& pWstrVec);
	static void AddToStrVec(std::vector<std::wstring>& mWstrVec, const std::wstring& mWstr);

	static void ReadProgramSettings(const nlohmann::json& config_json);

	static bool GetSteamPaths(std::wstring& game_path, std::wstring& workshop_path);
	static void FindLocalUsers();
	static void FindGamePath(const nlohmann::json& config_json, bool& should_write);
	static void ReadUserSettings(const nlohmann::json& config_json, bool& should_write);

	static nlohmann::json GetConfigJson(bool* should_write = nullptr);
	static void UpdatePathReplacement();

public:
	static void SaveConfig();
	static void ReadConfig();
};