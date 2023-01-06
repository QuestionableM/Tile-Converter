#include "KeywordReplacer.hpp"

#include "ObjectDatabase/DatabaseConfig.hpp"
#include "Utils/String.hpp"
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

void KeywordReplacer::LoadResourceUpgrades(const std::wstring& path)
{
	const nlohmann::json uJson = JsonReader::LoadParseJson(path);
	if (!uJson.is_object()) return;

	DebugOutL("Loading resource upgrades: ", 0b1101_fg, path);

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

			std::wstring upKeyWstr = String::ToWide(upgrade_key.get_ref<const std::string&>());
			std::wstring upValWstr = String::ToWide(upgrade_val.get_ref<const std::string&>());

			KeywordReplacer::CreateKey(upKeyWstr, upValWstr);

			if (m_ResourceUpgrades.find(upKeyWstr) == m_ResourceUpgrades.end())
				m_ResourceUpgrades.insert(std::make_pair(upKeyWstr, upValWstr));
		}
	}
}

std::wstring KeywordReplacer::ReplaceKey(const std::wstring& path)
{
	std::wstring v_output;
	KeywordReplacer::UpgradeResource(path, v_output);

	if (v_output.empty() || v_output[0] != L'$')
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
	KeywordReplacer::UpgradeResource(path, path);

	if (path.empty() || path[0] != L'$')
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
	KeywordReplacer::UpgradeResource(path, path);

	if (path.empty() || path[0] != L'$')
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

void KeywordReplacer::Clear()
{
	DebugOutL(__FUNCTION__);

	m_KeyReplacements.clear();
	m_ResourceUpgrades.clear();
}