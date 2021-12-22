#pragma once

#include <string>
#include "Tile/CellHeader.hpp"
#include "Tile/TileHeader.hpp"
#include "Tile/Tile.hpp"

#include "SMReaders/MipReader.hpp"
#include "SMReaders/ClutterReader.hpp"
#include "SMReaders/AssetListReader.hpp"
#include "SMReaders/HarvestableListReader.hpp"

#include <sstream>
#include <iomanip>
#include <fstream>

class TileReader
{
	static std::vector<Byte> ReadBytes(const std::wstring& path)
	{
		std::ifstream input_file(path, std::ios::binary);
		std::vector<Byte> file_bytes = {};

		if (input_file.is_open())
		{
			input_file.seekg(0, std::ios::end);
			const std::size_t file_size = (std::size_t)input_file.tellg();
			input_file.seekg(0, std::ios::beg);

			file_bytes.resize(file_size);
			input_file.read((char*)file_bytes.data(), file_size);

			input_file.close();
		}

		return file_bytes;
	}

	TileReader() = default;

public:
	static Tile* ReadTile(const std::wstring& path)
	{
		std::vector<Byte> file_bytes = TileReader::ReadBytes(path);

		return TileReader::ReadTile(file_bytes);
	}

	static std::string GetHexString(const std::vector<Byte>& bytes, const int& maxLength, const int& lineLength)
	{
		std::string output_string;
		int a = 1;

		for (std::size_t i = 0; i < std::min(bytes.size(), (std::size_t)maxLength); i++)
		{
			std::stringstream sstream;
			sstream << std::setfill('0') << std::setw(sizeof(Byte) * 2) << std::hex << (int)bytes[i];

			output_string.append(sstream.str());

			if ((a++) % lineLength == 0) output_string.append("\n\t\t");
		}

		return output_string;
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
			DebugOutL("\t\t", GetHexString(bytes, header->CellHeadersSize, 32));
		}

		MemoryWrapper reader((Byte*)header->TileData().data());

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
					//PrefabReader - in the works
					//BlueprintListReader - in the works
					//DecalReader - in the works
					HarvestableListReader::Read(h, reader, part);
				}
			}
		}

		return tile;
	}
};