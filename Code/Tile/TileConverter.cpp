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

ConvertError::ConvertError(const unsigned short& ec, const std::wstring& error_msg)
{
	this->ErrorCode = ec;
	this->ErrorMessage = error_msg;
}

ConvertError::operator bool() const noexcept
{
	return (this->ErrorCode != 0);
}

std::wstring ConvertError::GetErrorMsg() const noexcept
{
	return this->ErrorMessage;
}

void TileConv::ConvertToModel(const std::wstring& tile_path, const std::wstring& tile_name, ConvertError& cError)
{
	fs::directory_entry fEntry(tile_path);
	if (!fEntry.is_regular_file())
	{
		cError = ConvertError(1, L"The specified path leads to a directory");
		return;
	}

	if (!File::CreateDirectorySafe(TileOutputDirectory.data()))
	{
		cError = ConvertError(1, L"Couldn't create the main output directory");
		return;
	}

	const fs::path& fPath = fEntry.path();
	const std::wstring tile_dir_path = std::wstring(TileOutputDirectory.data()) + L"/" + tile_name;

	if (!File::CreateDirectorySafe(tile_dir_path))
	{
		cError = ConvertError(1, L"Couldn't create the tile output directory!");
		return;
	}

	{
		Tile* out_tile = TileReader::ReadTile(tile_path, cError);

		if (!cError)
		{
			out_tile->WriteToFile(tile_dir_path + L"/", tile_name);
		}

		delete out_tile; //clear the tile data

		ModelStorage::ClearStorage(); // clear the cache
	}
}