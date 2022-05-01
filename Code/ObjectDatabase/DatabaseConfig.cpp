#include "DatabaseConfig.hpp"
#include "ObjectDatabase/KeywordReplacer.hpp"
#include "Utils/String.hpp"
#include "Utils/File.hpp"

#include "Console.hpp"

std::wstring DatabaseConfig::GamePath = L"";
std::vector<std::wstring> DatabaseConfig::AssetListFolders = {};
std::vector<std::wstring> DatabaseConfig::ModFolders = {};

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

void DatabaseConfig::ReadUserSettings(nlohmann::json& config_json, bool& should_write)
{
	const auto& user_settings = JsonReader::Get(config_json, "UserSettings");
	if (user_settings.is_object())
	{
		const auto& game_path = JsonReader::Get(user_settings, "GamePath");
		if (game_path.is_string())
		{
			const std::string sm_path = game_path.get<std::string>();
			DatabaseConfig::GamePath = String::ToWide(sm_path);

			DebugOutL("Game path: ", DatabaseConfig::GamePath);
		}

		const auto& ws_mod_folders = JsonReader::Get(user_settings, "WorkshopModFolders");
		if (ws_mod_folders.is_array())
		{
			for (const auto& ws_item : ws_mod_folders)
			{
				if (!ws_item.is_string()) continue;

				const std::string ws_mod_folder = ws_item.get<std::string>();
				ModFolders.push_back(String::ToWide(ws_mod_folder));
				DebugOutL("Added a new mod folder: ", ws_mod_folder);
			}
		}
	}
	
	if (DatabaseConfig::GamePath.empty() || !File::Exists(DatabaseConfig::GamePath))
	{
		std::wstring game_path, ws_path;

		if (DatabaseConfig::GetSteamPaths(game_path, ws_path))
		{
			nlohmann::json u_set = {};

			u_set["GamePath"] = String::ToUtf8(game_path);
			DatabaseConfig::GamePath = game_path;

			DebugOutL(ConCol::BLUE_INT, "Found a game path from the registry: ", DatabaseConfig::GamePath);
			
			if (ModFolders.empty())
			{
				nlohmann::json ws_mods_array = nlohmann::json::array();
				ws_mods_array.push_back(String::ToUtf8(ws_path));
				ModFolders.push_back(ws_path);

				u_set["WorkshopModFolders"] = ws_mods_array;
				DebugOutL(ConCol::BLUE_INT, "Found a workshop path from the registry: ", ws_path);
			}
			else
			{
				u_set["WorkshopModFolders"] = JsonReader::Get(user_settings, "WorkshopModFolders");
			}

			config_json["UserSettings"] = u_set;
			should_write = true;
		}
		else
		{
			return;
		}
	}

	KeywordReplacer::SetReplacement(L"$GAME_FOLDER", DatabaseConfig::GamePath);
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

	const auto& pResourceUpgradeFiles = JsonReader::Get(program_settings, "ResourceUpgradeFiles");
	if (pResourceUpgradeFiles.is_array())
	{
		for (const auto& pResourceUpgradePath : pResourceUpgradeFiles)
		{
			if (!pResourceUpgradePath.is_string()) continue;

			std::wstring pResourcePath = String::ToWide(pResourceUpgradePath.get<std::string>());
			KeywordReplacer::ReplaceKeyR(pResourcePath);

			KeywordReplacer::LoadResourceUpgrades(pResourcePath);
		}
	}

	const auto& pScarpAssetDb = JsonReader::Get(program_settings, "ScrapAssetDatabase");
	if (pScarpAssetDb.is_array())
	{
		for (const auto& pAssetDir : pScarpAssetDb)
		{
			if (!pAssetDir.is_string()) continue;

			std::wstring pAssetDirStr = String::ToWide(pAssetDir.get<std::string>());
			KeywordReplacer::ReplaceKeyR(pAssetDirStr);

			AssetListFolders.push_back(pAssetDirStr);
		}
	}
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
		nlohmann::json pSet = nlohmann::json::object();

		nlohmann::json pKeywords = nlohmann::json::object();
		pKeywords["$CHALLENGE_DATA"] = "$GAME_FOLDER/ChallengeData";
		pKeywords["$GAME_DATA"] = "$GAME_FOLDER/Data";
		pKeywords["$SURVIVAL_DATA"] = "$GAME_FOLDER/Survival";

		nlohmann::json pResourceUpgrades = nlohmann::json::array();
		pResourceUpgrades.push_back("$GAME_DATA/upgrade_resources.json");

		nlohmann::json pAssetDB = nlohmann::json::array();
		pAssetDB.push_back("$CHALLENGE_DATA/Terrain/Database/AssetSets");
		pAssetDB.push_back("$SURVIVAL_DATA/Terrain/Database/AssetSets");
		pAssetDB.push_back("$GAME_DATA/Terrain/Database/AssetSets");

		pAssetDB.push_back("$SURVIVAL_DATA/Harvestables/Database/HarvestableSets");

		pAssetDB.push_back("$CHALLENGE_DATA/Objects/Database/ShapeSets");
		pAssetDB.push_back("$GAME_DATA/Objects/Database/ShapeSets");
		pAssetDB.push_back("$SURVIVAL_DATA/Objects/Database/ShapeSets");

		pAssetDB.push_back("$GAME_DATA/Terrain/Database/clutter.json");
		pAssetDB.push_back("$SURVIVAL_DATA/Terrain/Database/clutter.json");

		pSet["Keywords"] = pKeywords;
		pSet["ResourceUpgradeFiles"] = pResourceUpgrades;
		pSet["ScrapAssetDatabase"] = pAssetDB;

		cfgData["ProgramSettings"] = pSet;

		if (should_write != nullptr)
			*should_write = true;
	}

	return cfgData;
}

void DatabaseConfig::SaveConfig()
{
	nlohmann::json cfgData = DatabaseConfig::GetConfigJson();
	
	nlohmann::json user_settings = nlohmann::json::object();

	user_settings["GamePath"] = String::ToUtf8(DatabaseConfig::GamePath);

	{
		nlohmann::json ws_mod_folders = nlohmann::json::array();

		for (const std::wstring& mod_dir : DatabaseConfig::ModFolders)
			ws_mod_folders.push_back(String::ToUtf8(mod_dir));

		user_settings["WorkshopModFolders"] = ws_mod_folders;
	}

	cfgData["UserSettings"] = user_settings;

	DebugOutL(ConCol::PINK_INT, "Saving a new config...");
	JsonReader::WriteJson(DatabaseConfig::ConfigPath.data(), cfgData);
}

void DatabaseConfig::ReadConfig()
{
	GamePath.clear();
	AssetListFolders.clear();
	ModFolders.clear();
	KeywordReplacer::Clear();

	bool should_write = false;
	nlohmann::json cfgData = DatabaseConfig::GetConfigJson(&should_write);

	DatabaseConfig::ReadUserSettings(cfgData, should_write);
	DatabaseConfig::ReadProgramSettings(cfgData);

	if (should_write)
	{
		DebugOutL(ConCol::PINK_INT, "Writing a new Config.json...");
		JsonReader::WriteJson(DatabaseConfig::ConfigPath.data(), cfgData);
	}
}