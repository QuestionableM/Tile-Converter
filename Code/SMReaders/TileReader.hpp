#pragma once

#include <string>
#include "Tile/CellHeader.hpp"
#include "Tile/TileHeader.hpp"
#include "Tile/Tile.hpp"

#include "SMReaders\MipReader.hpp"
#include "SMReaders\ClutterReader.hpp"
#include "SMReaders\AssetListReader.hpp"
#include "SMReaders\HarvestableListReader.hpp"
#include "SMReaders\PrefabReader.hpp"
#include "SMReaders\BlueprintListReader.hpp"
#include "SMReaders\DecalListReader.hpp"

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

		DebugOutL("TileFileVersion: ", header->m_Version);
		DebugOutL("TileUuid: ", header->m_Uuid.ToString());
		DebugOutL("CreatorId: ", header->m_CreatorId);
		DebugOutL("Size: ", header->m_Width, ", ", header->m_Height);
		DebugOutL("Type: ", header->m_Type, "\n");
		DebugOutL("Header info:");
		DebugOutL("CellHeaderOffset: ", header->m_CellHeaderOffset);
		DebugOutL("CellHeadersSize: ", header->m_CellHeadersSize, "\n");

		for (int a = 0; a < header->m_Width * header->m_Height; a++)
		{
			int x = a % header->m_Width;
			int y = a / header->m_Width;

			std::vector<Byte> bytes = header->GetHeader(x, y)->Data();
			DebugOutL("\tBLOB(", x, ", ", y, "):");
			DebugOutL("\t\t", String::BytesToHexString(bytes, header->m_CellHeadersSize, 32));
		}

		MemoryWrapper reader(header->TileData());

		const int tileXSize = header->m_Width;
		const int tileYSize = header->m_Width;

		Tile* tile = new Tile(tileXSize, tileYSize);
		tile->SetVersion(header->m_Version);
		tile->SetTileType(header->m_Type);
		tile->SetCreatorId(header->m_CreatorId);

		if (tileYSize > 0)
		{
			for (int y = 0; y < tileYSize; y++)
			{
				if (cError) break;

				for (int x = 0; x < tileXSize; x++)
				{
					CellHeader* h  = header->GetHeader(x, y);
					TilePart* part = tile->GetPart(x, y);

					if (header->m_Type == 0)
					{
						MipReader::Read    (h, reader, part, cError);
						ClutterReader::Read(h, reader, part, cError);
					}
				
					AssetListReader::Read      (h, reader, part, cError);
					PrefabReader::Read         (h, reader, part, cError);
					BlueprintListReader::Read  (h, reader, part, cError);
					HarvestableListReader::Read(h, reader, part, cError);
					DecalListReader::Read      (h, reader, part, cError);
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