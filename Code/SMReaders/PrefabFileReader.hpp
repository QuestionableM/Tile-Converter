#pragma once

#include "ObjectDatabase\KeywordReplacer.hpp"
#include "ObjectDatabase\ObjectDatabase.hpp"
#include "ObjectDatabase\Mods\Mod.hpp"

#include "Tile\Object\Blueprint.hpp"
#include "Tile\Object\Prefab.hpp"
#include "Tile\Object\Decal.hpp"
#include "Tile\ConvertSettings.hpp"

#include "Utils\Memory.hpp"
#include "Utils\File.hpp"

#include "SMReaders\PrefabHeader.hpp"
#include "Console.hpp"

#include <algorithm>


class PrefabFileReader
{
	PrefabFileReader() = default;

public:
	static Prefab* Read(const std::wstring& path, const std::wstring& flag)
	{
		std::vector<Byte> bytes = File::ReadFileBytes(path);

		if (bytes.size() == 0)
		{
			DebugErrorL("Couldn't read the specified prefab file: ", path);
			return nullptr;
		}

		return PrefabFileReader::Read(bytes, path, flag);
	}

	static Prefab* Read(const std::vector<Byte>& bytes, const std::wstring& ppath, const std::wstring& pflag)
	{
		MemoryWrapper reader(bytes);

		const std::vector<char> magic = reader.NextObjects<char, true>(4);

		if (std::string(magic.begin(), magic.end()) != "PREF")
		{
			DebugErrorL("The specified prefab contains wrong magic value");
			return nullptr;
		}

		Prefab* prefab = new Prefab(ppath, pflag);
		
		const int version = reader.NextObject<int, true>();
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
			PrefabFileReader::ReadDecals(stream, prefab, pHeader.decalsCount, version);
		}

		if (pHeader.has_0x5c != 0)
		{
			PrefabFileReader::Read_248(stream, prefab, pHeader.count_0x54);
		}

		if (pHeader.has_0x6c != 0)
		{
			PrefabFileReader::Read_1(stream, prefab, pHeader.count_0x64);
		}

		if (pHeader.has_0x7c != 0)
		{
			PrefabFileReader::Read_2(stream, prefab, pHeader.count_0x74);
		}

