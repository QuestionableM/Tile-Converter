#include "Json.hpp"

#include "Utils\String.hpp"
#include "Utils\File.hpp"
#include "Console.hpp"

#include <fstream>
#include <iomanip>

nlohmann::json JsonReader::ParseJsonString(const std::string& json_str)
{
	try
	{
		return nlohmann::json::parse(json_str, nullptr, true, true);
	}
#ifdef _DEBUG
	catch (nlohmann::json::parse_error& p_err)
	{
		DebugErrorL("Couldn't parse the specified json string: ", json_str, "\nError: ", p_err.what());
	}
#else
	catch (...) {}
#endif

	return m_emptyObject;
}

nlohmann::json JsonReader::LoadParseJson(const std::wstring& path)
{
	std::string v_fileData;
	if (!File::ReadToString(path, v_fileData))
	{
		DebugErrorL("Couldn't read the specified json file: ", path);
		return m_emptyObject;
	}

	try
	{
		return nlohmann::json::parse(v_fileData, nullptr, true, true);
	}
#ifdef _DEBUG
	catch (nlohmann::json::parse_error& p_err)
	{
		DebugErrorL("Couldn't parse the json file:\nPath: ", path, "\nError: ", p_err.what());
	}
#else
	catch (...) {}
#endif

	return m_emptyObject;
}

bool JsonReader::LoadParseJsonLua(const std::wstring& path, nlohmann::json& v_json, std::string& v_error_message)
{
	std::string v_fileData;
	if (!File::ReadToString(path, v_fileData))
	{
		v_error_message = "Failed to open file";
		return false;
	}

	try
	{
		v_json = nlohmann::json::parse(v_fileData, nullptr, true, true);
		return true;
	}
	catch (nlohmann::json::parse_error& p_err)
	{
		v_error_message = "Parse error: " + std::string(p_err.what());
	}
	catch (...)
	{
		v_error_message = "Unknown json error";
	}

	return false;
}

void JsonReader::WriteJson(const std::wstring& path, const nlohmann::json& pJson)
{
	std::ofstream out_file(path);
	if (!out_file.is_open()) return;

	out_file << std::setw(1) << std::setfill('\t') << pJson;
	out_file.close();
}