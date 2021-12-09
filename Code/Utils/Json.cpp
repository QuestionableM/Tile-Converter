#include "Json.hpp"

#include <fstream>
#include <sstream>

#include "Console.hpp"

const nlohmann::json JsonReader::EmptyObject = nlohmann::json();

nlohmann::json JsonReader::LoadParseJson(const std::wstring& path)
{
	std::ifstream input_file(path);
	if (input_file.is_open())
	{
		try
		{
			std::string _RawJson;

			std::stringstream sstream;
			sstream << input_file.rdbuf();
			_RawJson = sstream.str();

			return nlohmann::json::parse(_RawJson, nullptr, true, true);
		}
		catch (...) {}
	}

	DebugErrorL("Couldn't load the specified file: ", path);

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