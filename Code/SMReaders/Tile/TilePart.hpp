#pragma once

#include <vector>
#include <assert.h>

#include "SMReaders/ByteImpl.hpp"
#include "SMReaders/Object/Asset.hpp"

class Tile;

class TilePart
{
	Tile* Parent;

public:
	// Mip
	std::vector<float> VertexHeight;
	std::vector<int> VertexColor;
	std::vector<long long> Ground;

	// Clutter
	std::vector<Byte> Clutter;

	//std::vector<std::vector<Harvestable*>> Harvestables; IMPLEMENT LATER
	std::vector<std::vector<Asset*>> Assets;

	//std::vector<Node*> Nodes; IMPLEMENT LATER
	//std::vector<Prefab*> Prefabs; IMPLEMENT LATER
	//std::vector<Blueprint*> Blueprints; IMPLEMENT LATER
	//std::vector<Decal*> Decals; IMPLEMENT LATER



public:
	TilePart(Tile* parent)
	{
		this->Parent = parent;

		VertexColor.resize(33 * 33);
		VertexHeight.resize(33 * 33);
		Ground.resize(65 * 65);
		Clutter.resize(128 * 128);

		Assets.resize(4);
		//Harvestables.reserve(4);
	}

	void SetVertexColor(const std::vector<int>& vert_array)
	{
		this->VertexColor = vert_array;
	}

	void SetVertexHeight(const std::vector<float>& height_array)
	{
		this->VertexHeight = height_array;
	}

	void SetGroundMaterials(const std::vector<long long>& material_array)
	{
		this->Ground = material_array;
	}

	void SetClutter(const std::vector<Byte>& clutter_array)
	{
		this->Clutter = clutter_array;
	}

	void AddAsset(Asset* asset, const int& index)
	{
		assert(asset != nullptr);
		assert(0 <= index && index <= 3);

		Assets[index].push_back(asset);
	}

	//void AddHarvestable(Harvestable* harvestable, const int& index)
	//{
	//	assert(harvestable == nullptr);
	//	assert(index < 0 || index > 3);

	//	Harvestables[index].push_back(harvestable);
	//}

	//void AddNode(Node* node)
	//{
	//	assert(node == nullptr);

	//	Nodes.push_back(node);
	//}

	//void AddPrefab(Prefab* prefab)
	//{
	//	assert(prefab == nullptr);

	//	Prefabs.push_back(prefab);
	//}

	//void AddBlueprint(Blueprint* blueprint)
	//{
	//	assert(blueprint == nullptr);

	//	Blueprints.push_back(blueprint);
	//}

	//void AddDecal(Decal* decal)
	//{
	//	assert(decal == nullptr);

	//	Decals.push_back(decal);
	//}

	Tile* GetParent()
	{
		return this->Parent;
	}
};