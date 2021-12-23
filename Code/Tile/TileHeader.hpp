#pragma once

#include "Utils/Memory.hpp"
#include "Utils/ByteImpl.hpp"

#include "Tile/CellHeader.hpp"

class TileHeader
{
	TileHeader() = default;

	std::vector<Byte> tile_bytes = {};

public:
	int Version = 0;
	long long CreatorId = 0;

	int Width = 0;
	int Height = 0;
	int Type = 0;

	int CellHeaderOffset = 0;
	int CellHeadersSize = 0;
	std::vector<CellHeader*> CellHeaders = {};
	MemoryWrapper mMemory;

	void FillHeaderBytes(const std::vector<Byte>& header_bytes)
	{
		const int wh_mul = Width * Height;
		for (int a = 0; a < wh_mul; a++)
		{
			std::vector<Byte> bytes_temp = {};
			bytes_temp.resize(0x124);

			std::memcpy(bytes_temp.data(), header_bytes.data() + (a * 0x124), 0x124);

			CellHeader* part = new CellHeader(bytes_temp);
			part->Read();

			CellHeaders[a] = part;
		}
	}

	static TileHeader* ReadTile(const std::vector<Byte>& bytes)
	{
		TileHeader* new_tile = new TileHeader();
		new_tile->tile_bytes = bytes;
		MemoryWrapper mMemory = (Byte*)bytes.data();
	
		std::vector<char> tile_keyword = mMemory.NextObjects<char>(4);
		std::string tile_key(tile_keyword.begin(), tile_keyword.end());

		if (tile_key != "TILE")
		{
			DebugOutL("Invalid File");
			return nullptr;
		}

		new_tile->Version = mMemory.NextObject<int>();
		DebugOutL("Version: ", new_tile->Version);

		if (new_tile->Version <= 1000000)
		{
			mMemory.Skip(16);
			new_tile->CreatorId = mMemory.NextObject<long long>();
		}

		DebugOutL("CreatorId: ", new_tile->CreatorId);

		new_tile->Width  = mMemory.NextObject<int>();
		new_tile->Height = mMemory.NextObject<int>();

		const int wh_mul = new_tile->Width * new_tile->Height;
		new_tile->CellHeaders.resize(wh_mul);

		DebugOutL("Size: {w: ", new_tile->Width, ", h: ", new_tile->Height, "}");

		new_tile->CellHeaderOffset = mMemory.NextObject<int>();
		new_tile->CellHeadersSize  = mMemory.NextObject<int>();
		mMemory.Skip(8);

		DebugOutL("CellHeadersOffset: ", new_tile->CellHeaderOffset);
		DebugOutL("CellHeadersSize: ", new_tile->CellHeadersSize);

		if (new_tile->Version <= 1000000)
		{
			new_tile->Type = mMemory.NextObject<int>() >> 0x18;
			DebugOutL("Type: ", new_tile->Type);
		}

		if (mMemory.Index() != new_tile->CellHeaderOffset)
		{
			DebugOutL("Error: index doesn't match the cell header offset!");
			return nullptr;
		}

		if (wh_mul != 0)
		{
			std::vector<Byte> headerBytes = {};
			headerBytes.resize(wh_mul * 0x124);

			for (int a = 0; a < wh_mul; a++)
			{
				std::vector<Byte> data = mMemory.NextObjects<Byte>(new_tile->CellHeadersSize);
				std::memcpy(headerBytes.data() + (a * 0x124), data.data(), new_tile->CellHeadersSize);
			}

			new_tile->FillHeaderBytes(headerBytes);
		}

		return new_tile;
	}

	const std::vector<Byte>& TileData() const
	{
		return this->tile_bytes;
	}

	CellHeader* GetHeader(const int& x, const int& y)
	{
		return this->CellHeaders[(std::size_t)(x + y * this->Width)];
	}
};