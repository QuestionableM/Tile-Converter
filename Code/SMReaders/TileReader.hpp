#pragma once

#include <string>
#include "Tile/CellHeader.hpp"
#include "Tile/TileHeader.hpp"
#include "Tile/Tile.hpp"

#include "SMReaders/MipReader.hpp"
#include "SMReaders/ClutterReader.hpp"
#include "SMReaders/AssetListReader.hpp"
#include "SMReaders/HarvestableListReader.hpp"
#include "SMReaders/PrefabReader.hpp"
#include "SMReaders/BlueprintListReader.hpp"

#include "ObjectDatabase/ProgCounter.hpp"

#include "Utils/File.hpp"
#include "Utils/String.hpp"

class TileReader
{
	TileReader() = default;

public:
	static Tile* ReadTile(const std::wstring& path, ConvertError& cError)
	{
		std::vector<Byte> file_bytes = File::ReadFileBytes(path);

		return TileReader::ReadTile(file_bytes, cError);
	}

	static Tile* ReadTile(const std::vector<Byte>& tile_data, ConvertError& cError)
	{
		ProgCounter::SetState(ProgState::ReadingTile, 0);

		TileHeader* header = TileHeader::ReadTile(tile_data, cError);
		if (!header) return nullptr;

		DebugOutL("TileFileVersion: ", header->Version);
		DebugOutL("TileUuid: ", "NOT IMPLEMENTED");
		DebugOutL("CreatorId: ", header->CreatorId);
		DebugOutL("Size: ", header->Width, ", ", header->Height);
		DebugOutL("Type: ", header->Type, "\n");
		DebugOutL("Header info:");
		DebugOutL("CellHeaderOffset: ", header->CellHeaderOffset);
		DebugOutL("CellHeadersSize: ", header->CellHeadersSize, "\n");

		for (int a = 0; a < header->Width * header->Height; a++)
		{
			int x = a % header->Width;
			int y = a / header->Width;

			std::vector<Byte> bytes = header->GetHeader(x, y)->Data();
			DebugOutL("\tBLOB(", x, ", ", y, "):");
			DebugOutL("\t\t", String::BytesToHexString(bytes, header->CellHeadersSize, 32));
		}

		MemoryWrapper reader(header->TileData());

		const int tileXSize = header->Width;
		const int tileYSize = header->Height;

		Tile* tile = new Tile(tileXSize, tileYSize);
		tile->SetVersion(header->Version);
		tile->SetTileType(header->Type);
		tile->SetCreatorId(header->CreatorId);

		if (tileYSize > 0)
		{
			for (int y = 0; y < tileYSize; y++)
			{
				if (cError) break;

				for (int x = 0; x < tileXSize; x++)
				{
					CellHeader* h  = header->GetHeader(x, y);
					TilePart* part = tile->GetPart(x, y);

					if (header->Type == 0)
					{
						MipReader::Read    (h, reader, part, cError);
						ClutterReader::Read(h, reader, part, cError);
					}
				
					AssetListReader::Read      (h, reader, part, cError);
					PrefabReader::Read         (h, reader, part, cError);
					BlueprintListReader::Read  (h, reader, part, cError);
					HarvestableListReader::Read(h, reader, part, cError);
				}
			}
		}

		delete header;

		if (cError)
		{
			delete tile;
			return nullptr;
		}

		return tile;
	}
};