#pragma once

#include "Utils/ByteImpl.hpp"
#include "Tile/Object/Asset.hpp"
#include "Tile/Object/Harvestable.hpp"
#include "Tile/Object/Prefab.h"
#include "Tile/WriterOffset.hpp"

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

	std::vector<std::vector<Harvestable*>> Harvestables;
	std::vector<std::vector<Asset*>> Assets;
	std::vector<Prefab*> Prefabs;

	//std::vector<Node*> Nodes; IMPLEMENT LATER
	//std::vector<Blueprint*> Blueprints; IMPLEMENT LATER
	//std::vector<Decal*> Decals; IMPLEMENT LATER

public:
	TilePart(Tile* parent);

	void SetVertexColor(const std::vector<int>& vert_array);
	void SetVertexHeight(const std::vector<float>& height_array);
	void SetGroundMaterials(const std::vector<long long>& material_array);
	void SetClutter(const std::vector<Byte>& clutter_array);

	void AddAsset(Asset* asset, const int& index);
	void AddHarvestable(Harvestable* harvestable, const int& index);
	void AddPrefab(Prefab* prefab);

	Tile* GetParent();

	void WriteToFile(std::ofstream& model, WriterOffsetData& mOffsetData, const int& xPos, const int& zPos);
};