#pragma once

#include "Console.hpp"
#include "SMReaders/Data/Memory.hpp"
#include <vector>

#include "SMReaders/ByteImpl.hpp"

class CellHeader
{
public:
	/* 0x00 */ std::vector<int> mipIndex;
	/* 0x18 */ std::vector<int> mipCompressedSize;
	/* 0x30 */ std::vector<int> mipSize;

	/* 0x48 */ int clutterIndex;
	/* 0x4c */ int clutterCompressedSize;
	/* 0x50 */ int clutterSize;

	/* 0x54 */ std::vector<int> assetListCount;
	/* 0x64 */ std::vector<int> assetListIndex;
	/* 0x74 */ std::vector<int> assetListCompressedSize;
	/* 0x84 */ std::vector<int> assetListSize;

	/* 0x94 */ int blueprintListCount;
	/* 0x98 */ int blueprintListIndex;
	/* 0x9c */ int blueprintListCompressedSize;
	/* 0xa0 */ int blueprintListSize;

	/* 0xa4 */ int nodeCount;
	/* 0xa8 */ int nodeIndex;
	/* 0xac */ int nodeCompressedSize;
	/* 0xb0 */ int nodeSize;

	/* 0xb4 */ int scriptCount;
	/* 0xb8 */ int scriptIndex;
	/* 0xbc */ int scriptCompressedSize;
	/* 0xc0 */ int scriptSize;

	/* 0xc4 */ int prefabCount;
	/* 0xc8 */ int prefabIndex;
	/* 0xcc */ int prefabCompressedSize;
	/* 0xd0 */ int prefabSize;

	/* 0xd4 */ int decalCount;
	/* 0xd8 */ int decalIndex;
	/* 0xdC */ int decalCompressedSize;
	/* 0xe0 */ int decalSize;

	/* 0xe4 */ std::vector<int> harvestableListCount;
	/* 0xf4 */ std::vector<int> harvestableListIndex;
	/* 0x104 */std::vector<int> harvestableListCompressedSize;
	/* 0x114 */std::vector<int> harvestableListSize;

	std::vector<Byte> mBytes = {};

	void Read()
	{
		MemoryWrapper mMemory(mBytes.data());

		this->mipIndex = mMemory.Objects<int>(0x0, 6);
		this->mipCompressedSize = mMemory.Objects<int>(0x18, 6);
		this->mipSize = mMemory.Objects<int>(0x30, 6);

		this->clutterIndex = mMemory.Object<int>(0x48);
		this->clutterCompressedSize = mMemory.Object<int>(0x4c);
		this->clutterSize = mMemory.Object<int>(0x50);

		this->assetListCount = mMemory.Objects<int>(0x54, 4);
		this->assetListIndex = mMemory.Objects<int>(0x64, 4);
		this->assetListCompressedSize = mMemory.Objects<int>(0x74, 4);
		this->assetListSize = mMemory.Objects<int>(0x84, 4);

		this->blueprintListCount = mMemory.Object<int>(0x94);
		this->blueprintListIndex = mMemory.Object<int>(0x98);
		this->blueprintListCompressedSize = mMemory.Object<int>(0x9c);
		this->blueprintListSize = mMemory.Object<int>(0xa0);

		this->nodeCount = mMemory.Object<int>(0xa4);
		this->nodeIndex = mMemory.Object<int>(0xa8);
		this->nodeCompressedSize = mMemory.Object<int>(0xac);
		this->nodeSize = mMemory.Object<int>(0xb0);

		this->prefabCount = mMemory.Object<int>(0xc4);
		this->prefabIndex = mMemory.Object<int>(0xc8);
		this->prefabCompressedSize = mMemory.Object<int>(0xcc);
		this->prefabSize = mMemory.Object<int>(0xd0);

		this->decalCount = mMemory.Object<int>(0xd4);
		this->decalIndex = mMemory.Object<int>(0xd8);
		this->decalCompressedSize = mMemory.Object<int>(0xdc);
		this->decalSize = mMemory.Object<int>(0xe0);

		this->harvestableListCount = mMemory.Objects<int>(0xe4, 4);
		this->harvestableListIndex = mMemory.Objects<int>(0xf4, 4);
		this->harvestableListCompressedSize = mMemory.Objects<int>(0x104, 4);
		this->harvestableListSize = mMemory.Objects<int>(0x114, 4);

		DebugOutL("MipIndex: ", this->mipIndex);
		DebugOutL("MipCompressedSize: ", this->mipCompressedSize);
		DebugOutL("MipSize: ", this->mipSize);

		DebugOutL("Asset list count: ", this->assetListCount);
		DebugOutL("Asset list index: ", this->assetListIndex);
		DebugOutL("Asset list compressed size: ", this->assetListCompressedSize);
		DebugOutL("Asset list size: ", this->assetListSize);
	}

	CellHeader(const std::vector<Byte>& memory)
	{
		this->mBytes = memory;
	}

	std::vector<Byte> Data()
	{
		return this->mBytes;
	}
};