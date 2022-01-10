#pragma once

#include "Tile/CellHeader.hpp"
#include "Utils/Memory.hpp"
#include "Tile/TilePart.hpp"
#include "Tile/Tile.hpp"
#include "ObjectDatabase/Mod/Mod.hpp"

#include "Tile/Object/Harvestable.hpp"

#include "lz4/lz4.h"

class HarvestableListReader
{
	HarvestableListReader() = default;

public:

#pragma warning(push)
#pragma warning(disable : 4996)

	static void Read(CellHeader* header, MemoryWrapper& reader, TilePart* part, ConvertError& cError)
	{
		if (cError || !ConvertSettings::ExportHarvestables) return;

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
				if (debugSize != harvestableListCompressedSize)
				{
					cError = ConvertError(1, L"HarvestableListReader::Read -> debugSize != harvestableListCompressedSize");
					return;
				}

				debugSize = HarvestableListReader::Read(bytes, a, header->harvestableListCount[a], part->GetParent()->GetVersion(), part);
				if (debugSize != header->harvestableListSize[a])
				{
					cError = ConvertError(1, L"HarvestableListReader::Read -> debugSize != header->harvestableListSize[a]");
					return;
				}
			}
		}
	}

#pragma warning(pop)

	static int Read(const std::vector<Byte>& bytes, const int& hvs_index, const int& len, const int& version, TilePart* part)
	{
		MemoryWrapper memory(bytes);

		int index = 0;
		for (int a = 0; a < len; a++)
		{
			std::vector<float> f_pos = memory.Objects<float>(index, 3);
			std::vector<float> f_quat = memory.Objects<float>(index + 0xc, 4);
			std::vector<float> f_size = memory.Objects<float>(index + 0x1c, 3);

			SMUuid uuid = memory.Objects<long long>(index + 0x28, 2);
			Color color = memory.Object<unsigned int>(index + 0x38);

			index += 0x3c;

			HarvestableData* hvs_data = Mod::GetGlobalHarvestbale(uuid);
			if (!hvs_data) continue;

			Model* hvs_model = ModelStorage::LoadModel(hvs_data->Mesh);
			if (!hvs_model) continue;

			Harvestable* nHvs = new Harvestable(hvs_data, hvs_model, color);
			nHvs->SetPosition({ f_pos[0], f_pos[1], f_pos[2] });
			nHvs->SetRotation({ f_quat[3], f_quat[0], f_quat[1], f_quat[2] });
			nHvs->SetSize({ f_size[0], f_size[1], f_size[2] });

			part->AddObject(nHvs, hvs_index);
		}

		return index;
	}
};