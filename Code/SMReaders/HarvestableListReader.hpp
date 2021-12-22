#pragma once

#include "Tile/CellHeader.hpp"
#include "Utils/Memory.hpp"
#include "Tile/TilePart.hpp"
#include "Tile/Tile.hpp"

#include "lz4/lz4.h"

class HarvestableListReader
{
public:
	static void Read(CellHeader* header, MemoryWrapper& reader, TilePart* part)
	{
		for (int a = 0; a < 4; a++)
		{
			const int harvestableListCompressedSize = header->harvestableListCompressedSize[a];
			const int harvestableListSize = header->harvestableListSize[a];

			if (header->harvestableListCount[a] != 0)
			{
				DebugOutL("Harvestable[", a, "]: ", harvestableListSize, ", ", harvestableListCompressedSize);

				std::vector<Byte> compressed = reader.Objects<Byte>(header->harvestableListIndex[a], harvestableListCompressedSize);

				std::vector<Byte> bytes = {};
				bytes.resize(harvestableListSize);

				int debugSize = LZ4_decompress_fast((char*)compressed.data(), (char*)bytes.data(), header->harvestableListSize[a]);
				assert(debugSize == harvestableListCompressedSize);

				debugSize = HarvestableListReader::Read(bytes, a, header->harvestableListCount[a], part->GetParent()->GetVersion(), part);
				assert(debugSize == header->harvestableListSize[a]);
			}
		}
	}

	static int Read(const std::vector<Byte>& bytes, const int& hvs_index, const int& len, const int& version, TilePart* part)
	{
		MemoryWrapper memory((Byte*)bytes.data());

		int index = 0;
		for (int a = 0; a < len; a++)
		{
			std::vector<float> f_pos = memory.Objects<float>(index, 3);
			std::vector<float> f_quat = memory.Objects<float>(index + 0xc, 4);
			std::vector<float> f_size = memory.Objects<float>(index + 0x1c, 3);

			SMUuid uuid = memory.Objects<long long>(index + 0x28, 2);
			unsigned int color = memory.Object<unsigned int>(index + 0x38);

			index += 0x3c;

			DebugOutL("HvsUuid: ", uuid.ToString());
			DebugOutL("HvsColor: ", color);
		}

		return index;
	}
	/*
	public int read(byte[] bytes, int harvestable_index, int len, int version, TilePart part) {
		Memory memory = new Memory(bytes);
		
		int index = 0;
		for(int i = 0; i < len; i++) {
			float[] f_pos = memory.Floats(3, index);
			float[] f_quat = memory.Floats(4, index + 0xc);
			float[] f_size = memory.Floats(3, index + 0x1c);
			UUID uuid = memory.Uuid(index + 0x28, true);
			int color = memory.Int(index + 0x38, true);
			index += 0x3c;
			
			HarvestableImpl harvestable = new HarvestableImpl();
			harvestable.setPosition(f_pos);
			harvestable.setRotation(f_quat);
			harvestable.setSize(f_size);
			harvestable.setUuid(uuid);
			
			// Convert to RGBA
			harvestable.setColor(color);
			
			part.addHarvestable(harvestable, harvestable_index);
		}
		
		return index;
	}
	*/
};