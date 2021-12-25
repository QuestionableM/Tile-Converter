#pragma once

#include "Utils/Memory.hpp"
#include "Utils/BitStream.hpp"
#include "Console.hpp"

class PrefabHeader
{
public:
	int blueprintCount;
	int hasBlueprints;
	
	int prefabCount;
	int hasPrefabs;
	
	int nodeCount;
	int hasNodes;
	
	int assetCount;
	int hasAssets;
	
	int decalsCount;
	int hasDecals;
	
	int count_0x54;
	int has_0x5c;
	
	int version;
	
	PrefabHeader(const int& version)
	{
		this->version = version;
	}
	
	void Read(BitStream& stream)
	{
		stream.Move(4 * 8);
		blueprintCount = stream.ReadInt();
		stream.Move(4 * 8);
		hasBlueprints = stream.ReadInt();

		stream.Move(4 * 8);
		prefabCount = stream.ReadInt();
		stream.Move(4 * 8);
		hasPrefabs = stream.ReadInt();

		stream.Move(4 * 8);
		nodeCount = stream.ReadInt();
		stream.Move(4 * 8);
		hasNodes = stream.ReadInt();

		if (2 < version) {
			stream.Move(4 * 8);
			assetCount = stream.ReadInt();
			stream.Move(4 * 8);
			hasAssets = stream.ReadInt();
		}

		if (3 < version) {
			stream.Move(4 * 8);
			decalsCount = stream.ReadInt();
			stream.Move(4 * 8);
			hasDecals = stream.ReadInt();
		}

		if (5 < version) {
			stream.Move(4 * 8);
			count_0x54 = stream.ReadInt();
			stream.Move(4 * 8);
			has_0x5c = stream.ReadInt();
		}

		DebugOutL("Prefab Data:");
		DebugOutL("Blueprint Count: ", blueprintCount);
		DebugOutL("Has Blueprints: ", hasBlueprints);
		DebugOutL("Prefab Count: ", prefabCount);
		DebugOutL("Has Prefabs: ", hasPrefabs);
		DebugOutL("Node Count: ", nodeCount);
		DebugOutL("Has Nodes: ", hasNodes);
		DebugOutL("Asset Count: ", assetCount);
		DebugOutL("Has Assets: ", hasAssets);
		DebugOutL("Decals Count: ", decalsCount);
		DebugOutL("Has Decals: ", hasDecals);
		DebugOutL("Count_0x54: ", count_0x54);
		DebugOutL("Has_0x5c: ", has_0x5c);
	}
};