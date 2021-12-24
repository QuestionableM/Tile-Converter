#pragma once

#include <vector>

#include "Tile/TilePart.hpp"

class Tile
{
	int Version;
	int Type;
	long long CreatorId;
	int Width;
	int Height;
	std::vector<TilePart*> Tiles;

public:
	Tile(const int& width, const int& height);

	int GetWidth() const;
	int GetHeight() const;
	int GetVersion() const;
	//UUID IMPLEMENTATION GOES HERE
	int GetTileType() const;
	long long GetCreatorId() const;

	void SetVersion(const int& version);
	void SetTileType(const int& type);
	void SetCreatorId(const long long& creator_id);
	void SetPart(const int& x, const int& y, TilePart* part);

	void Resize(const int& width, const int& height);

	std::vector<float> GetVertexHeight() const;
	std::vector<int> GetVertexColor() const;
	std::vector<Byte> GetClutter() const;
	std::vector<long long> GetGround() const;
	TilePart* GetPart(const int& x, const int& y) const;

	void WriteToFile(const std::wstring& path);
};