#pragma once

#include "Utils/ByteImpl.hpp"
#include "Tile/Object/Asset.hpp"
#include "Tile/Object/Harvestable.hpp"
#include "Tile/Object/Prefab.h"
#include "Tile/Object/Blueprint.hpp"
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

	/*
		the 1st vector contains: harvestables, assets, prefabs and blueprints
		the 2nd-4th vectors contain: assets and harvestables
	*/
	std::array<std::vector<TileEntity*>, 4> Objects = {};

public:
	TilePart(Tile* parent);

	void SetVertexColor(const std::vector<int>& vert_array);
	void SetVertexHeight(const std::vector<float>& height_array);
	void SetGroundMaterials(const std::vector<long long>& material_array);
	void SetClutter(const std::vector<Byte>& clutter_array);

	void AddObject(TileEntity* object, const int& index = 0);

	Tile* GetParent();

	void WriteToFile(std::ofstream& model, WriterOffsetData& mOffsetData, const int& xPos, const int& zPos);
};