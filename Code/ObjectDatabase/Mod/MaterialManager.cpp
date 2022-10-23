#include "MaterialManager.hpp"

#include "ObjectDatabase/DatabaseConfig.hpp"

#include "Utils/String.hpp"
#include "Utils/Json.hpp"

#include "Console.hpp"

MaterialManager::MaterialMap MaterialManager::m_materialStorage = {};

void MaterialManager::Initialize()
{
	nlohmann::json pMatMap = JsonReader::LoadParseJson(DatabaseConfig::MaterialMapPath.data());
	if (!pMatMap.is_object()) return;

	for (const auto& pObject : pMatMap.items())
	{
		if (!pObject.value().is_string()) continue;

		const std::wstring pKey = String::ToWide(pObject.key());
		const std::wstring pValue = String::ToWide(pObject.value().get_ref<const std::string&>());

		if (m_materialStorage.find(pKey) != m_materialStorage.end())
			continue;

		m_materialStorage.insert(std::make_pair(pKey, pValue));
	}
}

std::wstring MaterialManager::GetMaterialW(const std::wstring& mat_name)
{
	const MaterialMap::const_iterator iter = m_materialStorage.find(mat_name);
	if (iter != m_materialStorage.end())
		return L"m" + iter->second;

	DebugOutL("Couldn't find the specified material: ", mat_name);

	return L"m1";
}

std::string MaterialManager::GetMaterialA(const std::wstring& mat_name)
{
	return String::ToUtf8(MaterialManager::GetMaterialW(mat_name));
}