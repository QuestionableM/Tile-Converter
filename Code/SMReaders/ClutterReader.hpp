#pragma once

#include "ObjectDatabase/Mod/Mod.hpp"

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

		Byte first_byte = memory.NextObject<Byte>();
		if (first_byte != 0)
		{
			std::size_t length = (std::size_t)((int)first_byte & 0xff);
			std::size_t offset = 2;

			part->ClutterData.resize(length);
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

				//ClutterData* pClutterData = Mod::GetGlobalClutter(clutter_uuid);
				//if (!pClutterData)
				//{
				//	DebugErrorL("Couldn't find clutter with the specified uuid: ", clutter_uuid.ToString());
				//}

				//part->ClutterData.push_back(pClutterData);

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

			memory.Set(offset);
		}

		std::vector<SignedByte> next_data = {};
		next_data.resize(128 * 128);

		std::size_t offset = memory.Index() - 2;

		for (std::size_t a = 0; a < 0x4000; a++)
			next_data[a] = memory.Object<SignedByte>(1 + a + offset);

		const std::size_t clutter_size = part->ClutterData.size();

		part->ClutterMap.resize(128 * 128);
		for (std::size_t a = 0; a < 0x4000; a++)
		{
			const SignedByte& cur_byte = next_data[a];
			if (cur_byte < 0) continue;

			ClutterData* clData = Mod::GetGlobalClutterById(cur_byte);
			if (clData != nullptr)
			{
				Model* pModel = ModelStorage::LoadModel(clData->Mesh, true, true);
				if (pModel != nullptr)
				{
					TileClutter* new_clutter = new TileClutter();
					new_clutter->pParent = clData;
					new_clutter->pModel = pModel;
					new_clutter->SetUuid(clData->Uuid);

					part->ClutterMap[a] = new_clutter;
					continue;
				}
			}

			part->ClutterMap[a] = nullptr;
		}

		part->SetClutter(next_data);
	}
};