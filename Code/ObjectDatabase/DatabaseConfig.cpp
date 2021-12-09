#include "DatabaseConfig.hpp"
#include "ObjectDatabase/KeywordReplacer.hpp"
#include "Utils/String.hpp"

#include "Console.hpp"

std::wstring DatabaseConfig::GamePath = L"";
std::vector<std::wstring> DatabaseConfig::AssetListFolders = {};
std::vector<std::wstring> DatabaseConfig::ModFolders = {};

void DatabaseConfig::ReadUserSettings(const nlohmann::json& config_json)
{
	const auto& user_settings = JsonReader::Get(config_json, "UserSettings");
	if (!user_settings.is_object()) return;

	const auto& game_path = JsonReader::Get(user_settings, "GamePath");
	if (game_path.is_string())
	{
		const std::string sm_path = game_path.get<std::string>();
		DatabaseConfig::GamePath = String::ToWide(sm_path);
		KeywordReplacer::AddKey(L"$GAME_FOLDER", DatabaseConfig::GamePath);

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

			KeywordReplacer::AddKey(pKey, pValue);
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

void DatabaseConfig::ReadConfig(const std::wstring& path)
{
	nlohmann::json cfgData = JsonReader::LoadParseJson(path);
	if (!cfgData.is_object()) return;

	GamePath.clear();
	AssetListFolders.clear();
	KeywordReplacer::Clear();

	DatabaseConfig::ReadUserSettings(cfgData);
	DatabaseConfig::ReadProgramSettings(cfgData);
}