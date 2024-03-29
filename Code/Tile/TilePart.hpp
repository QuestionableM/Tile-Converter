#pragma once

#include "Tile\Object\TileClutter.hpp"
#include "Tile\Object\Harvestable.hpp"
#include "Tile\Object\Blueprint.hpp"
#include "Tile\Object\Prefab.hpp"
#include "Tile\Object\Asset.hpp"
#include "Tile\WriterOffset.hpp"

#include "Utils\ByteImpl.hpp"

class Tile;

class TilePart
{
	Tile* Parent;

public:
	// Mip
	std::array<float, 33 * 33> m_VertexHeight;
	std::array<int, 33 * 33> m_VertexColor;
	std::array<long long, 65 * 65> m_Ground;

	// Clutter
	std::array<SignedByte, 128 * 128> m_Clutter;
	std::array<TileClutter*, 128 * 128> m_ClutterMap;

	/*
		the 1st vector contains: harvestables, assets, prefabs and blueprints
		the 2nd-4th vectors contain: assets and harvestables
	*/
	std::array<std::vector<TileEntity*>, 4> m_Objects = {};

public:
	TilePart(Tile* parent);
	~TilePart();

	void AddObject(TileEntity* object, const int& index = 0);

	inline Tile* GetParent() { return this->Parent; }

	void WriteToFile(std::ofstream& model, WriterOffsetData& mOffsetData, const int& xPos, const int& zPos);
	void FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tData) const;
	std::size_t GetAmountOfObjects() const;
};