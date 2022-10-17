#pragma once

#include "Utils/Memory.hpp"
#include "Utils/ByteImpl.hpp"
#include "Utils\Uuid.hpp"

#include "Tile/CellHeader.hpp"

class TileHeader
{
	TileHeader() = default;

	std::vector<Byte> m_TileBytes = {};

public:
	int m_Version;
	long long m_CreatorId;
	SMUuid m_Uuid;

	int m_Width;
	int m_Height;
	int m_Type;

	int m_CellHeaderOffset;
	int m_CellHeadersSize;
	std::vector<CellHeader*> m_CellHeaders = {};

	~TileHeader()
	{
		for (std::size_t a = 0; a < m_CellHeaders.size(); a++)
			delete m_CellHeaders[a];
	}

	void FillHeaderBytes(const std::vector<Byte>& header_bytes, const std::size_t& header_size)
	{
		const int wh_mul = m_Width * m_Height;
		for (int a = 0; a < wh_mul; a++)
		{
			std::vector<Byte> bytes_temp = {};
			bytes_temp.resize(header_size);

			std::memcpy(bytes_temp.data(), header_bytes.data() + (a * header_size), header_size);

			CellHeader* p_Header = new CellHeader(bytes_temp);
			p_Header->Read();

			this->m_CellHeaders[a] = p_Header;
		}
	}

	static TileHeader* ReadTile(const std::vector<Byte>& bytes, ConvertError& cError)
	{
		MemoryWrapper mMemory = bytes;
	
		std::vector<char> tile_keyword = mMemory.NextObjects<char>(4);
		std::string tile_key(tile_keyword.begin(), tile_keyword.end());

		if (tile_key != "TILE")
		{
			DebugOutL("Invalid File");
			cError = ConvertError(1, L"TileHeader::ReadTile -> Invalid File");
			return nullptr;
		}

		TileHeader* new_tile = new TileHeader();
		new_tile->m_TileBytes = bytes;

		new_tile->m_Version = mMemory.NextObject<int>();
		DebugOutL("Version: ", new_tile->m_Version);

		if (new_tile->m_Version <= 1000000)
		{
			new_tile->m_Uuid = mMemory.NextObject<SMUuid>();
			new_tile->m_CreatorId = mMemory.NextObject<long long>();
		}

		DebugOutL("CreatorId: ", new_tile->m_CreatorId);
		DebugOutL("Uuid: ", new_tile->m_Uuid.ToString());

		new_tile->m_Width  = mMemory.NextObject<int>();
		new_tile->m_Height = mMemory.NextObject<int>();

		const int wh_mul = new_tile->m_Width * new_tile->m_Height;
		new_tile->m_CellHeaders.resize(wh_mul);

		DebugOutL("Size: {w: ", new_tile->m_Width, ", h: ", new_tile->m_Height, "}");

		new_tile->m_CellHeaderOffset = mMemory.NextObject<int>();
		new_tile->m_CellHeadersSize  = mMemory.NextObject<int>();
		mMemory.Skip(8);

		DebugOutL("CellHeadersOffset: ", new_tile->m_CellHeaderOffset);
		DebugOutL("CellHeadersSize: ",   new_tile->m_CellHeadersSize);

		if (new_tile->m_Version <= 1000000)
		{
			new_tile->m_Type = mMemory.NextObject<int>() >> 0x18;
			DebugOutL("Type: ", new_tile->m_Type);
		}

		if (mMemory.Index() != new_tile->m_CellHeaderOffset)
		{
			DebugOutL("Error: index doesn't match the cell header offset!");
			cError = ConvertError(1, L"TileHeader::ReadTile -> Index doesn't match the cell header offset!");

			delete new_tile;
			return nullptr;
		}

		if (wh_mul != 0)
		{
			//Keep an eye on this line of code, in case the reader breaks
			//388 bytes : 292 bytes
			const std::size_t header_size = (new_tile->m_Version > 9) ? 0x184 : 0x124;

			std::vector<Byte> headerBytes = {};
			headerBytes.resize(wh_mul * header_size); //0x124

			for (int a = 0; a < wh_mul; a++)
				mMemory.NextObjectsRef(headerBytes.data() + (a * header_size), new_tile->m_CellHeadersSize);

			new_tile->FillHeaderBytes(headerBytes, header_size);
		}

		return new_tile;
	}

	const std::vector<Byte>& TileData() const
	{
		return m_TileBytes;
	}

	CellHeader* GetHeader(const int& x, const int& y)
	{
		return m_CellHeaders[(std::size_t)(x + y * m_Width)];
	}
};