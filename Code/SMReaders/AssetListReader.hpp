#pragma once

#include "Tile/CellHeader.hpp"
#include "Tile/Tile.hpp"
#include "Tile/Object/Asset.hpp"
#include "ObjectDatabase/ObjectDatabase.hpp"
#include "ObjectDatabase/Mod/Mod.hpp"

#include "Utils/String.hpp"

#include "lz4/lz4.h"

class AssetListReader
{
public:

#pragma warning(push)
#pragma warning(disable : 4996)

	static void Read(CellHeader* header, MemoryWrapper& reader, TilePart* part)
	{
		for (int a = 0; a < 4; a++)
		{
			const int assetListCompressedSize = header->assetListCompressedSize[a];
			const int assetListSize = header->assetListSize[a];

			if (header->assetListCount[a] != 0)
			{
				DebugOutL("Asset[", a, "]: ", header->assetListCount[a]);
				const int assetListIndex = header->assetListIndex[a];

				std::vector<Byte> compressed = reader.Objects<Byte>(assetListIndex, assetListCompressedSize);

				std::vector<Byte> bytes = {};
				bytes.resize(assetListSize);

				int debugSize = LZ4_decompress_fast((char*)compressed.data(), (char*)bytes.data(), assetListSize);
				assert(debugSize == assetListCompressedSize);

				debugSize = AssetListReader::Read(bytes, a, header->assetListCount[a], part->GetParent()->GetVersion(), part);
				assert(debugSize == assetListSize);
			}
		}
	}

#pragma warning(pop)

	static int Read(const std::vector<Byte>& bytes, const int& asset_idx, const int& len, const int& version, TilePart* part)
	{
		MemoryWrapper memory(bytes);

		int index = 0;
		for (int a = 0; a < len; a++)
		{
			std::vector<float> f_pos = memory.Objects<float>(index, 3);
			std::vector<float> f_quat = memory.Objects<float>(index + 0xc, 4);
			std::vector<float> f_size = {};
			f_size.resize(3);

			if (version < 5)
			{
				float dim = memory.Object<float>(index + 0x1c);
				f_size = { dim, dim, dim };

				index += 0x20;
			}
			else
			{
				f_size = memory.Objects<float>(index + 0x1c, 3);

				index += 0x28;
			}

			SMUuid uuid;

			if (version < 4)
			{
				// int bVar4 = memory.Byte(index++);
				// String str = memory.String(bVar4, index);
				// System.out.printf("  ReadString: [%s]\n", str);

				// TODO: ????
			}
			else
			{
				uuid = memory.Objects<long long>(index, 2);
				index += 0x10;
			}

			std::unordered_map<std::wstring, Color> color_map;

			int bVar4 = (int)memory.Object<Byte>(index++) & 0xff;
			if (bVar4 != 0)
			{
				int length = bVar4;

				for (int j = 0; j < length; j++)
				{
					bVar4 = (int)memory.Object<Byte>(index++) & 0xff;
					std::vector<char> str_vec = memory.Objects<char>(index, bVar4);

					const std::wstring wstr_data = String::ToWide(std::string(str_vec.begin(), str_vec.end()));
					index += bVar4;
					const unsigned int color = memory.Object<unsigned int>(index);
					index += 4;

					if (color_map.find(wstr_data) == color_map.end())
						color_map.insert(std::make_pair(wstr_data, color));
				}
			}

			AssetData* asset_data = Mod::GetGlobalAsset(uuid);
			if (!asset_data) continue;

			Model* pModel = ModelStorage::LoadModel(asset_data->Mesh, true, true);
			if (!pModel) continue;

			Asset* nAsset = new Asset(asset_data, pModel, color_map);
			nAsset->SetPosition({ f_pos[0], f_pos[1], f_pos[2] });
			nAsset->SetRotation({ f_quat[3], f_quat[0], f_quat[1], f_quat[2] });
			nAsset->SetSize({ f_size[0], f_size[1], f_size[2] });

			part->AddObject(nAsset, asset_idx);
		}

		return index;
	}
};