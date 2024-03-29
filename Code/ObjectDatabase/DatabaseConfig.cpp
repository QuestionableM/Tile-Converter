#include "DatabaseConfig.hpp"

#include "ObjectDatabase\KeywordReplacer.hpp"
#include "Utils\String.hpp"
#include "Utils\File.hpp"
#include "Console.hpp"

#include <valve_vdf\vdf_parser.hpp>

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

	if (replace_keys)
	{
		for (const auto& v_curVal : v_jArray)
		{
			if (!v_curVal.is_string()) continue;

			const std::wstring v_wstrPath = String::ToWide(v_curVal.get_ref<const std::string&>());
			pWstrVec.push_back(KeywordReplacer::ReplaceKey(v_wstrPath));
		}
	}
	else
	{
		for (const auto& v_curVal : v_jArray)
		{
			if (!v_curVal.is_string()) continue;

			const std::wstring v_wstrPath = String::ToWide(v_curVal.get_ref<const std::string&>());
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
			const auto& v_keyObjVal = key_obj.value();
			if (!v_keyObjVal.is_string()) continue;

			const std::wstring pKey = String::ToWide(key_obj.key());

			std::wstring pValue = String::ToWide(v_keyObjVal.get_ref<const std::string&>());
			KeywordReplacer::ReplaceKeyR(pValue);

			DatabaseConfig::AddKeywordReplacement(pKey, pValue);
		}
	}

	DatabaseConfig::JsonStrArrayToVector(program_settings, "ResourceUpgradeFiles", DatabaseConfig::ResourceUpgradeFiles, true);
	KeywordReplacer::LoadResourceUpgradesFromConfig();

	DatabaseConfig::JsonStrArrayToVector(program_settings, "ScrapAssetDatabase", DatabaseConfig::AssetListFolders, true);
}

