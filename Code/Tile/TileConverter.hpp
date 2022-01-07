#pragma once

#include <string>

class ConvertSettings
{
	//model settings
	static bool ExportUvs;
	static bool ExportNormals;

	//tile settings
	static bool ExportClutter;
	static bool ExportAssets;
	static bool ExportPrefabs;
	static bool ExportBlueprints;
	static bool ExportHarvestables;
};

enum class ConvertResult : unsigned short
{
	Success             = 0, //no errors
	Error_Unknown       = 1, //unknown error
	Error_IsDirectory   = 2, //path to tile is not a regular file
	Error_MainDirCreate = 3, //the main tile output directory could not be created
	Error_DirCreate     = 4  //the tile output directory could not be created
};

class TileConverter
{
public:
	static ConvertResult ConvertToModel(const std::wstring& tile_path);
};