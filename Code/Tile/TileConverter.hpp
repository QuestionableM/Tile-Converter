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

class ConvertError
{
	unsigned short ErrorCode = 0;
	std::wstring ErrorMessage = L"";
public:
	ConvertError() = default;
	ConvertError(const unsigned short& ec, const std::wstring& error_msg);

	explicit operator bool() const noexcept;
	std::wstring GetErrorMsg() const noexcept;
};

class TileConv
{
public:
	static void ConvertToModel(const std::wstring& tile_path, const std::wstring& tile_name, ConvertError& cError);
};