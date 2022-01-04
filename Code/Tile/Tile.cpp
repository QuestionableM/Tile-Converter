#include "Tile.hpp"

#include "Console.hpp"
#include "Utils/String.hpp"

#include "ObjectDatabase/ObjectData.hpp"

#include <gtc/matrix_transform.hpp>
#include <PerlinNoise/PerlinNoise.hpp>

#include <filesystem>
namespace fs = std::filesystem;

Tile::Tile(const int& width, const int& height)
{
	this->Width = width;
	this->Height = height;
	this->Tiles.resize((std::size_t)(this->Width * this->Height));

	for (std::size_t a = 0; a < this->Tiles.size(); a++)
		this->Tiles[a] = new TilePart(this);
}

int Tile::GetWidth() const
{
	return this->Width;
}

int Tile::GetHeight() const
{
	return this->Height;
}

int Tile::GetVersion() const
{
	return this->Version;
}

//UUID IMPLEMENTATION GOES HERE

int Tile::GetTileType() const
{
	return this->Type;
}

long long Tile::GetCreatorId() const
{
	return this->CreatorId;
}



void Tile::SetVersion(const int& version)
{
	this->Version = version;
}

void Tile::SetTileType(const int& type)
{
	this->Type = type;
}

void Tile::SetCreatorId(const long long& creator_id)
{
	this->CreatorId = creator_id;
}

void Tile::SetPart(const int& x, const int& y, TilePart* part)
{
	assert(part == nullptr);

	this->Tiles[(std::size_t)(x + y * this->Width)] = part;
}



void Tile::Resize(const int& width, const int& height)
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



std::vector<float> Tile::GetVertexHeight() const
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

std::vector<int> Tile::GetVertexColor() const
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

std::vector<TileClutter*> Tile::GetClutter() const
{
	const int w = this->Width * 128;
	const int h = this->Height * 128;

	std::vector<TileClutter*> clutter_bytes = {};
	clutter_bytes.resize((std::size_t)(w * h));

	for (int y = 0; y < this->Height; y++)
	{
		for (int x = 0; x < this->Width; x++)
		{
			TilePart* part = this->GetPart(x, y);

			const std::vector<TileClutter*>& pClutterArray = part->ClutterMap;
			int idx = x * 128 + y * 128 * h;

			for (int a = 0; a < 128; a++)
			{
				std::memcpy(clutter_bytes.data() + (idx + a * w), pClutterArray.data() + (a * 128), 128 * sizeof(std::size_t));
			}
		}
	}

	return clutter_bytes;
}

std::vector<long long> Tile::GetGround() const
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

TilePart* Tile::GetPart(const int& x, const int& y) const
{
	return this->Tiles[(std::size_t)(x + y * this->Width)];
}

float lerp(const float& s, const float& e, const float& t)
{
	return s + (e - s) * t;
}

float blerp(const float& c00, const float& c10, const float& c01, const float& c11, const float& tx, const float& ty)
{
	return lerp(lerp(c00, c10, tx), lerp(c01, c11, tx), ty);
}

float GetHeightPoint(const std::vector<float>& vec, const std::size_t& vec_width, const float& x, const float& y)
{
	const int x_int = int(x);
	const int y_int = int(y);

	const float c00 = vec[(x_int    ) + (y_int    ) * vec_width];
	const float c10 = vec[(x_int + 1) + (y_int    ) * vec_width];
	const float c01 = vec[(x_int    ) + (y_int + 1) * vec_width];
	const float c11 = vec[(x_int + 1) + (y_int + 1) * vec_width];

	const float x_inter = x - (float)x_int;
	const float y_inter = y - (float)y_int;

	return blerp(c00, c10, c01, c11, x_inter, y_inter);
}

