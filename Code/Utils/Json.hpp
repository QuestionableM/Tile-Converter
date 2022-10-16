#pragma once

#include <nlohmann/json.hpp>

class JsonReader
{
public:
	static nlohmann::json ParseJsonString(const std::string& json_str);
	static nlohmann::json LoadParseJson(const std::wstring& path);

	static void WriteJson(const std::wstring& path, const nlohmann::json& pJson);

	inline static const nlohmann::json& Get(const nlohmann::json& obj, const std::string& key)
	{
		const auto v_iter = obj.find(key);
		if (v_iter != obj.end())
			return v_iter.value();

		return m_emptyObject;
	}

	inline static const nlohmann::json& Get(const nlohmann::json& obj, const std::size_t& key)
	{
		if (key < obj.size())
			return obj.at(key);

		return m_emptyObject;
	}

private:
	inline const static nlohmann::json m_emptyObject = nlohmann::json();
};