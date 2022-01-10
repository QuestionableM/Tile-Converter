#pragma once

#include <string>

class ConvertSettings
{
public:
	//model settings
	static bool ExportUvs;
	static bool ExportNormals;
	static bool ExportMaterials;

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

class ConvertError
{
	unsigned short ErrorCode;
	std::wstring ErrorMessage;
public:
	ConvertError(const unsigned short& ec, const std::wstring& error_msg);
};

class TileConv
{
public:
	static ConvertResult ConvertToModel(const std::wstring& tile_path, const std::wstring& tile_name);
};