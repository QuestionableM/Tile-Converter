#pragma once

#include <vector>
#include "SMReaders/Tile/TilePart.hpp"
#include "SMReaders/ByteImpl.hpp"

class Tile
{
public:
	virtual int GetWidth() const   = 0;
	virtual int GetHeight() const  = 0;
	virtual int GetVersion() const = 0;

	virtual int GetTileType() const = 0;
	virtual long long GetCreatorId() const = 0;

	virtual void SetVersion(const int& version) = 0;
	virtual void SetTileType(const int& type) = 0;
	virtual void SetCreatorId(const long long& creator_id) = 0;

	virtual void Resize(const int& width, const int& height) = 0;

	virtual std::vector<float> GetVertexHeight() = 0;
	virtual std::vector<int> GetVertexColor() = 0;
	virtual std::vector<Byte> GetClutter() = 0;
	virtual std::vector<long long> GetGround() = 0;
	
	virtual TilePart* GetPart(const int& x, const int& y) = 0;
};