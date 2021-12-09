#pragma once

#include <nlohmann/json.hpp>

class JsonReader
{
	const static nlohmann::json EmptyObject;
public:
	static nlohmann::json LoadParseJson(const std::wstring& path);

	static const nlohmann::json& Get(const nlohmann::json& obj, const std::string& key);
	static const nlohmann::json& Get(const nlohmann::json& obj, const std::size_t& key);
};