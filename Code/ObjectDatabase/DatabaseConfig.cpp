#include "DatabaseConfig.hpp"
#include "ObjectDatabase/KeywordReplacer.hpp"
#include "Utils/String.hpp"
#include "Utils/File.hpp"

#include "Console.hpp"

#include <filesystem>
namespace fs = std::filesystem;

void DatabaseConfig::WstrArrayToJson(nlohmann::json& j_obj, const std::string& key, const std::vector<std::wstring>& r_wstr_vec)
{
	nlohmann::json v_strArray = nlohmann::json::array();

	for (const std::wstring& v_curWstr : r_wstr_vec)
		v_strArray.push_back(String::ToUtf8(v_curWstr));

	j_obj[key] = v_strArray;
}

void DatabaseConfig::JsonStrArrayToVector(const nlohmann::json& pJson, const std::string& pKey, std::vector<std::wstring>& pWstrVec, const bool& replace_keys)
{
	const auto& v_jArray = JsonReader::Get(pJson, pKey);
	if (!v_jArray.is_array()) return;

	for (const auto& v_curVal : v_jArray)
	{
		if (!v_curVal.is_string()) continue;

		const std::wstring v_wstrPath = String::ToWide(v_curVal.get<std::string>());
		if (replace_keys)
		{
			pWstrVec.push_back(KeywordReplacer::ReplaceKey(v_wstrPath));
		}
		else
		{
			pWstrVec.push_back(v_wstrPath);
		}
	}
}

void DatabaseConfig::AddToStrVec(std::vector<std::wstring>& mWstrVec, const std::wstring& mWstr)
{
	for (const std::wstring& wstr_data : mWstrVec)
	{
		if (wstr_data == mWstr || File::Equivalent(wstr_data, mWstr))
			return;
	}

	{
		std::wstring mFinalString = mWstr;

		String::ToLowerR(mFinalString);
		String::ReplaceAllR(mFinalString, L'\\', L'/');

		mWstrVec.push_back(mFinalString);
	}
}

void DatabaseConfig::ReadProgramSettings(const nlohmann::json& config_json)
{
	const auto& program_settings = JsonReader::Get(config_json, "ProgramSettings");
	if (!program_settings.is_object()) return;

	const auto& pKeywords = JsonReader::Get(program_settings, "Keywords");
	if (pKeywords.is_object())
	{
		for (const auto& key_obj : pKeywords.items())
		{
			if (!key_obj.value().is_string()) continue;

			const std::wstring pKey = String::ToWide(key_obj.key());

			std::wstring pValue = String::ToWide(key_obj.value().get<std::string>());
			KeywordReplacer::ReplaceKeyR(pValue);

			KeywordReplacer::SetReplacement(pKey, pValue);
		}
	}

	{
		std::vector<std::wstring> l_upgrade_array = {};
		DatabaseConfig::JsonStrArrayToVector(program_settings, "ResourceUpgradeFiles", l_upgrade_array, true);

		for (const std::wstring& l_upgrade_path : l_upgrade_array)
			KeywordReplacer::LoadResourceUpgrades(l_upgrade_path);
	}

	DatabaseConfig::JsonStrArrayToVector(program_settings, "ScrapAssetDatabase", DatabaseConfig::AssetListFolders, true);
}

bool DatabaseConfig::GetSteamPaths(std::wstring& game_path, std::wstring& workshop_path)
{
	std::wstring steam_path = String::ReadRegistryKey(L"SOFTWARE\\Valve\\Steam", L"SteamPath");
	if (steam_path.empty() || !File::Exists(steam_path))
		steam_path = String::ReadRegistryKey(L"SOFTWARE\\WOW6432Node\\Valve\\Steam", L"SteamPath");

	if (steam_path.empty() || !File::Exists(steam_path)) return false;

	const std::wstring sm_path = steam_path + L"/steamapps/common/scrap mechanic";
	const std::wstring ws_path = steam_path + L"/steamapps/workshop/content/387990";

	if (File::Exists(sm_path))
		game_path = sm_path;

	if (File::Exists(ws_path))
		workshop_path = ws_path;

	return true;
}

void DatabaseConfig::FindLocalUsers()
{
	std::wstring v_smLocalData;
	if (!File::GetAppDataPath(v_smLocalData))
		return;

	v_smLocalData.append(L"\\Axolot Games\\Scrap Mechanic\\User");
	if (!File::Exists(v_smLocalData))
		return;

	std::error_code v_errorCode;
	fs::directory_iterator v_dirIterator(v_smLocalData, fs::directory_options::skip_permission_denied, v_errorCode);

	for (const auto& v_curDir : v_dirIterator)
	{
		if (v_errorCode)
		{
			DebugErrorL("Couldn't read an item in the directory: ", v_smLocalData);
			continue;
		}

		if (!v_curDir.is_directory()) continue;

		std::wstring l_mod_path = v_curDir.path().wstring() + L"\\Mods";

		if (File::Exists(l_mod_path))
		{
			DebugOutL("Found a new path to local mods: ", 0b01101_fg, l_mod_path);
			DatabaseConfig::AddToStrVec(DatabaseConfig::LocalModFolders, l_mod_path);
		}
	}
}

