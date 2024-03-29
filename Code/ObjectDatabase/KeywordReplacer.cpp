#include "KeywordReplacer.hpp"

#include "ObjectDatabase\DatabaseConfig.hpp"
#include "Utils\String.hpp"
#include "Console.hpp"

void KeywordReplacer::CreateKey(std::wstring& key, std::wstring& replacement)
{
	String::ToLowerR(key);
	String::ToLowerR(replacement);

	String::ReplaceAllR(key,         L'\\', L'/');
	String::ReplaceAllR(replacement, L'\\', L'/');
}

void KeywordReplacer::SetReplacement(const std::wstring& key, const std::wstring& replacement)
{
	std::wstring v_lowerKey = key;
	std::wstring v_lowerVal = replacement;

	KeywordReplacer::CreateKey(v_lowerKey, v_lowerVal);

	const StringMap::iterator v_iter = m_KeyReplacements.find(v_lowerKey);
	if (v_iter != m_KeyReplacements.end())
	{
		v_iter->second = v_lowerVal;
		return;
	}

	m_KeyReplacements.insert(std::make_pair(v_lowerKey, v_lowerVal));
}

void KeywordReplacer::SetModData(const std::wstring& path, const SMUuid& uuid)
{
	const std::wstring v_contentKey = L"$content_" + uuid.ToWstring();

	KeywordReplacer::SetReplacement(v_contentKey    , path);
	KeywordReplacer::SetReplacement(L"$mod_data"    , path);
	KeywordReplacer::SetReplacement(L"$content_data", path);
}

void KeywordReplacer::ClearContentKey()
{
	const StringMap::iterator v_iter = m_KeyReplacements.find(L"$content_data");
	if (v_iter == m_KeyReplacements.end())
		return;

	m_KeyReplacements.erase(v_iter);
}

bool KeywordReplacer::ContentKeyExists()
{
	return (m_KeyReplacements.find(L"$content_data") != m_KeyReplacements.end());
}

void KeywordReplacer::UpgradeResource(const std::wstring& mPath, std::wstring& mOutput)
{
	std::wstring v_lowerPath = mPath;

	{
		String::ToLowerR(v_lowerPath);
		String::ReplaceAllR(v_lowerPath, L'\\', L'/');
	}

	const StringMap::const_iterator v_iter = m_ResourceUpgrades.find(v_lowerPath);
	if (v_iter != m_ResourceUpgrades.end())
	{
		mOutput = v_iter->second;
	}
	else
	{
		mOutput = v_lowerPath;
	}
}

void KeywordReplacer::LoadResourceUpgradesFromConfig()
{
	for (const std::wstring& v_upgrades_path : DatabaseConfig::ResourceUpgradeFiles)
		KeywordReplacer::LoadResourceUpgrades(v_upgrades_path);
}

void KeywordReplacer::LoadResourceUpgrades(const std::wstring& path)
{
	simdjson::dom::document v_doc;
	if (!JsonReader::LoadParseSimdjsonCommentsC(path, v_doc, simdjson::dom::element_type::OBJECT))
		return;

	DebugOutL("Loading resource upgrades: ", 0b1101_fg, path);

	const auto v_upgrade_array = v_doc.root()["upgrade"];
	if (!v_upgrade_array.is_array())
		return;

	for (const auto v_upgrade_list : v_upgrade_array.get_array())
	{
		if (!v_upgrade_list.is_array()) continue;

		for (const auto v_upgrade_obj : v_upgrade_list.get_array())
		{
			if (!v_upgrade_obj.is_array()) continue;

			const auto v_upgrade_obj_arr = v_upgrade_obj.get_array();

			const auto v_upgrade_key = v_upgrade_obj_arr.at(0);
			const auto v_upgrade_val = v_upgrade_obj_arr.at(1);

			if (!(v_upgrade_key.is_string() && v_upgrade_val.is_string())) continue;

			std::wstring v_key_wstr = String::ToWide(v_upgrade_key.get_string());
			std::wstring v_val_wstr = String::ToWide(v_upgrade_val.get_string());

			KeywordReplacer::CreateKey(v_key_wstr, v_val_wstr);

			if (m_ResourceUpgrades.find(v_key_wstr) != m_ResourceUpgrades.end())
				m_ResourceUpgrades.insert(std::make_pair(v_key_wstr, v_val_wstr));
		}
	}
}

std::wstring KeywordReplacer::ReplaceKey(const std::wstring& path)
{
	if (path.empty())
		return path;

	std::wstring v_output;
	KeywordReplacer::UpgradeResource(path, v_output);

	if (v_output[0] != L'$')
		return v_output;

	const std::size_t v_key_idx = v_output.find_first_of(L'/');
	if (v_key_idx == std::wstring::npos)
		return v_output;

	const std::wstring v_key_chunk = v_output.substr(0, v_key_idx);
	const StringMap::const_iterator v_iter = m_KeyReplacements.find(v_key_chunk);
	if (v_iter == m_KeyReplacements.end())
		return v_output;

	return (v_iter->second + v_output.substr(v_key_idx));
}

void KeywordReplacer::ReplaceKeyR(std::wstring& path)
{
	if (path.empty())
		return;

	KeywordReplacer::UpgradeResource(path, path);

	if (path[0] != L'$')
		return;

	const std::size_t v_key_idx = path.find_first_of(L'/');
	if (v_key_idx == std::wstring::npos)
		return;

	const std::wstring v_key_chunk = path.substr(0, v_key_idx);
	const StringMap::const_iterator v_iter = m_KeyReplacements.find(v_key_chunk);
	if (v_iter == m_KeyReplacements.end())
		return;

	path = (v_iter->second + path.substr(v_key_idx));
}

bool KeywordReplacer::ReplaceKeyRLua(std::wstring& path)
{
	if (path.empty())
		return true;

	KeywordReplacer::UpgradeResource(path, path);

	if (path[0] != L'$')
		return true;

	const std::size_t v_key_idx = path.find_first_of(L'/');
	if (v_key_idx == std::wstring::npos)
		return true;

	const std::wstring v_key_chunk = path.substr(0, v_key_idx);
	if (v_key_chunk == L"$content_data")
	{
		//Throw key error if $CONTENT_DATA doesn't exist
		if (!KeywordReplacer::ContentKeyExists())
			return false;
	}

	const StringMap::const_iterator v_iter = m_KeyReplacements.find(v_key_chunk);
	if (v_iter == m_KeyReplacements.end())
		return true;

	path = (v_iter->second + path.substr(v_key_idx));
	return true;
}

void KeywordReplacer::Initialize()
{
	DebugOutL(__FUNCTION__);

	KeywordReplacer::Clear();
	KeywordReplacer::LoadResourceUpgradesFromConfig();

	for (const auto& v_kv_pair : DatabaseConfig::DefaultKeywords)
		KeywordReplacer::SetReplacement(v_kv_pair.first, v_kv_pair.second);
}

void KeywordReplacer::Clear()
{
	DebugOutL(__FUNCTION__);

	m_KeyReplacements.clear();
	m_ResourceUpgrades.clear();
}