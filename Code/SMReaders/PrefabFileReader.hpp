#pragma once

#include "Tile/Object/Prefab.h"
#include "Tile/Object/Blueprint.hpp"
#include "SMReaders/PrefabHeader.hpp"

#include "Utils/File.hpp"
#include "Utils/Memory.hpp"
#include "ObjectDatabase/ObjectDatabase.hpp"
#include "ObjectDatabase/Mod/Mod.hpp"
#include "ObjectDatabase/KeywordReplacer.hpp"

#include <algorithm>

#include "Console.hpp"

class PrefabFileReader
{
	PrefabFileReader() = default;

public:
	static Prefab* Read(const std::wstring& path)
	{
		std::vector<Byte> bytes = File::ReadFileBytes(path);

		if (bytes.size() == 0)
		{
			DebugErrorL("Couldn't read the specified prefab file: ", path);
			return nullptr;
		}

		return PrefabFileReader::Read(bytes);
	}

	static Prefab* Read(const std::vector<Byte>& bytes)
	{
		MemoryWrapper reader(bytes);

		std::vector<char> magic = reader.NextObjects<char>(4, true);

		if (std::string(magic.begin(), magic.end()) != "PREF")
		{
			DebugErrorL("The specified prefab contains wrong magic value");
			return nullptr;
		}

		Prefab* prefab = new Prefab();
		
		const int version = reader.NextObject<int>(true);
		DebugOutL("Prefab Version: ", version);

		BitStream stream(reader);

		PrefabHeader pHeader(version);
		pHeader.Read(stream);

		if (pHeader.hasBlueprints != 0)
		{
			PrefabFileReader::ReadBlueprints(stream, prefab, pHeader.blueprintCount);
		}

		if (pHeader.hasPrefabs != 0)
		{
			PrefabFileReader::ReadPrefabs(stream, prefab, pHeader.prefabCount, version);
		}

		if (pHeader.hasNodes != 0)
		{
			PrefabFileReader::ReadNodes(stream, prefab, pHeader.nodeCount);
		}

		if (pHeader.hasAssets != 0)
		{
			PrefabFileReader::ReadAssets(stream, prefab, pHeader.assetCount);
		}

		if (pHeader.hasDecals != 0)
		{
			PrefabFileReader::ReadDecals(stream, prefab, pHeader.decalsCount);
		}

		if (pHeader.has_0x5c != 0)
		{
			PrefabFileReader::Read_248(stream, prefab, pHeader.count_0x54);
		}

		return prefab;
	}

	static void ReadBlueprints(BitStream& stream, Prefab* prefab, const int& count)
	{
		static const std::string bp_secret = "?JB:";

		for (int a = 0; a < count; a++)
		{
			int string_length = stream.ReadInt();
			const std::string value = stream.ReadString(string_length);

			std::vector<float> f_pos = { stream.ReadFloat(), stream.ReadFloat(), stream.ReadFloat() };
			std::vector<float> f_quat = { stream.ReadFloat(), stream.ReadFloat(), stream.ReadFloat(), stream.ReadFloat() };

			stream.ReadInt();

			const std::size_t bp_index = value.find(bp_secret);
			if (bp_index != std::string::npos)
			{
				const std::string bp_string = value.substr(bp_index + bp_secret.size());

				Blueprint* blueprint = Blueprint::FromJsonString(bp_string);
				if (!blueprint) continue;

				DebugOutL("Blueprint Position: ", f_pos);
				DebugOutL("Blueprint Rotation: ", f_quat);
				DebugOutL("Blueprint Value: ", bp_string);

				blueprint->SetPosition({ f_pos[0], f_pos[1], f_pos[2] });
				blueprint->SetRotation({ f_quat[3], f_quat[0], f_quat[1], f_quat[2] });
				blueprint->SetSize({ 0.25f, 0.25f, 0.25f });
				
				prefab->AddBlueprint(blueprint);
			}
			else
			{
				const std::wstring bp_wide_val = String::ToWide(value);
				const std::wstring bp_path = KeywordReplacer::ReplaceKey(bp_wide_val);

				Blueprint* blueprint = Blueprint::FromFile(bp_path);
				if (!blueprint) continue;

				DebugOutL("Blueprint Position: ", f_pos);
				DebugOutL("Blueprint Rotation: ", f_quat);
				DebugOutL("Blueprint Path: ", bp_path);

				blueprint->SetPosition({ f_pos[0], f_pos[1], f_pos[2] });
				blueprint->SetRotation({ f_quat[3], f_quat[0], f_quat[1], f_quat[2] });
				blueprint->SetSize({ 0.25f, 0.25f, 0.25f });

				prefab->AddBlueprint(blueprint);
			}
		}
	}

