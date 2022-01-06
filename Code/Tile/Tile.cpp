#include "Tile.hpp"

#include "Console.hpp"
#include "Utils/String.hpp"

#include "ObjectDatabase/ObjectData.hpp"

#include <gtc/matrix_transform.hpp>
#include <PerlinNoise/PerlinNoise.hpp>
#include <lodepng/lodepng.h>

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

float GetHeightPoint(
	const std::vector<float>& vec,
	const std::size_t& vec_width,
	const std::size_t& vec_height,
	const std::size_t& gridSzX,
	const std::size_t& gridSzY,
	const float& x,
	const float& y
)
{
	const float gx = x / float(vec_height) * gridSzX;
	const float gy = y / float(vec_width) * gridSzY;

	const int gxi = int(gx);
	const int gyi = int(gy);

	const float& c00 = vec[(gxi    ) + (gyi    ) * (gridSzX + 1)];
	const float& c10 = vec[(gxi + 1) + (gyi    ) * (gridSzX + 1)];
	const float& c01 = vec[(gxi    ) + (gyi + 1) * (gridSzX + 1)];
	const float& c11 = vec[(gxi + 1) + (gyi + 1) * (gridSzX + 1)];

	return blerp(c00, c10, c01, c11, gx - gxi, gy - gyi);
}

char GetHeightCharacter(const float& height)
{
	if (height <= 0.0f) return '0';

	if (height > 0.0f && height < 0.1f)
		return '1';
	else if (height >= 0.1f && height < 0.2f)
		return '2';
	else if (height >= 0.2f && height < 0.3f)
		return '3';
	else if (height >= 0.3f && height < 0.4f)
		return '4';
	else if (height >= 0.4f && height < 0.5f)
		return '5';
	else if (height >= 0.5f && height < 0.6f)
		return '6';
	else if (height >= 0.6f && height < 0.7f)
		return '7';
	else if (height >= 0.7f && height < 0.8f)
		return '8';

	return '9';
}

void Tile::WriteTerrain(std::ofstream& model, WriterOffsetData& mOffset, const std::vector<float>& height_map) const
{
	DebugOutL("Writing terrain...");

	const int tWidth  = this->Width  * 32 + 1;
	const int tHeight = this->Height * 32 + 1;

	mOffset.Vertex += (std::size_t)(tHeight * tWidth);
	mOffset.Uv     += (std::size_t)(tHeight * tWidth);

	constexpr const float tile_size = 2.0f;

	const float hWidth = (float)this->Width * 32.0f;
	const float hHeight = (float)this->Height * 32.0f;

	for (int y = 0; y < tHeight; y++)
	{
		std::string height_row;
		height_row.resize(tWidth);

		for (int x = 0; x < tWidth; x++)
		{
			const float& height = height_map[x + y * tWidth];

			height_row[x] = GetHeightCharacter(height);

			const float x_point = ((float)-x * tile_size) + hWidth;
			const float y_point = ((float)-y * tile_size) + hHeight;

			model << "v " << x_point << " " << y_point << " " << height << "\n";
		}

		DebugOutL(height_row);
	}

	const float uvWidth = (float)(tWidth - 1);
	const float uvHeight = (float)(tHeight - 1);

	for (int y = 0; y < tHeight; y++)
	{
		for (int x = 0; x < tWidth; x++)
		{
			const float u = (float)x / uvWidth;
			const float v = uvHeight - (float)y / uvHeight;

			model << "vt " << u << " " << v << "\n";
		}
	}

	for (int y = 0; y < tHeight - 1; y++)
	{
		for (int x = 0; x < tWidth - 1; x++)
		{
			const int h00 = (x    ) + (y    ) * tWidth + 1;
			const int h01 = (x    ) + (y + 1) * tWidth + 1;
			const int h10 = (x + 1) + (y    ) * tWidth + 1;
			const int h11 = (x + 1) + (y + 1) * tWidth + 1;

			model << "f " << h00 << "/" << h00 << " " << h10 << "/" << h10 << " " << h01 << "/" << h01 << "\n";
			model << "f " << h01 << "/" << h01 << " " << h10 << "/" << h10 << " " << h11 << "/" << h11 << "\n";
		}
	}
}

