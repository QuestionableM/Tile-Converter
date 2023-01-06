#pragma once

class ConvertSettings
{
	ConvertSettings() = default;
	~ConvertSettings() = default;

public:
	//model settings
	inline static bool ExportUvs = true;
	inline static bool ExportNormals = true;
	inline static bool ExportMaterials = true;
	inline static bool ExportGroundTextures = false;
	inline static bool Export8kGroundTextures = false;

	//tile settings
	inline static bool ExportClutter = true;
	inline static bool ExportAssets = true;
	inline static bool ExportPrefabs = true;
	inline static bool ExportBlueprints = true;
	inline static bool ExportHarvestables = true;
	inline static bool ExportDecals = false;
};