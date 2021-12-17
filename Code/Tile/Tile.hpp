#pragma once

#include "Tile/TilePart.hpp"
#include "Tile/WriterOffset.hpp"
#include "Console.hpp"

#include <fstream>
#include <assert.h>
#include <vector>

class Tile
{
	int Version;
	int Type;
	long long CreatorId;
	int Width;
	int Height;
	std::vector<TilePart*> Tiles;

public:

	Tile(const int& width, const int& height)
	{
		this->Width = width;
		this->Height = height;
		this->Tiles.resize((std::size_t)(this->Width * this->Height));

		for (std::size_t a = 0; a < this->Tiles.size(); a++)
			this->Tiles[a] = new TilePart(this);
	}

	int GetWidth() const
	{
		return this->Width;
	}

	int GetHeight() const
	{
		return this->Height;
	}

	int GetVersion() const
	{
		return this->Version;
	}

	//UUID IMPLEMENTATION GOES HERE
	
	int GetTileType() const
	{
		return this->Type;
	}

	long long GetCreatorId() const
	{
		return this->CreatorId;
	}



	void SetVersion(const int& version)
	{
		this->Version = version;
	}

	void SetTileType(const int& type)
	{
		this->Type = type;
	}

	void SetCreatorId(const long long& creator_id)
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



	std::vector<float> GetVertexHeight()
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

	std::vector<int> GetVertexColor()
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

	std::vector<Byte> GetClutter()
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

	std::vector<long long> GetGround()
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

	TilePart* GetPart(const int& x, const int& y)
	{
		return this->Tiles[(std::size_t)(x + y * this->Width)];
	}

	void SetPart(const int& x, const int& y, TilePart* part)
	{
		assert(part == nullptr);

		this->Tiles[(std::size_t)(x + y * this->Width)] = part;
	}

	void WriteToFile(const std::wstring& path)
	{
		std::ofstream output_model(path);
		if (output_model.is_open())
		{
			DebugOutL("Writing terrain...");

			const std::vector<float> pHeightArray = this->GetVertexHeight();

			const int tWidth = this->GetWidth() * 32 + 1;
			const int tHeight = this->GetHeight() * 32 + 1;

			WriterOffsetData offset_data = { 0, 0, 0, 0 };
			offset_data.Vertex += (tHeight * tWidth);

			constexpr const float tile_size = 2.0f;

			const float half_width = ((float)tWidth / 2.0f) * tile_size;
			const float half_height = ((float)tHeight / 2.0f) * tile_size;

			for (int y = 0; y < tHeight; y++)
			{
				for (int x = 0; x < tWidth; x++)
				{
					const int index = x + y * tWidth;
					const float& height = pHeightArray[index];

					output_model << "v " << (((float)x * tile_size) - half_width) << " " << height << " " << (((float)y * tile_size) - half_height) << "\n";
				}
			}

			for (int y = 0; y < tHeight - 1; y++)
			{
				for (int x = 0; x < tWidth - 1; x++)
				{
					const int h00 = (x)+(y)*tWidth + 1;
					const int h01 = (x)+(y + 1) * tWidth + 1;
					const int h10 = (x + 1) + (y)*tWidth + 1;
					const int h11 = (x + 1) + (y + 1) * tWidth + 1;

					output_model << "f " << h00 << " " << h01 << " " << h10 << "\n";
					output_model << "f " << h01 << " " << h11 << " " << h10 << "\n";
				}
			}

			output_model << "o Assets\n";

			DebugOutL("Writing assets...");

			for (int y = 0; y < this->Width; y++)
			{
				for (int x = 0; x < this->Height; x++)
				{
					TilePart* tPart = this->GetPart(x, y);

					tPart->WriteToFile(output_model, offset_data, x, y);
				}
			}

			output_model.close();
		}
		
		DebugOutL("Finished!");
	}
};