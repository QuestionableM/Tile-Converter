#pragma once
#pragma unmanaged

#include <nlohmann\json.hpp>

#if defined(_MT)
	#undef _MT
	#include <simdjson\simdjson.h>
	#define _MT
#else
	#include <simdjson\simdjson.h>
#endif

class JsonReader
{
public:
	static nlohmann::json ParseJsonString(const std::string& json_str);
	static nlohmann::json LoadParseJson(const std::wstring& path);

	static bool LoadParseJsonLua(const std::wstring& path, nlohmann::json& v_json, std::string& v_error_message);
	static bool ParseJsonStringLua(const std::string& json_str, nlohmann::json& v_json, std::string& v_error_message);

	static void WriteJson(const std::wstring& path, const nlohmann::json& pJson);
	static std::string WriteJsonString(const nlohmann::json& v_json);

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

	//Simdjson functions

	//Only used by Simdjson as it doesn't support json with comments
	static std::string RemoveComments(const std::string& json_string);

	//Parse simdjson unchecked
	static bool LoadParseSimdjson(const std::wstring& path, simdjson::dom::document& v_doc);
	//Parse simdjson with root node type check
	static bool LoadParseSimdjsonC(const std::wstring& path, simdjson::dom::document& v_doc, const simdjson::dom::element_type& type_check);
	//Parse simdjson with comments
	static bool LoadParseSimdjsonComments(const std::wstring& path, simdjson::dom::document& v_doc);
	//Parse simdjson with comments and root node type checking
	static bool LoadParseSimdjsonCommentsC(const std::wstring& path, simdjson::dom::document& v_doc, const simdjson::dom::element_type& type_check);

	//Should be used to get numbers from simdjson elements
	template<typename T>
	inline constexpr static T GetNumber(const simdjson::simdjson_result<simdjson::dom::element>& v_elem)
	{
		static_assert(std::is_arithmetic_v<T>, "Json::GetNumber -> Template argument must be of arithmetic type!");

		switch (v_elem.type())
		{
		case simdjson::dom::element_type::DOUBLE:
			{
				if constexpr (std::is_same_v<T, double>)
					return v_elem.get_double();
				else
					return static_cast<T>(v_elem.get_double());
			}
		case simdjson::dom::element_type::INT64:
			{
				if constexpr (std::is_same_v<T, long long>)
					return v_elem.get_int64();
				else
					return static_cast<T>(v_elem.get_int64());
			}
		case simdjson::dom::element_type::UINT64:
			{
				if constexpr (std::is_same_v<T, unsigned long long>)
					return v_elem.get_uint64();
				else
					return static_cast<T>(v_elem.get_uint64());
			}
		}

		return 0;
	}

private:
	inline const static nlohmann::json m_emptyObject = nlohmann::json();

	JsonReader() = default;
	JsonReader(const JsonReader&&) = delete;
	JsonReader(JsonReader&&)       = delete;
	JsonReader(JsonReader&)        = delete;
	~JsonReader() = default;
};

#pragma managed