void Tile::WriteClutter(std::ofstream& model, WriterOffsetData& mOffset, const std::vector<float>& height_map) const
{
	DebugOutL("Writing clutter...");

	std::vector<TileClutter*> tile_clutter = this->GetClutter();

	const std::size_t clWidth  = (std::size_t)this->Width  * 128;
	const std::size_t clHeight = (std::size_t)this->Height * 128;

	const int gridSizeX = this->Width  * 32;
	const int gridSizeY = this->Height * 32;

	const float tWidth  = (float)gridSizeX;
	const float tHeight = (float)gridSizeY;

	//initialize perlin noise
	const siv::PerlinNoise rotation_noise(1337u);
	const siv::PerlinNoise scale_noise(1488u);

	for (std::size_t y = 0; y < clWidth; y++)
	{
		std::string clutter_row;
		clutter_row.resize(clHeight);

		for (std::size_t x = 0; x < clHeight; x++)
		{
			TileClutter* tClutter = tile_clutter[x + y * clWidth];
			clutter_row[x] = (tClutter != nullptr) ? 'x' : '0';

			if (!tClutter) continue;

			glm::vec3 tClutterPos;
			tClutterPos.x = -((float)x * 0.5f) + tWidth;
			tClutterPos.y = -((float)y * 0.5f) + tHeight;
			tClutterPos.z = GetHeightPoint(height_map, clWidth, clHeight, gridSizeX, gridSizeY, (float)x, (float)y);

			const float rot_angle = (float)rotation_noise.octave2D_11((double)x * 15.0, (double)y * 17.14, 4) * glm::two_pi<float>();
			const float rand_scale = (float)scale_noise.octave2D_11((double)x * 54.4f, (double)y * 24.54, 8) * tClutter->ScaleVariance();

			tClutter->SetPosition(tClutterPos);
			tClutter->SetRotation(glm::rotate(glm::quat(), rot_angle, glm::vec3(0.0f, 0.0f, 1.0f)));
			tClutter->SetSize(glm::vec3(0.25f - (rand_scale * 0.25f)));

			tClutter->WriteObjectToFile(model, mOffset, glm::mat4(1.0f));
		}

		DebugOutL(clutter_row);
	}
}

void Tile::WriteAssets(std::ofstream& model, WriterOffsetData& mOffset) const
{
	DebugOutL("Writing assets...");

	for (int y = 0; y < this->Width; y++)
	{
		for (int x = 0; x < this->Height; x++)
		{
			TilePart* tPart = this->GetPart(x, y);

			tPart->WriteToFile(model, mOffset, x, y);
		}
	}
}

void WritePngImage(const std::vector<Byte>& img_data, const std::string& output_name, const unsigned& width, const unsigned& height)
{
	std::vector<Byte> img_data_png;
	lodepng::State state;

	unsigned int error = lodepng::encode(img_data_png, img_data, width, height, state);
	if (!error)
	{
		if (lodepng::save_file(img_data_png, output_name))
		{
			DebugErrorL("Couldn't save image: ", output_name);
		}
	}
	else
	{
		DebugErrorL("Couldn't encode image: ", output_name);
	}
}

void Tile::WriteMaterials() const
{
	DebugOutL("Writing materials...");

	const std::vector<long long> ground_data = this->GetGround();

	const std::size_t gnd_width  = (std::size_t)this->Width  * 64 + 1;
	const std::size_t gnd_height = (std::size_t)this->Height * 64 + 1;

	for (std::size_t mat_id = 0; mat_id < 2; mat_id++)
	{
		std::vector<Byte> material_map;
		material_map.resize(gnd_width * gnd_height * 4);

		for (std::size_t y = 0; y < gnd_width; y++)
		{
			for (std::size_t x = 0; x < gnd_height; x++)
			{
				const long long& cur_data = ground_data[x + y * gnd_width];

				const long cur_chunk = (long)(cur_data >> (32 * mat_id));

				material_map[4 * gnd_width * y + 4 * x + 0] = (Byte)(cur_chunk >> 0);
				material_map[4 * gnd_width * y + 4 * x + 1] = (Byte)(cur_chunk >> 8);
				material_map[4 * gnd_width * y + 4 * x + 2] = (Byte)(cur_chunk >> 16);
				material_map[4 * gnd_width * y + 4 * x + 3] = (Byte)(cur_chunk >> 24);
			}
		}

		const std::string out_name = "./MaterialMap" + std::to_string(mat_id) + ".png";
		WritePngImage(material_map, out_name, (unsigned)gnd_width, (unsigned)gnd_height);
	}
}

void Tile::WriteColorMap() const
{
	DebugOutL("Writing color map...");

	std::vector<int> vert_colors = this->GetVertexColor();

	const std::size_t col_width  = (std::size_t)this->Width  * 32 + 1;
	const std::size_t col_height = (std::size_t)this->Height * 32 + 1;

	std::vector<Byte> color_map;
	color_map.resize(col_width * col_height * 4);

	for (std::size_t y = 0; y < col_height; y++)
	{
		for (std::size_t x = 0; x < col_width; x++)
		{
			const int& cur_data = vert_colors[x + y * col_width];

			color_map[4 * col_width * y + 4 * x + 0] = (Byte)cur_data;
			color_map[4 * col_width * y + 4 * x + 1] = (Byte)(cur_data >> 8);
			color_map[4 * col_width * y + 4 * x + 2] = (Byte)(cur_data >> 16);
			color_map[4 * col_width * y + 4 * x + 3] = 255;
		}
	}

	WritePngImage(color_map, "./ColorMap.png", (unsigned)col_width, (unsigned)col_height);
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

		WriterOffsetData offset_data = { 0, 0, 0, 0 };
		const std::vector<float> pHeightArray = this->GetVertexHeight();

		this->WriteTerrain(output_model, offset_data, pHeightArray);
		this->WriteClutter(output_model, offset_data, pHeightArray);
		this->WriteAssets (output_model, offset_data);
		this->WriteMaterials();
		this->WriteColorMap();

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