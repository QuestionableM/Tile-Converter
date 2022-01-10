#include "TileConverter.hpp"
#include "SMReaders/TileReader.hpp"

#include "Utils/File.hpp"

#include <filesystem>
namespace fs = std::filesystem;

bool ConvertSettings::ExportUvs       = true;
bool ConvertSettings::ExportNormals   = true;
bool ConvertSettings::ExportMaterials = true;

bool ConvertSettings::ExportClutter      = true;
bool ConvertSettings::ExportAssets       = true;
bool ConvertSettings::ExportPrefabs      = true;
bool ConvertSettings::ExportBlueprints   = true;
bool ConvertSettings::ExportHarvestables = true;

constexpr const static std::wstring_view TileOutputDirectory = L"./ConvertedTiles";

ConvertResult TileConv::ConvertToModel(const std::wstring& tile_path, const std::wstring& tile_name)
{
	fs::directory_entry fEntry(tile_path);
	if (!fEntry.is_regular_file()) return ConvertResult::Error_IsDirectory;

	if (!File::CreateDirectorySafe(TileOutputDirectory.data())) return ConvertResult::Error_MainDirCreate;

	const fs::path& fPath = fEntry.path();
	const std::wstring tile_dir_path = std::wstring(TileOutputDirectory.data()) + L"/" + tile_name;

	if (!File::CreateDirectorySafe(tile_dir_path)) return ConvertResult::Error_DirCreate;

	{
		Tile* out_tile = TileReader::ReadTile(tile_path);
		out_tile->WriteToFile(tile_dir_path + L"/", tile_name);

		delete out_tile; //clear the tile data

		ModelStorage::ClearStorage(); // clear the cache
	}

	return ConvertResult::Success;
}