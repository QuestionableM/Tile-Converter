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

#include "Utils/File.hpp"
#include "Utils/String.hpp"

class TileReader
{
	TileReader() = default;

public:
	static Tile* ReadTile(const std::wstring& path)
	{
		std::vector<Byte> file_bytes = File::ReadFileBytes(path);

		return TileReader::ReadTile(file_bytes);
	}

	static Tile* ReadTile(const std::vector<Byte>& tile_data)
	{
		TileHeader* header = TileHeader::ReadTile(tile_data);
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
				for (int x = 0; x < tileXSize; x++)
				{
					CellHeader* h = header->GetHeader(x, y);
					TilePart* part = tile->GetPart(x, y);

					if (header->Type == 0)
					{
						MipReader::Read(h, reader, part);
						ClutterReader::Read(h, reader, part);
					}

					AssetListReader::Read(h, reader, part);
					//NodeReader - not needed
					//ScriptReader - not needed
					PrefabReader::Read(h, reader, part);
					//BlueprintListReader - in the works
					//DecalReader - in the works
					HarvestableListReader::Read(h, reader, part);
				}
			}
		}

		return tile;
	}
};