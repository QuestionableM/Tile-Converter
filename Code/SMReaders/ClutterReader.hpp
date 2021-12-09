#pragma once

#include "Tile/CellHeader.hpp"
#include "Tile/TilePart.hpp"
#include "Utils/Uuid.hpp"

#include "lz4/lz4.h"

class ClutterReader
{
public:
	static void Read(CellHeader* header, MemoryWrapper& memory, TilePart* part)
	{
		ClutterReader::Read(ClutterReader::Read(header, memory), part);
	}

	static std::vector<Byte> Read(CellHeader* header, MemoryWrapper& memory)
	{
		DebugOutL("Clutter: ", header->clutterCompressedSize, " ", header->clutterSize);

		std::vector<Byte> compressed = memory.Objects<Byte>(header->clutterIndex, header->clutterCompressedSize);
		std::vector<Byte> bytes = {};
		bytes.resize(header->clutterSize);

		int debugSize = LZ4_decompress_fast((char*)compressed.data(), (char*)bytes.data(), header->clutterSize);
		DebugOutL("DebugSize: ", debugSize, ", CompressedSize: ", header->clutterCompressedSize);
		assert(debugSize == header->clutterCompressedSize);

		return bytes;
	}

	static void Read(const std::vector<Byte>& bytes, TilePart* part)
	{
		MemoryWrapper memory((Byte*)bytes.data());

		Byte first_byte = memory.NextObject<Byte>();
		if (first_byte != 0)
		{
			int length = (int)first_byte & 0xff;
			DebugOutL("Clutter Length: ", length);
			int offset = 2;

			for (int i = 0; i < length; i++)
			{
				int uVar7 = 0;
				for (int j = 0; j < 0x10; j++)
				{
					int read = (int)memory.Object<Byte>(offset + j);

					int a = (uVar7 ^ read);
					int b = 0x9e3779b9;
					int c = (uVar7 >> 6);
					int d = (uVar7 >> 2);

					uVar7 = a + b + c + d;
				}

				SMUuid clutter_uuid(memory.Objects<long long>(offset, 2));
				DebugOutL("Clutter Uuid: ", clutter_uuid.ToString());

				int iVar8 = 0;
				/*
				if(iVar8 != 1) {
					//Log("TerrainGrass", "false && \"Uuid dosen't exist in the clutter manager, defaulting to 0.\"", 149);
				} else {
					// FUN_009b8120(&local_64,local_3c,(byte *)((int)&bytes + 2));
				}
				*/

				offset += 0x11;
			}

			memory.Set(offset);
		}

		std::vector<Byte> next_data = {};
		next_data.resize(128 * 128);

		{
			std::size_t offset = memory.Index();

			for (int a = 0; a < 0x4000; a++)
			{
				Byte read = memory.Object<Byte>(1 + a + offset);
				next_data[a] = read;
			}

			part->SetClutter(next_data);
		}
	}
};