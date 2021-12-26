#include "Json.hpp"

#include "Utils/File.hpp"
#include "Console.hpp"

const nlohmann::json JsonReader::EmptyObject = nlohmann::json();

nlohmann::json JsonReader::ParseJsonStringInternal(const std::string& json_str)
{
	return nlohmann::json::parse(json_str);
}

nlohmann::json JsonReader::ParseJsonString(const std::string& json_str)
{
	try
	{
		return nlohmann::json::parse(json_str, nullptr, true, true);
	}
#ifdef _DEBUG
	catch (nlohmann::json::parse_error& p_err)
	{
		DebugErrorL("Couldn't load the specified json string: ", json_str, "\nError: ", p_err.what());
	}
#else
	catch (...) {}
#endif

	return EmptyObject;
}

nlohmann::json JsonReader::LoadParseJson(const std::wstring& path)
{
	const std::string file_data = File::ReadToString(path);

	try
	{
		return nlohmann::json::parse(file_data, nullptr, true, true);
	}
#ifdef _DEBUG
	catch (nlohmann::json::parse_error& p_err)
	{
		DebugErrorL("Couldn't load the specified json file: ", path, "\nError: ", p_err.what());
	}
#else
	catch (...) {}
#endif

	return EmptyObject;
}

const nlohmann::json& JsonReader::Get(const nlohmann::json& obj, const std::string& key)
{
	if (obj.find(key) != obj.end())
		return obj.at(key);

	return JsonReader::EmptyObject;
}

const nlohmann::json& JsonReader::Get(const nlohmann::json& obj, const std::size_t& key)
{
	if (key < obj.size())
		return obj.at(key);

	return JsonReader::EmptyObject;
}