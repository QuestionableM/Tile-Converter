#pragma once

#include "Utils/Uuid.hpp"
#include <unordered_map>

class KeywordReplacer
{
	static std::unordered_map<std::wstring, std::wstring> KeyReplacements;
	static std::unordered_map<std::wstring, std::wstring> ResourceUpgrades;

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
};