using vdf_childs_table = std::unordered_map<std::string, std::shared_ptr<tyti::vdf::object>>;
using vdf_attrib_table = std::unordered_map<std::string, std::string>;
bool DatabaseConfig::GetSteamPaths(std::wstring& r_game_path, std::wstring& r_workshop_path)
{
	std::wstring v_steamPath;
	if (!String::ReadRegistryKey(L"SOFTWARE\\Valve\\Steam", L"SteamPath", v_steamPath))
	{
		if (!String::ReadRegistryKey(L"SOFTWARE\\WOW6432Node\\Valve\\Steam", L"SteamPath", v_steamPath))
		{
			return false;
		}
	}

	if (!File::Exists(v_steamPath))
		return false;

	DebugOutL("Found a steam path: ", v_steamPath);

	const std::wstring v_guessedGamePath = v_steamPath + L"/steamapps/common/scrap mechanic";
	const std::wstring v_guessedWorkshopPath = v_steamPath + L"/steamapps/workshop/content/387990";

	if (File::Exists(v_guessedGamePath) && File::Exists(v_guessedWorkshopPath))
	{
		r_game_path = v_guessedGamePath;
		r_workshop_path = v_guessedWorkshopPath;

		DebugOutL("Guessed the game path: ", 0b1101_fg, v_guessedGamePath);
		DebugOutL("Guessed the workshop path: ", 0b1101_fg, v_guessedWorkshopPath);

		return true;
	}
	else
	{
		const std::wstring v_vdfPath = v_steamPath + L"/steamapps/libraryfolders.vdf";
		if (!File::Exists(v_vdfPath))
			return false;

		DebugOutL("VDF Path found: ", v_vdfPath);

		std::ifstream v_vdfReader(v_vdfPath);
		if (!v_vdfReader.is_open())
			return false;

		tyti::vdf::basic_object<char> v_vdfRoot = tyti::vdf::read(v_vdfReader);
		v_vdfReader.close();

		if (v_vdfRoot.name != "libraryfolders")
			return false;

		const vdf_childs_table& v_vdfLibFolders = v_vdfRoot.childs;
		for (const auto& v_libFolder : v_vdfLibFolders)
		{
			const vdf_attrib_table& v_libFolderAttribs = v_libFolder.second->attribs;
			const vdf_attrib_table::const_iterator v_attribIter = v_libFolderAttribs.find("path");
			if (v_attribIter == v_libFolderAttribs.end())
				continue;

			const vdf_childs_table& v_libFolderChilds = v_libFolder.second->childs;
			const vdf_childs_table::const_iterator v_childsIter = v_libFolderChilds.find("apps");
			if (v_childsIter == v_libFolderChilds.end())
				continue;

			const vdf_attrib_table& v_appAttribs = v_childsIter->second->attribs;
			const vdf_attrib_table::const_iterator v_appAttribsIter = v_appAttribs.find("387990");
			if (v_appAttribsIter == v_appAttribs.end())
				continue;

			const std::wstring v_libraryPathWstr = String::ToWide(v_attribIter->second);
			const std::wstring v_librarySmPath = v_libraryPathWstr + L"/steamapps/common/scrap mechanic";
			const std::wstring v_libraryWsPath = v_libraryPathWstr + L"/steamapps/workshop/content/387990";

			const bool v_librarySmExists = File::Exists(v_librarySmPath);
			const bool v_libraryWsExists = File::Exists(v_libraryWsPath);

			if (v_librarySmExists)
			{
				DebugOutL("Found a game path from the library: ", 0b1101_fg, v_librarySmPath);
				r_game_path = v_librarySmPath;
			}

			if (v_libraryWsExists)
			{
				DebugOutL("Found a workshop path from the library: ", 0b1101_fg, v_libraryWsPath);
				r_workshop_path = v_libraryWsPath;
			}

			if (v_librarySmExists || v_libraryWsExists)
			{
				DebugOutL("Successfully got the steam paths!");
				return true;
			}
		}
	}

	return false;
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
			DatabaseConfig::GamePath = String::ToWide(game_path.get_ref<const std::string&>());
			DebugOutL("Game Path: ", DatabaseConfig::GamePath);
		}

		DatabaseConfig::JsonStrArrayToVector(user_settings, "LocalModFolders", DatabaseConfig::LocalModFolders, false);
		DatabaseConfig::JsonStrArrayToVector(user_settings, "WorkshopModFolders", DatabaseConfig::ModFolders, false);
	}

	DatabaseConfig::FindGamePath(config_json, should_write);
}

nlohmann::json DatabaseConfig::GetConfigJson(bool* should_write, const bool& read_from_file)
{
	nlohmann::json cfgData;
	if (read_from_file)
	{
		cfgData = JsonReader::LoadParseJson(DatabaseConfig::ConfigPath.data());
		if (!cfgData.is_object())
			cfgData = nlohmann::json::object();
	}
	else
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

	cfgData["ProgramSettings"] = v_programSettings;

	return cfgData;
}

void DatabaseConfig::AddKeywordReplacement(const std::wstring& key, const std::wstring& value)
{
	DebugOutL("Set ", key, " to ", value);

	KeywordReplacer::SetReplacement(key, value);
	DatabaseConfig::DefaultKeywords.push_back(std::make_pair(key, value));
}

void DatabaseConfig::UpdatePathReplacement()
{
	DatabaseConfig::AddKeywordReplacement(L"$GAME_FOLDER", DatabaseConfig::GamePath);
}

void DatabaseConfig::SaveConfig()
{
	nlohmann::json cfgData = DatabaseConfig::GetConfigJson(nullptr, false);

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
	DatabaseConfig::LocalModFolders.clear();
	DatabaseConfig::ResourceUpgradeFiles.clear();
	DatabaseConfig::DefaultKeywords.clear();
	KeywordReplacer::Clear();

	bool should_write = false;
	nlohmann::json cfgData = DatabaseConfig::GetConfigJson(&should_write, true);

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