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
	MipReader() = default;

public:
	inline static void Read(CellHeader* header, MemoryWrapper& reader, TilePart* part, ConvertError& cError)
	{
		if (cError) return;

		const std::vector<Byte> bytes = MipReader::Read(header, 0, reader, cError);
		if (bytes.empty()) return;

		MipReader::Read(bytes, part);
	}

#pragma warning(push)
#pragma warning(disable : 4996)

	inline static std::vector<Byte> Read(CellHeader* header, const int& mipOrLevel, MemoryWrapper& reader, ConvertError& cError)
	{
		DebugOutL("MipIndex: ", header->mipIndex[mipOrLevel], ", MipCompressedSize: ", header->mipCompressedSize[mipOrLevel]);
		std::vector<Byte> compressed = reader.Objects<Byte>(header->mipIndex[mipOrLevel], header->mipCompressedSize[mipOrLevel]);

		std::vector<Byte> decompressed_bytes = {};
		decompressed_bytes.resize(header->mipSize[mipOrLevel]);

		int debugSize = LZ4_decompress_fast((char*)compressed.data(), (char*)decompressed_bytes.data(), header->mipSize[mipOrLevel]);
		if (debugSize != header->mipCompressedSize[mipOrLevel])
		{
			cError = ConvertError(1, L"MipReader::Read -> debugSize != header->mipCompressedSize[mipOrLevel]");
			return {};
		}

		return decompressed_bytes;
	}

#pragma warning(pop)

	inline static void Read(const std::vector<Byte>& bytes, TilePart* part)
	{
		//color and height map is 0x21 * 0x21 (33 * 33)
		constexpr std::size_t wh_mul = 0x21 * 0x21;
		constexpr std::size_t wh_mul_8 = wh_mul * 8;

		MemoryWrapper memory(bytes);

		for (std::size_t a = 0; a < wh_mul; a++)
		{
			const std::size_t mOffset = a * 8;

			part->m_VertexHeight[a] = memory.Object<float>(mOffset);
			part->m_VertexColor [a] = memory.Object<int>(mOffset + 4);
		}

		memory.ObjectsRef<long long>(part->m_Ground.data(), wh_mul_8, 0x41 * 0x41);
	}
};