		return prefab;
	}

	static void ReadBlueprints(BitStream& stream, Prefab* prefab, const int& count)
	{
		for (int a = 0; a < count; a++)
		{
			const int string_length = stream.ReadInt();
			const std::string value = stream.ReadString(string_length);

			const glm::vec3 f_pos = stream.ReadVec3();
			const glm::quat f_quat = stream.ReadQuat();

			stream.ReadInt();

			if (ConvertSettings::ExportBlueprints)
			{
				Blueprint* blueprint = Blueprint::LoadAutomatic(value);
				if (!blueprint) continue;

				blueprint->SetPosition(f_pos);
				blueprint->SetRotation(f_quat);

				prefab->AddObject(blueprint);
			}
		}
	}

	static void ReadPrefabs(BitStream& stream, Prefab* prefab, const int& count, const int& version)
	{
		for (int a = 0; a < count; a++)
		{
			const int l_StringLength = stream.ReadInt();
			const std::wstring l_PrefLocalPath = String::ToWide(stream.ReadString(l_StringLength));
			const std::wstring l_PrefFullPath = KeywordReplacer::ReplaceKey(l_PrefLocalPath);
			DebugOutL(0b1011_fg, "Recursive Prefab Path: ", l_PrefFullPath);

			const glm::vec3 f_pos = stream.ReadVec3();
			const glm::quat f_quat = stream.ReadQuat();
			glm::vec3 f_size;

			if (version < 5)
			{
				f_size = glm::vec3(1.0f);
			}
			else
			{
				f_size = stream.ReadVec3();
			}

			stream.ReadInt();
			stream.ReadInt();

			if (ConvertSettings::ExportPrefabs)
			{
				Prefab* rec_prefab = PrefabFileReader::Read(l_PrefFullPath, L"");
				if (!rec_prefab) continue;

				rec_prefab->SetPosition(f_pos);
				rec_prefab->SetRotation(f_quat);
				rec_prefab->SetSize(f_size);

				prefab->AddObject(rec_prefab);
			}
		}
	}

	static void ReadNodes(BitStream& stream, Prefab* prefab, const int& count)
	{
		const int uVar2 = stream.ReadByte();
		std::vector<std::string> tags = {};

		if (uVar2 != 0)
		{
			tags.reserve(uVar2);
			for (int a = 0; a < uVar2; a++)
			{
				const int size = stream.ReadByte();
				tags.push_back(stream.ReadString(size));
			}
		}

		for (int a = 0; a < count; a++)
		{
			const glm::vec3 f_pos = stream.ReadVec3();
			const glm::quat f_quat = stream.ReadQuat();
			const glm::vec3 f_size = stream.ReadVec3();

			/*
			NodeImpl node = new NodeImpl(tags);
			node.setPosition(f_pos); //+ f_position
			node.setRotation(f_quat);
			node.setSize(f_size);
			*/

			const int bVar2 = stream.ReadByte();
			if (bVar2 != 0)
			{
				for (int b = 0; b < bVar2; b++)
				{
					const int idx = stream.ReadByte();
					std::string tag_name = tags[idx];
					
					//node.setTagState(tag_name, true);
				}
			}

			const int uVar3 = stream.ReadInt();
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
			const glm::vec3 f_pos = stream.ReadVec3();
			const glm::quat f_quat = stream.ReadQuat();
			const glm::vec3 f_size = stream.ReadVec3();

			const SMUuid uuid = stream.ReadUuid();
			const int materialCount = stream.ReadByte();

			std::unordered_map<std::wstring, Color> color_map;

			if (materialCount != 0)
			{
				for (int b = 0; b < materialCount; b++)
				{
					const int length = stream.ReadByte();
					const std::string str = stream.ReadString(length);

					const std::wstring wstr = String::ToWide(str);
					const unsigned int color = stream.ReadInt();

					if (color_map.find(wstr) == color_map.end())
						color_map.insert(std::make_pair(wstr, color));
				}
			}

			if (ConvertSettings::ExportAssets)
			{
				AssetData* asset_data = Mod::GetGlobalAsset(uuid);
				if (!asset_data) continue;

				Model* pModel = ModelStorage::LoadModel(asset_data->Mesh);
				if (!pModel) continue;

				Asset* nAsset = new Asset(asset_data, pModel, color_map);
				nAsset->SetPosition(f_pos);
				nAsset->SetRotation(f_quat);
				nAsset->SetSize(f_size);

				prefab->AddObject(nAsset);
			}
		}
	}

	static void ReadDecals(BitStream& stream, Prefab* prefab, const int& count, const int& version)
	{
		DebugOutL("Reading ", count, " decals...");

		for (int a = 0; a < count; a++)
		{
			const glm::vec3 v_pos = stream.ReadVec3();
			const glm::quat v_quat = stream.ReadQuat();
			const glm::vec3 v_size = stream.ReadVec3();

			const SMUuid v_uuid = stream.ReadUuid();
			const Color v_color = stream.ReadInt();

			//Read a random 4 byte value
			stream.ReadInt();

			if (ConvertSettings::ExportDecals)
			{
				const DecalData* v_decalData = Mod::GetGlobalDecal(v_uuid);
				if (!v_decalData) continue;

				Decal* v_newDecal = new Decal(v_decalData, v_color);
				v_newDecal->SetPosition(v_pos);
				v_newDecal->SetRotation(v_quat);
				v_newDecal->SetSize(v_size);

				prefab->AddObject(v_newDecal);
			}
		}
	}

	static void Read_248(BitStream& stream, Prefab* prefab, const int& count)
	{
		DebugWarningL("UNIMPLEMENTED -> ", stream.Index());
	}

	static void Read_1(BitStream& stream, Prefab* prefab, const int& count)
	{
		DebugWarningL("UNIMPLEMENTED -> ", stream.Index());
	}

	static void Read_2(BitStream& stream, Prefab* prefab, const int& count)
	{
		DebugWarningL("UNIMPLEMENTED -> ", stream.Index());
	}
};