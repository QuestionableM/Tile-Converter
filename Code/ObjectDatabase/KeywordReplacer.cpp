#include "KeywordReplacer.hpp"

#include "ObjectDatabase/DatabaseConfig.hpp"
#include "Utils/String.hpp"
#include "Console.hpp"

/*void KeywordReplacer::SetModData(const SMUuid& uuid, const std::wstring& path)
{
	if (m_KeyReplacements.find(L"$mod_data") != m_KeyReplacements.end())
	{
		m_KeyReplacements.at(L"$mod_data") = path;
	}
	else
	{
		m_KeyReplacements.insert(std::make_pair(L"$mod_data", path));
	}

	const std::wstring cContentKey = L"$content_" + uuid.ToWstring();
	if (m_KeyReplacements.find(cContentKey) != m_KeyReplacements.end())
	{
		DebugOutL("Mod Uuid Collision!");
	}
	else
	{
		m_KeyReplacements.insert(std::make_pair(cContentKey, path));
	}
}

std::wstring KeywordReplacer::ProcessString(const std::wstring& str)
{
	std::wstring str_copy = str;

	String::ToLowerR(str_copy);
	String::ReplaceAllR(str_copy, L'\\', L'/');

	return str_copy;
}

void KeywordReplacer::AddPairToMap(const std::wstring& key, const std::wstring& val, std::unordered_map<std::wstring, std::wstring>& map)
{
	if (map.find(key) != map.end()) return;

	const std::wstring rKey = KeywordReplacer::ProcessString(key);
	const std::wstring rVal = KeywordReplacer::ProcessString(val);

	map.insert(std::make_pair(rKey, rVal));
}

void KeywordReplacer::AddKey(const std::wstring& key, const std::wstring& replacement)
{
	KeywordReplacer::AddPairToMap(key, replacement, KeyReplacements);
}

void KeywordReplacer::AddResourceUpgrade(const std::wstring& key, const std::wstring& replacement)
{
	KeywordReplacer::AddPairToMap(key, replacement, ResourceUpgrades);
}

void KeywordReplacer::LoadResourceUpgrades(const std::wstring& path) {
	const nlohmann::json uJson = JsonReader::LoadParseJson(path);
	if (!uJson.is_object()) return;

	const auto& upgrade_array = JsonReader::Get(uJson, "upgrade");
	if (!upgrade_array.is_array()) return;

	for (const auto& upgrade_list : upgrade_array) {
		if (!upgrade_list.is_array()) continue;

		for (const auto& upgrade_obj : upgrade_list) {
			const auto& upgrade_key = JsonReader::Get(upgrade_obj, 0);
			const auto& upgrade_val = JsonReader::Get(upgrade_obj, 1);

			if (!(upgrade_key.is_string() && upgrade_val.is_string())) continue;

			std::wstring upKeyWstr = String::ToWide(upgrade_key.get<std::string>());
			std::wstring upValWstr = String::ToWide(upgrade_val.get<std::string>());

			KeywordReplacer::AddResourceUpgrade(upKeyWstr, upValWstr);
		}
	}
}

std::wstring KeywordReplacer::GetResourceUpgrade(const std::wstring& path)
{
	std::wstring lower_path = String::ToLower(path);
	String::ReplaceAllR(lower_path, L'\\', L'/');

	if (ResourceUpgrades.find(lower_path) != ResourceUpgrades.end())
		return ResourceUpgrades.at(lower_path);

	return lower_path;
}

std::wstring KeywordReplacer::ReplaceKey(const std::wstring& path)
{
	const std::wstring upgr_path = KeywordReplacer::GetResourceUpgrade(path);

	//for (const auto& key_repl : KeyReplacements)
	//{
	//	if (upgr_path.find(key_repl.first) != std::wstring::npos)
	//		return (key_repl.second + upgr_path.substr(key_repl.first.size()));
	////}

	return upgr_path;
}

void KeywordReplacer::ReplaceKeyR(std::wstring& path)
{
	const std::wstring upgr_path = KeywordReplacer::GetResourceUpgrade(path);

	for (const auto& key_repl : KeyReplacements)
	{
		if (upgr_path.find(key_repl.first) != std::wstring::npos)
		{
			path = (key_repl.second + upgr_path.substr(key_repl.first.size()));
			return;
		}
	}
}

void KeywordReplacer::Clear()
{
	KeyReplacements.clear();
	ResourceUpgrades.clear();
}*/