void Tile::WriteToFile(const std::wstring& path) const
{
	std::ofstream output_model(path);
	if (output_model.is_open())
	{
		fs::path mPath = path;

		const bool is_mtl_valid = (mPath.has_stem() && mPath.has_parent_path());
		std::wstring mtl_path;

		if (is_mtl_valid)
		{
			mtl_path = mPath.parent_path().wstring() + L"/" + mPath.stem().wstring() + L".mtl";

			const std::string mtl_name = "mtllib " + mPath.stem().string() + ".mtl\n";
			output_model.write(mtl_name.c_str(), mtl_name.size());
		}

		DebugOutL("Writing terrain...");

		const std::vector<float> pHeightArray = this->GetVertexHeight();

		const int tWidth = this->GetWidth() * 32 + 1;
		const int tHeight = this->GetHeight() * 32 + 1;

		WriterOffsetData offset_data = { 0, 0, 0, 0 };
		offset_data.Vertex += (std::size_t)(tHeight * tWidth);

		constexpr const float tile_size = 2.0f;

		const float half_width = ((float)tWidth / 2.0f) * tile_size;
		const float half_height = ((float)tHeight / 2.0f) * tile_size;

		for (int y = 0; y < tHeight; y++)
		{
			std::string height_row;
			height_row.resize(tWidth);

			for (int x = 0; x < tWidth; x++)
			{
				const float& height = pHeightArray[x + y * tWidth];

				if (height > 0.0f)
				{
					if (height > 0.0f && height < 0.1f)
						height_row[x] = '1';
					else if (height >= 0.1f && height < 0.2f)
						height_row[x] = '2';
					else if (height >= 0.2f && height < 0.3f)
						height_row[x] = '3';
					else if (height >= 0.3f && height < 0.4f)
						height_row[x] = '4';
					else if (height >= 0.4f && height < 0.5f)
						height_row[x] = '5';
					else if (height >= 0.5f && height < 0.6f)
						height_row[x] = '6';
					else if (height >= 0.6f && height < 0.7f)
						height_row[x] = '7';
					else if (height >= 0.7f && height < 0.8f)
						height_row[x] = '8';
					else
						height_row[x] = '9';
				}
				else
				{
					height_row[x] = '0';
				}

				const float x_point = ((float)-x * tile_size) + half_width;
				const float y_point = ((float)-y * tile_size) + half_height;

				output_model << "v " << x_point << " " << y_point << " " << height << "\n";
			}

			DebugOutL(height_row);
		}

		for (int y = 0; y < tHeight - 1; y++)
		{
			for (int x = 0; x < tWidth - 1; x++)
			{
				const int h00 = (x    ) + (y    ) * tWidth + 1;
				const int h01 = (x    ) + (y + 1) * tWidth + 1;
				const int h10 = (x + 1) + (y    ) * tWidth + 1;
				const int h11 = (x + 1) + (y + 1) * tWidth + 1;

				output_model << "f " << h00 << " " << h10 << " " << h01 << "\n";
				output_model << "f " << h01 << " " << h10 << " " << h11 << "\n";
			}
		}

		{
			DebugOutL("Writing clutter...");

			std::vector<TileClutter*> tile_clutter = this->GetClutter();

			const std::size_t clWidth  = (std::size_t)this->Width  * 128;
			const std::size_t clHeight = (std::size_t)this->Height * 128;

			const float clHalfWidth = (float)clWidth * 0.5f;
			const float clHalfHeight = (float)clHeight * 0.5f;

			const std::size_t height_width = (std::size_t)this->Width * 32;

			const int gridSizeX = this->Width * 32;
			const int gridSizeY = this->Height * 32;

			//initialize perlin noise
			const siv::PerlinNoise rotation_noise(1337u);
			const siv::PerlinNoise scale_noise(1488u);

			const float rotation_max = glm::pi<float>() * 2.0f;

			for (std::size_t y = 0; y < clWidth; y++)
			{
				std::string clutter_row;
				clutter_row.resize(clHeight);

				for (std::size_t x = 0; x < clHeight; x++)
				{
					TileClutter* tClutter = tile_clutter[x + y * clWidth];
					clutter_row[x] = (tClutter != nullptr) ? 'x' : '0';

					if (!tClutter) continue;

					glm::vec3 tClutterPos(-((float)x * 0.5f) + half_width, -((float)y * 0.5f) + half_height, 0.0f);

					const float gx = x / float(clHeight) * gridSizeX;
					const float gy = y / float(clWidth)  * gridSizeY;

					const int gxi = int(gx);
					const int gyi = int(gy);

					const float& c00 = pHeightArray[(gxi    ) + (gyi    ) * (gridSizeX + 1)];
					const float& c10 = pHeightArray[(gxi + 1) + (gyi    ) * (gridSizeX + 1)];
					const float& c01 = pHeightArray[(gxi    ) + (gyi + 1) * (gridSizeX + 1)];
					const float& c11 = pHeightArray[(gxi + 1) + (gyi + 1) * (gridSizeX + 1)];

					tClutterPos.z = blerp(c00, c10, c01, c11, gx - gxi, gy - gyi);

					const float rot_angle = (float)rotation_noise.octave2D_11((double)x * 15.0, (double)y * 17.14, 4) * rotation_max;
					const float rand_scale = (float)scale_noise.octave2D_11((double)x * 54.4f, (double)y * 24.54, 8) * tClutter->pParent->ScaleVariance;

					tClutter->SetPosition(tClutterPos);
					tClutter->SetRotation(glm::rotate(glm::quat(), rot_angle, glm::vec3(0.0f, 0.0f, 1.0f)));
					tClutter->SetSize(glm::vec3(0.25f - (rand_scale * 0.25f)));

					tClutter->WriteObjectToFile(output_model, offset_data, glm::mat4(1.0f));
				}

				DebugOutL(clutter_row);
			}
		}

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

		if (!mtl_path.empty())
			this->WriteMtlFile(mtl_path);
	}

	DebugOutL("Finished!");
}

void Tile::WriteMtlFile(const std::wstring& path) const
{
	std::ofstream oMtl(path);
	if (!oMtl.is_open()) return;

	DebugOutL("Writing an mtl file...");

	std::unordered_map<std::string, ObjectTexData> tData = {};

	for (const TilePart* tPart : this->Tiles)
	{
		for (std::size_t a = 0; a < tPart->Objects.size(); a++)
		{
			for (const TileEntity* cObject : tPart->Objects[a])
				cObject->FillTextureMap(tData);
		}
		
		for (const TileClutter* cClutter : tPart->ClutterMap)
		{
			if (!cClutter) continue;

			cClutter->FillTextureMap(tData);
		}
	}

	for (const auto& tDatum : tData)
	{
		std::string output_str = "newmtl " + tDatum.first;
		output_str.append("\nNs 324");
		output_str.append("\nKa 1 1 1\nKd ");
		output_str.append(tDatum.second.TexColor.StringNormalized());
		output_str.append("\nKs 0.5 0.5 0.5");
		output_str.append("\nKe 0 0 0");
		output_str.append("\nNi 1.45");
		output_str.append("\nd 1");
		output_str.append("\nillum 2");

		const TextureList& tList = tDatum.second.Textures;

		if (!tList.nor.empty()) output_str.append("\nmap_Bump " + String::ToUtf8(tList.nor));
		if (!tList.dif.empty()) output_str.append("\nmap_Kd " + String::ToUtf8(tList.dif));
		if (!tList.asg.empty()) output_str.append("\nmap_Ks " + String::ToUtf8(tList.asg));

		output_str.append("\n\n");

		oMtl.write(output_str.c_str(), output_str.size());
	}
}