void DatabaseConfig::FindGamePath(const nlohmann::json& config_json, bool& should_write)
{
	if (DatabaseConfig::GamePath.empty() || !File::Exists(DatabaseConfig::GamePath))
	{
		std::wstring game_path, ws_path;
		if (DatabaseConfig::GetSteamPaths(game_path, ws_path))
		{
			DatabaseConfig::GamePath = game_path;
			DebugOutL("Found a game path from the registry: ", 0b1101_fg, DatabaseConfig::GamePath);
			DebugOutL("Found a workshop path from the registry: ", 0b1101_fg, ws_path);

			DatabaseConfig::AddToStrVec(DatabaseConfig::ModFolders, ws_path);

			should_write = true;
		}

		DatabaseConfig::FindLocalUsers();
	}
}

void DatabaseConfig::ReadUserSettings(const nlohmann::json& config_json, bool& should_write)
{
	const auto& user_settings = JsonReader::Get(config_json, "UserSettings");
	if (user_settings.is_object())
	{
		const auto& game_path = JsonReader::Get(user_settings, "GamePath");
		if (game_path.is_string())
		{
			DatabaseConfig::GamePath = String::ToWide(game_path.get<std::string>());
			DebugOutL("Game Path: ", DatabaseConfig::GamePath);
		}

		DatabaseConfig::JsonStrArrayToVector(user_settings, "LocalModFolders", DatabaseConfig::LocalModFolders, false);
		DatabaseConfig::JsonStrArrayToVector(user_settings, "WorkshopModFolders", DatabaseConfig::ModFolders, false);
	}

	DatabaseConfig::FindGamePath(config_json, should_write);
}

nlohmann::json DatabaseConfig::GetConfigJson(bool* should_write)
{
	nlohmann::json cfgData = JsonReader::LoadParseJson(DatabaseConfig::ConfigPath.data());
	if (!cfgData.is_object())
	{
		cfgData = nlohmann::json::object();
	}

	nlohmann::json v_programSettings = JsonReader::Get(cfgData, "ProgramSettings");
	if (!v_programSettings.is_object())
		v_programSettings = nlohmann::json::object();

	if (!v_programSettings.contains("Keywords"))
	{
		v_programSettings["Keywords"] =
		{
			{ "$CHALLENGE_DATA", "$GAME_FOLDER/ChallengeData" },
			{ "$GAME_DATA"     , "$GAME_FOLDER/Data"          },
			{ "$SURVIVAL_DATA" , "$GAME_FOLDER/Survival"      }
		};

		if (should_write)
			(*should_write) = true;
	}

	if (!v_programSettings.contains("ResourceUpgradeFiles"))
	{
		v_programSettings["ResourceUpgradeFiles"] =
		{
			"$GAME_DATA/upgrade_resources.json"
		};

		if (should_write)
			(*should_write) = true;
	}

	if (!v_programSettings.contains("ScrapAssetDatabase"))
	{
		v_programSettings["ScrapAssetDatabase"] =
		{
			"$CHALLENGE_DATA/Terrain/Database/AssetSets",
			"$SURVIVAL_DATA/Terrain/Database/AssetSets",
			"$GAME_DATA/Terrain/Database/AssetSets",

			"$SURVIVAL_DATA/Harvestables/Database/HarvestableSets",

			"$CHALLENGE_DATA/Objects/Database/ShapeSets",
			"$GAME_DATA/Objects/Database/ShapeSets",
			"$SURVIVAL_DATA/Objects/Database/ShapeSets",

			"$GAME_DATA/Terrain/Database/clutter.json",
			"$SURVIVAL_DATA/Terrain/Database/clutter.json"
		};

		if (should_write)
			(*should_write) = true;
	}

	return cfgData;
}

void DatabaseConfig::UpdatePathReplacement()
{
	DebugOutL("Set $GAME_FOLDER to ", DatabaseConfig::GamePath);
	KeywordReplacer::SetReplacement(L"$GAME_FOLDER", DatabaseConfig::GamePath);
}

void DatabaseConfig::SaveConfig()
{
	nlohmann::json cfgData = DatabaseConfig::GetConfigJson();

	{
		nlohmann::json user_settings = nlohmann::json::object();

		user_settings["GamePath"] = String::ToUtf8(DatabaseConfig::GamePath);

		DatabaseConfig::WstrArrayToJson(user_settings, "WorkshopModFolders", DatabaseConfig::ModFolders);
		DatabaseConfig::WstrArrayToJson(user_settings, "LocalModFolders", DatabaseConfig::LocalModFolders);

		cfgData["UserSettings"] = user_settings;
	}

	DebugOutL(0b0110_fg, "Saving a new config...");
	JsonReader::WriteJson(DatabaseConfig::ConfigPath.data(), cfgData);

	//Update the game path keyword in case the path was updated
	DatabaseConfig::UpdatePathReplacement();
}

void DatabaseConfig::ReadConfig()
{
	DebugOutL(0b0110_fg, "Reading the program config...");

	DatabaseConfig::GamePath.clear();
	DatabaseConfig::AssetListFolders.clear();
	DatabaseConfig::ModFolders.clear();
	KeywordReplacer::Clear();

	bool should_write = false;
	nlohmann::json cfgData = DatabaseConfig::GetConfigJson(&should_write);

	DatabaseConfig::ReadUserSettings(cfgData, should_write);
	
	//Stop reading the config if the path to the game is invalid
	if (DatabaseConfig::GamePath.empty()) return;

	DatabaseConfig::UpdatePathReplacement();
	DatabaseConfig::ReadProgramSettings(cfgData);

	if (should_write)
	{
		DatabaseConfig::SaveConfig();
	}
}