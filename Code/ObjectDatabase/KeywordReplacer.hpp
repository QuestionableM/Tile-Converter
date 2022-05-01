#pragma once

#include "Utils/Uuid.hpp"
#include <unordered_map>

/*class KeywordReplacer
{
	inline static std::unordered_map<std::wstring, std::wstring> m_KeyReplacements  = {};
	inline static std::unordered_map<std::wstring, std::wstring> m_ResourceUpgrades = {};

public:
	static void SetModData(const SMUuid& uuid, const std::wstring& path);

private:
	static std::wstring ProcessString(const std::wstring& str);
	static void AddPairToMap(const std::wstring& key, const std::wstring& val, std::unordered_map<std::wstring, std::wstring>& map);

public:
	static void AddKey(const std::wstring& key, const std::wstring& replacement);
	static void AddResourceUpgrade(const std::wstring& key, const std::wstring& replacement);

	static void LoadResourceUpgrades(const std::wstring& path);

	static std::wstring GetResourceUpgrade(const std::wstring& path);
	static std::wstring ReplaceKey(const std::wstring& path);
	static void ReplaceKeyR(std::wstring& path);

	static void Clear();
};*/

class KeywordReplacer
{
	inline static std::unordered_map<std::wstring, std::wstring> m_KeyReplacements = {};
	inline static std::unordered_map<std::wstring, std::wstring> m_ResourceUpgrades = {};

	static void CreateKey(std::wstring& key, std::wstring& replacement);
public:
	static void SetReplacement(const std::wstring& key, const std::wstring& replacement);
	static void SetModData(const std::wstring& path, const SMUuid& uuid);

	static void UpgradeResource(const std::wstring& mPath, std::wstring& mOutput);
	static void LoadResourceUpgrades(const std::wstring& path);

	static std::wstring ReplaceKey(const std::wstring& path);
	static void ReplaceKeyR(std::wstring& path);

	static void Clear();
};