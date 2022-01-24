#include "MaterialManager.hpp"

#include "ObjectDatabase/DatabaseConfig.hpp"

#include "Utils/String.hpp"
#include "Utils/Json.hpp"

#include "Console.hpp"

std::unordered_map<std::wstring, std::wstring> MaterialManager::MatStorage = {};

void MaterialManager::Initialize()
{
	nlohmann::json pMatMap = JsonReader::LoadParseJson(DatabaseConfig::MaterialMapPath.data());
	if (!pMatMap.is_object()) return;

	for (const auto& pObject : pMatMap.items())
	{
		if (!pObject.value().is_string()) continue;

		const std::wstring pKey = String::ToWide(pObject.key());
		const std::wstring pValue = String::ToWide(pObject.value().get<std::string>());

		if (MatStorage.find(pKey) != MatStorage.end())
			continue;

		MatStorage.insert(std::make_pair(pKey, pValue));
	}
}

std::wstring MaterialManager::GetMaterialW(const std::wstring& mat_name)
{
	if (MatStorage.find(mat_name) != MatStorage.end())
		return L"m" + MatStorage.at(mat_name);

	DebugOutL("Couldn't find the specified material: ", mat_name);

	return L"m1";
}

std::string MaterialManager::GetMaterialA(const std::wstring& mat_name)
{
	const std::wstring wMaterial = MaterialManager::GetMaterialW(mat_name);

	return String::ToUtf8(wMaterial);
}