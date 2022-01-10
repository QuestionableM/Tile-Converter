#pragma once

#include "ObjectDatabase/Mod/Mod.hpp"

#include "Tile/CellHeader.hpp"
#include "Tile/TilePart.hpp"
#include "Utils/Uuid.hpp"

#include "lz4/lz4.h"

class ClutterReader
{
	ClutterReader() = default;

public:
	static void Read(CellHeader* header, MemoryWrapper& memory, TilePart* part)
	{
		if (!ConvertSettings::ExportClutter) return;

		ClutterReader::Read(ClutterReader::Read(header, memory), part);
	}

#pragma warning(push)
#pragma warning(disable : 4996)

	static std::vector<Byte> Read(CellHeader* header, MemoryWrapper& memory)
	{
		DebugOutL("Clutter: ", header->clutterCompressedSize, " ", header->clutterSize);

		std::vector<Byte> compressed = memory.Objects<Byte>(header->clutterIndex, header->clutterCompressedSize);
		std::vector<Byte> bytes = {};
		bytes.resize(header->clutterSize);

		int debugSize = LZ4_decompress_fast((char*)compressed.data(), (char*)bytes.data(), header->clutterSize);
		assert(debugSize == header->clutterCompressedSize);

		return bytes;
	}

#pragma warning(pop)

	static void Read(const std::vector<Byte>& bytes, TilePart* part)
	{
		MemoryWrapper memory(bytes);

		Byte first_byte = memory.Object<Byte>(0);
		if (first_byte != 0)
		{
			std::size_t length = (std::size_t)((int)first_byte & 0xff);
			std::size_t offset = 2;

			for (std::size_t i = 0; i < length; i++)
			{
				int uVar7 = 0;

				for (std::size_t j = 0; j < 0x10; j++)
				{
					const int read = (int)memory.Object<char>(offset + j);

					const int a = (uVar7 ^ read);
					const int b = 0x9e3779b9;
					const int c = (uVar7 << 6);
					const int d = (uVar7 >> 2);

					uVar7 = a + b + c + d;
				}

				SMUuid clutter_uuid(memory.Objects<long long>(offset, 2));
				DebugOutL(ConCol::YELLOW_INT, "Clutter: ", clutter_uuid.ToString(), " -> ", uVar7);

				/*
				int iVar8 = 0;
				if(iVar8 != 1) {
					//Log("TerrainGrass", "false && \"Uuid dosen't exist in the clutter manager, defaulting to 0.\"", 149);
				} else {
					// FUN_009b8120(&local_64,local_3c,(byte *)((int)&bytes + 2));
				}
				*/

				offset += 0x11;
			}

			memory.Set(offset - 2);
		}


		std::vector<SignedByte> next_data = {};
		next_data.resize(128 * 128);

		std::size_t offset = memory.Index();

		part->ClutterMap.resize(128 * 128);
		for (std::size_t a = 0; a < 0x4000; a++)
		{
			const SignedByte cur_byte = memory.Object<SignedByte>(1 + a + offset);
			next_data[a] = cur_byte;

			if (cur_byte < 0) continue;

			ClutterData* clData = Mod::GetGlobalClutterById(cur_byte);
			if (!clData) continue;

			Model* pModel = ModelStorage::LoadModel(clData->Mesh);
			if (!pModel) continue;

			part->ClutterMap[a] = new TileClutter(clData, pModel);
		}

		part->SetClutter(next_data);
	}
};