#pragma once

#include <string>

class ConvertSettings
{
	ConvertSettings()  = default;
	~ConvertSettings() = default;

public:
	//model settings
	inline static bool ExportUvs              = true;
	inline static bool ExportNormals          = true;
	inline static bool ExportMaterials        = true;
	inline static bool ExportGroundTextures   = false;
	inline static bool Export8kGroundTextures = false;

	//tile settings
	inline static bool ExportClutter      = true;
	inline static bool ExportAssets       = true;
	inline static bool ExportPrefabs      = true;
	inline static bool ExportBlueprints   = true;
	inline static bool ExportHarvestables = true;
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
	TileConv()  = default;
	~TileConv() = default;

	static void WriteToFileInternal(class Tile* pTile, const std::wstring& tile_name, ConvertError& cError);
public:
	static void ConvertToModel(const std::wstring& tile_path, const std::wstring& tile_name, ConvertError& cError);
};