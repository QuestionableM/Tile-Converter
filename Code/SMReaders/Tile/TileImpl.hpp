#pragma once

#include "SMReaders/Tile/Tile.hpp"
#include <assert.h>

class TileImpl : public Tile
{
	int Version;
	int Type;
	long long CreatorId;
	int Width;
	int Height;
	std::vector<TilePart*> Tiles;
public:

	TileImpl(const int& width, const int& height)
	{
		this->Width = width;
		this->Height = height;
		this->Tiles.resize((std::size_t)(this->Width * this->Height));

		for (std::size_t a = 0; a < this->Tiles.size(); a++)
			this->Tiles[a] = new TilePart(this);
	}

	int GetWidth() const override
	{
		return this->Width;
	}

	int GetHeight() const override
	{
		return this->Height;
	}

	int GetVersion() const override
	{
		return this->Version;
	}

	//UUID IMPLEMENTATION GOES HERE
	
	int GetTileType() const override
	{
		return this->Type;
	}

	long long GetCreatorId() const override
	{
		return this->CreatorId;
	}



	void SetVersion(const int& version) override
	{
		this->Version = version;
	}

	void SetTileType(const int& type) override
	{
		this->Type = type;
	}

	void SetCreatorId(const long long& creator_id) override
	{
		this->CreatorId = creator_id;
	}



	void Resize(const int& width, const int& height)
	{
		assert(width < 1 || height < 1);

		std::vector<TilePart*> nTileArray = {};
		nTileArray.resize((std::size_t)(width * height));

		int min_height = std::min(height, this->Height);
		int min_width = std::min(width, this->Width);

		for (int y = 0; y < min_height; y++)
		{
			for (int x = 0; x < min_width; x++)
			{
				nTileArray[(std::size_t)(x + y * width)] = this->GetPart(x, y);
			}
		}

		for (std::size_t a = 0; a < nTileArray.size(); a++)
		{
			if (nTileArray[a] == nullptr)
				nTileArray[a] = new TilePart(this);
		}

		this->Width = width;
		this->Height = height;
		this->Tiles = nTileArray;
	}



	std::vector<float> GetVertexHeight() override
	{
		const int w = this->Width * 32 + 1;
		const int h = this->Height * 32 + 1;

		std::vector<float> float_array = {};
		float_array.resize((std::size_t)(w * h));

		for (int y = 0; y < this->Height; y++)
		{
			for (int x = 0; x < this->Width; x++)
			{
				TilePart* part = this->GetPart(x, y);

				const std::vector<float>& pVertHeights = part->VertexHeight;
				int idx = x * 32 + y * 32 * h;

				for (int a = 0; a < 32; a++)
				{
					std::memcpy(float_array.data() + (idx + a * w), pVertHeights.data() + (a * 33), 33 * sizeof(float));
				}
			}
		}

		return float_array;
	}

	std::vector<int> GetVertexColor() override
	{
		const int w = this->Width * 32 + 1;
		const int h = this->Height * 32 + 1;

		std::vector<int> vert_colors = {};
		vert_colors.resize((std::size_t)(w * h));

		for (int y = 0; y < this->Height; y++)
		{
			for (int x = 0; x < this->Width; x++)
			{
				TilePart* part = this->GetPart(x, y);

				const std::vector<int>& pVertColor = part->VertexColor;
				int idx = x * 32 + y * 32 * h;

				for (int a = 0; a < 32; a++)
				{
					std::memcpy(vert_colors.data() + (idx + a * w), pVertColor.data() + (a * 33), 33 * sizeof(int));
				}
			}
		}

		return vert_colors;
	}

	std::vector<Byte> GetClutter() override
	{
		const int w = this->Width * 128;
		const int h = this->Height * 128;

		std::vector<Byte> clutter_bytes = {};
		clutter_bytes.resize((std::size_t)(w * h));

		for (int y = 0; y < this->Height; y++)
		{
			for (int x = 0; x < this->Width; x++)
			{
				TilePart* part = this->GetPart(x, y);

				const std::vector<Byte>& pClutterArray = part->Clutter;
				int idx = x * 128 + y * 128 * h;

				for (int a = 0; a < 128; a++)
				{
					std::memcpy(clutter_bytes.data() + (idx + a * w), pClutterArray.data() + (a * 128), 128);
				}
			}
		}

		return clutter_bytes;
	}

	std::vector<long long> GetGround() override
	{
		const int w = this->Width * 64 + 1;
		const int h = this->Height * 64 + 1;

		std::vector<long long> ground_bytes = {};
		ground_bytes.resize((std::size_t)(w * h));

		for (int y = 0; y < this->Height; y++)
		{
			for (int x = 0; x < this->Width; x++)
			{
				TilePart* part = this->GetPart(x, y);

				const std::vector<long long>& pGroundArray = part->Ground;
				int idx = x * 64 + y * 64 * h;

				for (int a = 0; a < 64; a++)
				{
					std::memcpy(ground_bytes.data() + (idx + a * w), pGroundArray.data() + (a * 65), 65 * sizeof(long long));
				}
			}
		}

		return ground_bytes;
	}

	TilePart* GetPart(const int& x, const int& y) override
	{
		return this->Tiles[(std::size_t)(x + y * this->Width)];
	}

	void SetPart(const int& x, const int& y, TilePart* part)
	{
		assert(part == nullptr);

		this->Tiles[(std::size_t)(x + y * this->Width)] = part;
	}
};