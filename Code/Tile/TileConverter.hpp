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
public:
	ConvertError() = default;

	inline ConvertError(const unsigned short& ec, const std::wstring& error_msg)
	{
		this->m_errorCode = ec;
		this->m_errorMessage = error_msg;
	}

	inline ConvertError(const std::wstring& error_msg, const std::wstring& func_string)
	{
		this->m_errorCode = 1;
		this->m_errorMessage = func_string + L" -> " + error_msg;
	}

	inline explicit operator bool() const noexcept
	{
		return (m_errorCode != 0);
	}

	inline std::wstring GetErrorMsg() const noexcept
	{
		return m_errorMessage;
	}

private:
	unsigned short m_errorCode = 0;
	std::wstring m_errorMessage;
};

class TileConv
{
	TileConv()  = default;
	~TileConv() = default;

	static void WriteToFileInternal(class Tile* pTile, const std::wstring& tile_name, ConvertError& cError);
public:
	static void ConvertToModel(const std::wstring& tile_path, const std::wstring& tile_name, ConvertError& cError);
};