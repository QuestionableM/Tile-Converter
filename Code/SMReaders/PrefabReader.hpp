#pragma once

#include "Tile/CellHeader.hpp"
#include "Tile/TilePart.hpp"
#include "Tile/Tile.hpp"
#include "Tile/Object/Prefab.h"

#include "Utils/Memory.hpp"

#include "SMReaders/PrefabFileReader.hpp"
#include "ObjectDatabase/KeywordReplacer.hpp"

#include <lz4/lz4.h>

class PrefabReader
{
	PrefabReader() = default;

public:

#pragma warning(push)
#pragma warning(disable : 4996)

	static void Read(CellHeader* header, MemoryWrapper& reader, TilePart* part, ConvertError& cError)
	{
		if (cError || !ConvertSettings::ExportPrefabs) return;
		if (header->prefabCount == 0 || header->prefabIndex == 0) return;

		DebugOutL("Prefab: ", header->prefabSize, " / ", header->prefabCompressedSize);

		std::vector<Byte> compressed = reader.Objects<Byte>(header->prefabIndex, header->prefabCompressedSize);
		
		std::vector<Byte> bytes = {};
		bytes.resize(header->prefabSize);

		int debugSize = LZ4_decompress_fast((char*)compressed.data(), (char*)bytes.data(), header->prefabSize);
		if (debugSize != header->prefabCompressedSize)
		{
			cError = ConvertError(1, L"PrefabReader::Read -> debugSize != header->prefabCompressedSize");
			return;
		}

		debugSize = PrefabReader::Read(bytes, header->prefabCount, part);
		if (debugSize != header->prefabSize)
		{
			cError = ConvertError(1, L"PrefabReader::Read -> debugSize != header->prefabSize");
			return;
		}
	}

#pragma warning(pop)

	static int Read(const std::vector<Byte>& bytes, const int& prefabCount, TilePart* part)
	{
		int index = 0;
		MemoryWrapper memory(bytes);
		const int version = part->GetParent()->GetVersion();

		for (int a = 0; a < prefabCount; a++)
		{
			const glm::vec3 f_pos = memory.Object<glm::vec3>(index);
			const std::array<float, 4> f_quat = memory.ObjectsConst<float, 4>(index + 0xc);
			
			glm::vec3 f_size;

			if (version < 9)
			{
				f_size = glm::vec3(1.0f);
				index += 0x1c;
			}
			else
			{
				f_size = memory.Object<glm::vec3>(index + 0x1c);
				index += 0x28;
			}

			const int string_length = memory.Object<int>(index);
			index += 4;
			const std::vector<char> path = memory.Objects<char>(index, string_length);
			index += string_length;

			const int bVar2 = (int)memory.Object<Byte>(index) & 0xff;
			index += 1;
			const std::vector<char> flag = memory.Objects<char>(index, bVar2);
			index += bVar2;
			index += 4;

			const std::wstring wide_path = String::ToWide(std::string(path.begin(), path.end()));
			const std::wstring pref_path = KeywordReplacer::ReplaceKey(wide_path);
			const std::wstring pref_flag = String::ToWide(std::string(flag.begin(), flag.end()));
			DebugOutL("Prefab Path: ", pref_path);

			Prefab* new_prefab = PrefabFileReader::Read(pref_path, pref_flag);
			if (!new_prefab) continue;

			new_prefab->SetPosition(f_pos);
			new_prefab->SetRotation({ f_quat[3], f_quat[0], f_quat[1], f_quat[2] });
			new_prefab->SetSize(f_size);

			part->AddObject(new_prefab);
		}

		return index;
	}
};