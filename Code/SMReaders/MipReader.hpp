#pragma once

#include "Utils/Memory.hpp"
#include "Utils/ByteImpl.hpp"

#include "Tile/CellHeader.hpp"
#include "Tile/TilePart.hpp"

#include "lz4/lz4.h"

#include <sstream>
#include <iomanip>

class MipReader
{
public:
	static void Read(CellHeader* header, MemoryWrapper& reader, TilePart* part)
	{
		MipReader::Read(MipReader::Read(header, reader), part);
	}

	static std::vector<Byte> Read(CellHeader* header, MemoryWrapper& reader)
	{
		return MipReader::Read(header, 0, reader);
	}

	static std::vector<Byte> Read(CellHeader* header, const int& mipOrLevel, MemoryWrapper& reader)
	{
		DebugOutL("MipIndex: ", header->mipIndex[mipOrLevel], ", MipCompressedSize: ", header->mipCompressedSize[mipOrLevel]);
		std::vector<Byte> compressed = reader.Objects<Byte>(header->mipIndex[mipOrLevel], header->mipCompressedSize[mipOrLevel]);

		std::vector<Byte> decompressed_bytes = {};
		decompressed_bytes.resize(header->mipSize[mipOrLevel]);

		int debugSize = LZ4_decompress_fast((char*)compressed.data(), (char*)decompressed_bytes.data(), header->mipSize[mipOrLevel]);
		assert(debugSize == header->mipCompressedSize[mipOrLevel]);

		return decompressed_bytes;
	}

	static void Read(const std::vector<Byte>& bytes, TilePart* part)
	{
		const int wh_mul = 0x21 * 0x21;

		MemoryWrapper memory((Byte*)bytes.data());

		std::vector<float> height;
		std::vector<int> color;

		height.resize(wh_mul);
		color.resize(wh_mul);

		for (int a = 0; a < wh_mul; a++)
		{
			const int mOffset = a * 8;

			height[a] = memory.Object<float>(mOffset);
			color[a] = memory.Object<int>(mOffset + 4);
		}

		std::vector<long long> ground = memory.Objects<long long>(wh_mul * 8, 0x41 * 0x41);

		part->SetVertexColor(color);
		part->SetVertexHeight(height);
		part->SetGroundMaterials(ground);
	}
};