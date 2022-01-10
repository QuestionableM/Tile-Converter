#pragma once

#include "Tile/CellHeader.hpp"
#include "Tile/TilePart.hpp"
#include "Tile/Tile.hpp"
#include "Utils/Memory.hpp"

#include <lz4/lz4.h>

class BlueprintListReader
{
	BlueprintListReader() = default;
public:

#pragma warning(push)
#pragma warning(disable : 4996)

	static void Read(CellHeader* header, MemoryWrapper& reader, TilePart* part)
	{
		if (!ConvertSettings::ExportBlueprints) return;
		if (header->blueprintListCount == 0 || header->blueprintListIndex == 0) return;

		DebugOutL("BlueprintList: ", header->blueprintListSize, " / ", header->blueprintListCompressedSize);

		std::vector<Byte> compressed = reader.Objects<Byte>(header->blueprintListIndex, header->blueprintListCompressedSize);

		std::vector<Byte> bytes;
		bytes.resize(header->blueprintListSize);

		int debugSize = LZ4_decompress_fast((char*)compressed.data(), (char*)bytes.data(), header->blueprintListSize);
		assert(debugSize == header->blueprintListCompressedSize);

		debugSize = BlueprintListReader::Read(bytes, header->blueprintListCount, part);
		assert(debugSize == header->blueprintListSize);
	}

#pragma warning(pop)

	static int Read(const std::vector<Byte>& bytes, const int& count, TilePart* part)
	{
		const int version = part->GetParent()->GetVersion();
		MemoryWrapper memory(bytes);

		int index = 0;
		for (int a = 0; a < count; a++)
		{
			std::vector<float> f_pos = memory.Objects<float>(index, 3);
			std::vector<float> f_quat = memory.Objects<float>(index + 0xc, 4);
			index += 0x1c;

			std::string value;
			if (version < 7)
			{
				const int val_len = (int)memory.Object<Byte>(index) & 0xff;
				index++;

				std::vector<char> value_data = memory.Objects<char>(index, val_len);
				index += val_len;

				value = std::string(value_data.begin(), value_data.end());
			}
			else
			{
				const int val_len = memory.Object<int>(index);
				index += 4;

				std::vector<char> value_data = memory.Objects<char>(index, val_len);
				index += val_len;

				value = std::string(value_data.begin(), value_data.end());
			}

			Blueprint* new_blueprint = Blueprint::LoadAutomatic(value);
			if (!new_blueprint) continue;

			new_blueprint->SetPosition({ f_pos[0], f_pos[1], f_pos[2] });
			new_blueprint->SetRotation({ f_quat[3], f_quat[0], f_quat[1], f_quat[2] });

			part->AddObject(new_blueprint);
		}

		return index;
	}
};