void KeywordReplacer::CreateKey(std::wstring& key, std::wstring& replacement)
{
	String::ToLowerR(key);
	String::ToLowerR(replacement);

	String::ReplaceAllR(key,         L'\\', L'/');
	String::ReplaceAllR(replacement, L'\\', L'/');
}

void KeywordReplacer::SetReplacement(const std::wstring& key, const std::wstring& replacement)
{
	std::wstring mLowerKey = key;
	std::wstring mLowerVal = replacement;

	KeywordReplacer::CreateKey(mLowerKey, mLowerVal);

	if (m_KeyReplacements.find(mLowerKey) != m_KeyReplacements.end())
	{
		m_KeyReplacements.at(mLowerKey) = mLowerVal;
		return;
	}

	m_KeyReplacements.insert(std::make_pair(mLowerKey, mLowerVal));
}

void KeywordReplacer::SetModData(const std::wstring& path, const SMUuid& uuid)
{
	const std::wstring mContentKey = String::ToWide("$content_" + uuid.ToString());

	KeywordReplacer::SetReplacement(mContentKey, path);
	KeywordReplacer::SetReplacement(L"$mod_data", path);
}

void KeywordReplacer::UpgradeResource(const std::wstring& mPath, std::wstring& mOutput)
{
	std::wstring mLowerPath = mPath;

	{
		String::ToLowerR(mLowerPath);
		String::ReplaceAllR(mLowerPath, L'\\', L'/');
	}

	if (m_ResourceUpgrades.find(mLowerPath) != m_ResourceUpgrades.end())
	{
		mOutput = m_ResourceUpgrades.at(mLowerPath);
	}
	else
	{
		mOutput = mLowerPath;
	}
}

void KeywordReplacer::LoadResourceUpgrades(const std::wstring& path)
{
	const nlohmann::json uJson = JsonReader::LoadParseJson(path);
	if (!uJson.is_object()) return;

	DebugOutL("Loading resource upgrades: ", ConCol::YELLOW_INT, path);

	const auto& upgrade_array = JsonReader::Get(uJson, "upgrade");
	if (!upgrade_array.is_array()) return;

	for (const auto& upgrade_list : upgrade_array)
	{
		if (!upgrade_list.is_array()) continue;

		for (const auto& upgrade_obj : upgrade_list)
		{
			const auto& upgrade_key = JsonReader::Get(upgrade_obj, 0);
			const auto& upgrade_val = JsonReader::Get(upgrade_obj, 1);

			if (!(upgrade_key.is_string() && upgrade_val.is_string())) continue;

			std::wstring upKeyWstr = String::ToWide(upgrade_key.get<std::string>());
			std::wstring upValWstr = String::ToWide(upgrade_val.get<std::string>());

			KeywordReplacer::CreateKey(upKeyWstr, upValWstr);

			if (m_ResourceUpgrades.find(upKeyWstr) == m_ResourceUpgrades.end())
				m_ResourceUpgrades.insert(std::make_pair(upKeyWstr, upValWstr));
		}
	}
}

std::wstring KeywordReplacer::ReplaceKey(const std::wstring& path)
{
	std::wstring mOutput;
	KeywordReplacer::UpgradeResource(path, mOutput);

	const std::size_t mKeyIdx = mOutput.find_first_of(L'/');
	if (mKeyIdx != std::wstring::npos)
	{
		const std::wstring mKeyChunk = mOutput.substr(0, mKeyIdx);

		if (m_KeyReplacements.find(mKeyChunk) != m_KeyReplacements.end())
			return (m_KeyReplacements.at(mKeyChunk) + mOutput.substr(mKeyIdx));
	}

	return mOutput;
}

void KeywordReplacer::ReplaceKeyR(std::wstring& path)
{
	KeywordReplacer::UpgradeResource(path, path);

	const std::size_t mKeyIdx = path.find_first_of(L'/');
	if (mKeyIdx != std::wstring::npos)
	{
		const std::wstring mKeyChunk = path.substr(0, mKeyIdx);

		if (m_KeyReplacements.find(mKeyChunk) != m_KeyReplacements.end())
			path = (m_KeyReplacements.at(mKeyChunk) + path.substr(mKeyIdx));
	}
}

void KeywordReplacer::Clear()
{
	DebugOutL(__FUNCTION__);

	m_KeyReplacements.clear();
	m_ResourceUpgrades.clear();
}