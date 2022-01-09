#pragma once

#include <nlohmann/json.hpp>

class JsonReader
{
	const static nlohmann::json EmptyObject;

	static nlohmann::json ParseJsonStringInternal(const std::string& json_str);
public:
	static nlohmann::json ParseJsonString(const std::string& json_str);
	static nlohmann::json LoadParseJson(const std::wstring& path);

	static void WriteJson(const std::wstring& path, const nlohmann::json& pJson);

	static const nlohmann::json& Get(const nlohmann::json& obj, const std::string& key);
	static const nlohmann::json& Get(const nlohmann::json& obj, const std::size_t& key);
};