	static void ReadPrefabs(BitStream& stream, Prefab* prefab, const int& count, const int& version)
	{
		for (int a = 0; a < count; a++)
		{
			int string_length = stream.ReadInt();
			std::wstring pref_path = String::ToWide(stream.ReadString(string_length));

			DebugOutL("Prefab: ", pref_path);
			std::vector<float> f_pos = {};
			std::vector<float> f_quat = {};
			std::vector<float> f_size = { 1.0f, 1.0f, 1.0f };

			if (version < 5)
			{
				f_pos = { stream.ReadFloat(), stream.ReadFloat(), stream.ReadFloat() };
				f_quat = { stream.ReadFloat(), stream.ReadFloat(), stream.ReadFloat(), stream.ReadFloat() };
			}
			else
			{
				f_pos = { stream.ReadFloat(), stream.ReadFloat(), stream.ReadFloat() };
				f_quat = { stream.ReadFloat(), stream.ReadFloat(), stream.ReadFloat(), stream.ReadFloat() };
				f_size = { stream.ReadFloat(), stream.ReadFloat(), stream.ReadFloat() };
			}

			stream.ReadInt();
			stream.ReadInt();

			Prefab* rec_prefab = PrefabFileReader::Read(pref_path);
			if (!rec_prefab) continue;

			rec_prefab->SetPosition({ f_pos[0], f_pos[1], f_pos[2] });
			rec_prefab->SetRotation({ f_quat[3], f_quat[0], f_quat[1], f_quat[2] });
			rec_prefab->SetSize({ f_size[0], f_size[1], f_size[2] });
			rec_prefab->SetPath(pref_path);

			prefab->AddPrefab(rec_prefab);
		}
	}

	static void ReadNodes(BitStream& stream, Prefab* prefab, const int& count)
	{
		int uVar2 = stream.ReadByte();
		std::vector<std::string> tags = {};

		if (uVar2 != 0)
		{
			tags.reserve(uVar2);
			for (int a = 0; a < uVar2; a++)
			{
				int size = stream.ReadByte();
				tags.push_back(stream.ReadString(size));
			}
		}

		for (int a = 0; a < count; a++)
		{
			std::vector<float> f_pos = { stream.ReadFloat(), stream.ReadFloat(), stream.ReadFloat() };
			std::vector<float> f_quat = { stream.ReadFloat(), stream.ReadFloat(), stream.ReadFloat(), stream.ReadFloat() };
			std::vector<float> f_size = { stream.ReadFloat(), stream.ReadFloat(), stream.ReadFloat() };

			/*
			NodeImpl node = new NodeImpl(tags);
			node.setPosition(f_pos); //+ f_position
			node.setRotation(f_quat);
			node.setSize(f_size);
			*/

			int bVar2 = stream.ReadByte();
			if (bVar2 != 0)
			{
				for (int b = 0; b < bVar2; b++)
				{
					int idx = stream.ReadByte();
					std::string tag_name = tags[idx];
					
					//node.setTagState(tag_name, true);
				}
			}

			int uVar3 = stream.ReadInt();
			if (uVar3 != 0)
			{
				stream.ReadBytes(uVar3);
				//Object deserialized = LuaDeserializer.DeserializePure(blob);
			}

			//prefab.addNode(node);
		}
	}

	static void ReadAssets(BitStream& stream, Prefab* prefab, const int& count)
	{
		for (int a = 0; a < count; a++)
		{
			std::vector<float> f_pos = { stream.ReadFloat(), stream.ReadFloat(), stream.ReadFloat() };
			std::vector<float> f_quat = { stream.ReadFloat(), stream.ReadFloat(), stream.ReadFloat(), stream.ReadFloat() };
			std::vector<float> f_size = { stream.ReadFloat(), stream.ReadFloat(), stream.ReadFloat() };

			SMUuid uuid = stream.ReadUuid();
			int materialCount = stream.ReadByte();

			Asset* asset = new Asset();

			if (materialCount != 0)
			{
				for (int b = 0; b < materialCount; b++)
				{
					int length = stream.ReadByte();
					std::string str = stream.ReadString(length);
					asset->AddMaterial(String::ToWide(str), stream.ReadInt());
				}
			}

			AssetData* mData = Mod::GetGlobalAsset(uuid);
			if (mData != nullptr)
			{
				asset->pModel = ModelStorage::LoadModel(mData->Mesh, true, true);
				if (asset->pModel != nullptr)
				{
					asset->SetPosition({ f_pos[0], f_pos[1], f_pos[2] });
					asset->SetRotation({ f_quat[3], f_quat[0], f_quat[1], f_quat[2] });
					asset->SetSize({ f_size[0], f_size[1], f_size[2] });
					asset->SetUuid(uuid);
					asset->pParent = mData;

					prefab->AddAsset(asset);

					continue;
				}
			}

			delete asset;
		}
	}

	static void ReadDecals(BitStream& stream, Prefab* prefab, const int& count)
	{
		DebugOutL("ReadDecals - UNIMPLEMENTED");
	}

	static void Read_248(BitStream& stream, Prefab* prefab, const int& count)
	{
		DebugOutL("Read_248 - UNIMPLEMENTED");
	}
};