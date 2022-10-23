#include "BlocksAndPartsMod.hpp"

#include "ObjectDatabase\KeywordReplacer.hpp"

#include "Utils\File.hpp"
#include "Utils\String.hpp"

#include "Console.hpp"

static const std::wstring g_ShapeSetDbExtensions[2] = { L"json", L"shapedb" };
bool BlocksAndPartsMod::GetShapeSetDatabaseFile(const std::wstring& mod_folder, std::wstring& r_shapedb_path)
{
	const std::wstring lNearFullPath = mod_folder + L"/Objects/Database/shapesets.";

	for (__int8 a = 0; a < 2; a++)
	{
		const std::wstring lFullShapedbPath = lNearFullPath + g_ShapeSetDbExtensions[a];

		if (File::Exists(lFullShapedbPath))
		{
			r_shapedb_path = lFullShapedbPath;
			return true;
		}
	}

	return false;
}

void BlocksAndPartsMod::LoadShapeSetDatabase(const std::wstring& path, Mod* pMod)
{
	const nlohmann::json l_ShapedbJson = JsonReader::LoadParseJson(path);
	if (!l_ShapedbJson.is_object()) return;

	const auto& l_ShapeSetList = JsonReader::Get(l_ShapedbJson, "shapeSetList");
	if (!l_ShapeSetList.is_array()) return;

	for (const auto& l_shapeset : l_ShapeSetList)
	{
		if (!l_shapeset.is_string()) continue;

		const std::wstring l_shapeset_wide = String::ToWide(l_shapeset.get_ref<const std::string&>());
		const std::wstring l_shapeset_path = KeywordReplacer::ReplaceKey(l_shapeset_wide);

		pMod->LoadFile(l_shapeset_path);
	}
}

void BlocksAndPartsMod::LoadObjectDatabase()
{
	KeywordReplacer::SetModData(m_Directory, m_Uuid);

	std::wstring lShapeSetDbPath;
	if (BlocksAndPartsMod::GetShapeSetDatabaseFile(m_Directory, lShapeSetDbPath))
	{
		BlocksAndPartsMod::LoadShapeSetDatabase(lShapeSetDbPath, this);
	}
	else
	{
		const std::wstring l_DatabaseFolder = m_Directory + L"/Objects/Database/ShapeSets";
		if (File::Exists(l_DatabaseFolder))
		{
			this->ScanDatabaseFolder(l_DatabaseFolder);
		}
	}
}