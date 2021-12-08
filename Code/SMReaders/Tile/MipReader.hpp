#pragma once

#include "SMReaders/Data/Memory.hpp"
#include "SMReaders/Tile/CellHeader.hpp"
#include "SMReaders/Tile/TilePart.hpp"
#include "SMReaders/ByteImpl.hpp"
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