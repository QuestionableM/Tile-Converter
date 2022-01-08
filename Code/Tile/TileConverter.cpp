#include "TileConverter.hpp"
#include "SMReaders/TileReader.hpp"

#include "Utils/File.hpp"

#include <filesystem>
namespace fs = std::filesystem;

bool ConvertSettings::ExportUvs     = false;
bool ConvertSettings::ExportNormals = false;

bool ConvertSettings::ExportClutter      = false;
bool ConvertSettings::ExportAssets       = false;
bool ConvertSettings::ExportPrefabs      = false;
bool ConvertSettings::ExportBlueprints   = false;
bool ConvertSettings::ExportHarvestables = false;

static std::wstring TileOutputDirectory = L"./ConvertedTiles";

ConvertResult TileConv::ConvertToModel(const std::wstring& tile_path)
{
	fs::directory_entry fEntry(tile_path);
	if (!fEntry.is_regular_file()) return ConvertResult::Error_IsDirectory;

	if (!File::CreateDirectorySafe(TileOutputDirectory)) return ConvertResult::Error_MainDirCreate;

	const fs::path& fPath = fEntry.path();
	const std::wstring file_name = fPath.has_stem() ? fPath.stem().wstring() : L"UnknownTile";
	const std::wstring tile_dir_path = TileOutputDirectory + L"/" + file_name;

	if (!File::CreateDirectorySafe(tile_dir_path)) return ConvertResult::Error_DirCreate;

	{
		Tile* out_tile = TileReader::ReadTile(tile_path);
		out_tile->WriteToFile(tile_dir_path + L"/", file_name);

		delete out_tile; //clear the tile data

		ModelStorage::ClearStorage(); // clear the cache
	}

	return ConvertResult::Success;
}