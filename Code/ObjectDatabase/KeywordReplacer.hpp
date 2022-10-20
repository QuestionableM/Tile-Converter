#pragma once

#include "Utils/Uuid.hpp"
#include <unordered_map>

class KeywordReplacer
{
	using StringMap = std::unordered_map<std::wstring, std::wstring>;

	inline static StringMap m_KeyReplacements  = {};
	inline static StringMap m_ResourceUpgrades = {};

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