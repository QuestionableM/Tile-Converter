#include "DatabaseConfig.hpp"
#include "ObjectDatabase/KeywordReplacer.hpp"
#include "Utils/String.hpp"
#include "Utils/File.hpp"

#include "Console.hpp"

#include <filesystem>
namespace fs = std::filesystem;

void DatabaseConfig::JsonStrArrayToVector(const nlohmann::json& pJson, const std::string& pKey, std::vector<std::wstring>& pWstrVec)
{
	const auto& json_str_array = JsonReader::Get(pJson, pKey);
	if (!json_str_array.is_array()) return;

	for (const auto& pValue : json_str_array)
	{
		if (!pValue.is_string()) continue;

		const std::wstring l_wide_path = String::ToWide(pValue.get<std::string>());
		const std::wstring l_final_path = KeywordReplacer::ReplaceKey(l_wide_path);

		pWstrVec.push_back(l_final_path);
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
		DatabaseConfig::JsonStrArrayToVector(program_settings, "ResourceUpgradeFiles", l_upgrade_array);

		for (const std::wstring& l_upgrade_path : l_upgrade_array)
			KeywordReplacer::LoadResourceUpgrades(l_upgrade_path);
	}

	DatabaseConfig::JsonStrArrayToVector(program_settings, "ScrapAssetDatabase", DatabaseConfig::AssetListFolders);
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
	std::wstring lSmLocalData;
	if (!File::GetAppDataPath(lSmLocalData)) return;

	lSmLocalData.append(L"\\Axolot Games\\Scrap Mechanic\\User");
	if (!File::Exists(lSmLocalData)) return;

	std::error_code rError;
	fs::directory_iterator rDirIterator(lSmLocalData, fs::directory_options::skip_permission_denied, rError);

	for (const auto& cur_dir : rDirIterator)
	{
		if (rError)
		{
			DebugErrorL("Couldn't read an item in the directory: ", lSmLocalData);
			continue;
		}

		if (!cur_dir.is_directory()) continue;

		std::wstring l_mod_path = cur_dir.path().wstring() + L"\\Mods";

		if (File::Exists(l_mod_path))
		{
			DebugOutL("Found a new path to mods: ", 0b01101_fg, l_mod_path);
			DatabaseConfig::AddToStrVec(DatabaseConfig::ModFolders, l_mod_path);
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

		const auto& wd_mod_folders = JsonReader::Get(user_settings, "WorkshopModFolders");
		if (wd_mod_folders.is_array())
		{
			for (const auto& ws_item : wd_mod_folders)
			{
				if (!ws_item.is_string()) continue;

				const std::string ws_mod_folder = ws_item.get<std::string>();
				DatabaseConfig::ModFolders.push_back(String::ToWide(ws_mod_folder));
				DebugOutL("Added a new mod folder: ", ws_mod_folder);
			}
		}
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

	if (!cfgData.contains("ProgramSettings"))
	{
		cfgData["ProgramSettings"] =
		{
			{
				"Keywords",
				{
					{ "$CHALLENGE_DATA", "$GAME_FOLDER/ChallengeData" },
					{ "$GAME_DATA"     , "$GAME_FOLDER/Data"          },
					{ "$SURVIVAL_DATA" , "$GAME_FOLDER/Survival"      }
				}
			},
			{
				"ResourceUpgradeFiles",
				{
					"$GAME_DATA/upgrade_resources.json"
				}
			},
			{
				"ScrapAssetDatabase",
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
				}
			}
		};

		if (should_write != nullptr)
			*should_write = true;
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

		{
			nlohmann::json ws_mod_folders = nlohmann::json::array();
			for (const std::wstring& mod_dir : DatabaseConfig::ModFolders)
				ws_mod_folders.push_back(String::ToUtf8(mod_dir));

			user_settings["WorkshopModFolders"] = ws_mod_folders;
		}

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