#include "Tile.hpp"

#include "ObjectDatabase/GroundTextureDatabase.hpp"
#include "ObjectDatabase/ProgCounter.hpp"
#include "ObjectDatabase/ObjectData.hpp"

#include "Tile/TileConverter.hpp"
#include "Utils/String.hpp"
#include "Console.hpp"

#include <PerlinNoise/PerlinNoise.hpp>
#include <gtc/matrix_transform.hpp>

#include <lodepng/lodepng.h>
#include <stb/stb_image.h>

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

Tile::~Tile()
{
	for (TilePart*& pPart : this->Tiles)
		delete pPart;
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

			for (int a = 0; a < 33; a++)
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

			for (int a = 0; a < 33; a++)
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

glm::vec3 CalculateNormalVector(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
	//p2 is the new origin (can be changed later)
	glm::vec3 a = p2 - p1;
	glm::vec3 b = p3 - p1;

	return glm::normalize(glm::cross(a, b));
}

Model* Tile::GenerateTerrainMesh(const std::vector<float>& height_map) const
{
	DebugOutL("Generating terrain mesh...");

	Model* tMesh = new Model();

	const std::size_t tWidth = (std::size_t)this->Width * 32 + 1;
	const std::size_t tHeight = (std::size_t)this->Height * 32 + 1;

	const float hWidth = (float)this->Width * 32.0f;
	const float hHeight = (float)this->Height * 32.0f;

	const bool eNormals = ConvertSettings::ExportNormals;
	const bool eUvs = ConvertSettings::ExportUvs;

	std::vector<std::size_t> normal_div = {};
	if (eNormals)
	{
		tMesh->normals.reserve(tWidth * tHeight);
		normal_div.reserve(tWidth * tHeight);
	}

	tMesh->vertices.reserve(tWidth * tHeight);
	for (std::size_t y = 0; y < tHeight; y++)
	{
		for (std::size_t x = 0; x < tWidth; x++)
		{
			const float& height = height_map[x + y * tWidth];
			const float vert_x = -((float)x * 2.0f) + hWidth;
			const float vert_y = -((float)y * 2.0f) + hHeight;

			tMesh->vertices.push_back(glm::vec3(vert_x, vert_y, height));

			if (eNormals)
			{
				tMesh->normals.push_back(glm::vec3(0.0f));
				normal_div.push_back(1);
			}
		}
	}

	if (eUvs)
	{
		const float uvWidth = (float)(tWidth - 1);
		const float uvHeight = (float)(tHeight - 1);

		tMesh->uvs.reserve(tWidth * tHeight);
		for (std::size_t y = 0; y < tHeight; y++)
		{
			for (std::size_t x = 0; x < tWidth; x++)
			{
				const float u = (float)x / uvWidth;
				const float v = uvHeight - (float)y / uvHeight;

				tMesh->uvs.push_back(glm::vec2(u, v));
			}
		}
	}

	SubMeshData* pSubMesh = new SubMeshData(0);

	//generate normals
	pSubMesh->DataIdx.reserve(tWidth * tHeight);
	for (std::size_t y = 0; y < tHeight - 1; y++)
	{
		for (std::size_t x = 0; x < tWidth - 1; x++)
		{
			const std::size_t h00 = (x    ) + (y    ) * tWidth;
			const std::size_t h01 = (x    ) + (y + 1) * tWidth;
			const std::size_t h10 = (x + 1) + (y    ) * tWidth;
			const std::size_t h11 = (x + 1) + (y + 1) * tWidth;

			if (eNormals)
			{
				const glm::vec3& p1 = tMesh->vertices[h00];
				const glm::vec3& p2 = tMesh->vertices[h01];
				const glm::vec3& p3 = tMesh->vertices[h10];
				const glm::vec3& p4 = tMesh->vertices[h11];

				const glm::vec3 t1_norm = CalculateNormalVector(p1, p3, p2); //first_triangle
				const glm::vec3 t2_norm = CalculateNormalVector(p2, p3, p4); //second_triangle

				tMesh->normals[h00] += t1_norm;
				tMesh->normals[h01] += (t1_norm + t2_norm);
				tMesh->normals[h10] += (t1_norm + t2_norm);
				tMesh->normals[h11] += t2_norm;

				normal_div[h00] += 1;
				normal_div[h01] += 2;
				normal_div[h10] += 2;
				normal_div[h11] += 1;
			}

			const long long l00 = (long long)h00;
			const long long l01 = (long long)h01;
			const long long l10 = (long long)h10;
			const long long l11 = (long long)h11;

			const VertexData vert1 = { l00, eUvs ? l00 : -1, eNormals ? l00 : -1 };
			const VertexData vert2 = { l01, eUvs ? l01 : -1, eNormals ? l01 : -1 };
			const VertexData vert3 = { l10, eUvs ? l10 : -1, eNormals ? l10 : -1 };
			const VertexData vert4 = { l11, eUvs ? l11 : -1, eNormals ? l11 : -1 };

			std::vector<VertexData> pVertData1 = { vert1, vert3, vert2 };
			std::vector<VertexData> pVertData2 = { vert2, vert3, vert4 };

			pSubMesh->DataIdx.push_back(pVertData1);
			pSubMesh->DataIdx.push_back(pVertData2);
		}
	}

	if (eNormals)
	{
		//make normals smooth
		for (std::size_t y = 0; y < tHeight - 1; y++)
		{
			for (std::size_t x = 0; x < tWidth - 1; x++)
			{
				const std::size_t h00 = (x    ) + (y    ) * tWidth;
				const std::size_t h01 = (x    ) + (y + 1) * tWidth;
				const std::size_t h10 = (x + 1) + (y    ) * tWidth;
				const std::size_t h11 = (x + 1) + (y + 1) * tWidth;

				tMesh->normals[h00] = glm::normalize(tMesh->normals[h00] / (float)normal_div[h00]);
				tMesh->normals[h01] = glm::normalize(tMesh->normals[h01] / (float)normal_div[h01]);
				tMesh->normals[h10] = glm::normalize(tMesh->normals[h10] / (float)normal_div[h10]);
				tMesh->normals[h11] = glm::normalize(tMesh->normals[h11] / (float)normal_div[h11]);
			}
		}
	}

	tMesh->subMeshData.push_back(pSubMesh);

	return tMesh;
}

void Tile::WriteTerrain(std::ofstream& model, WriterOffsetData& mOffset, const std::vector<float>& height_map) const
{
	if (this->Type != 0) return;

	DebugOutL("Writing terrain...");
	ProgCounter::SetState(ProgState::WritingGroundMesh, 0);

	Model* terrain = this->GenerateTerrainMesh(height_map);
	terrain->WriteToFile(glm::mat4(1.0f), mOffset, model, nullptr);

	delete terrain;
}

void Tile::WriteClutter(std::ofstream& model, WriterOffsetData& mOffset, const std::vector<float>& height_map) const
{
	if (this->Type != 0 || !ConvertSettings::ExportClutter) return;

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
	const siv::PerlinNoise x_noise(6842u);
	const siv::PerlinNoise y_noise(1813u);

	ProgCounter::SetState(ProgState::WritingClutter, clWidth * clHeight);
	for (std::size_t y = 0; y < clWidth; y++)
	{
		for (std::size_t x = 0; x < clHeight; x++)
		{
			TileClutter* tClutter = tile_clutter[x + y * clWidth];
			if (!tClutter) continue;

			const float x_offset = (float)x_noise.octave2D_11((double)x * 91.42f, (double)y * 83.24f, 4) * 0.5f;
			const float y_offset = (float)y_noise.octave2D_11((double)x * 73.91f, (double)y * 98.46f, 4) * 0.5f;

			const float xPosClamp = std::clamp<float>((float)x + x_offset, 0.0f, (float)clWidth);
			const float yPosClamp = std::clamp<float>((float)y + y_offset, 0.0f, (float)clHeight);

			glm::vec3 tClutterPos;
			tClutterPos.x = -((float)x * 0.5f) + tWidth + x_offset;
			tClutterPos.y = -((float)y * 0.5f) + tHeight + y_offset;
			tClutterPos.z = GetHeightPoint(height_map, clWidth, clHeight, gridSizeX, gridSizeY, xPosClamp, yPosClamp);

			const float rot_angle = (float)rotation_noise.octave2D_11((double)x * 15.0, (double)y * 17.14, 4) * glm::two_pi<float>();
			const float rand_scale = (float)scale_noise.octave2D_11((double)x * 54.4f, (double)y * 24.54, 8) * tClutter->ScaleVariance();

			tClutter->SetPosition(tClutterPos);
			tClutter->SetRotation(glm::rotate(glm::quat(), rot_angle, glm::vec3(0.0f, 0.0f, 1.0f)));
			tClutter->SetSize(glm::vec3(0.25f - (rand_scale * 0.25f)));

			tClutter->WriteObjectToFile(model, mOffset, glm::mat4(1.0f));

			ProgCounter::ProgressValue++;
		}
	}
}

std::size_t Tile::GetAmountOfObjects() const
{
	std::size_t output = 0;

	for (const TilePart* tPart : this->Tiles)
		output += tPart->GetAmountOfObjects();

	return output;
}

void Tile::WriteAssets(std::ofstream& model, WriterOffsetData& mOffset) const
{
	DebugOutL("Writing assets...");

	ProgCounter::SetState(ProgState::WritingObjects, this->GetAmountOfObjects());
	for (int y = 0; y < this->Width; y++)
	{
		for (int x = 0; x < this->Height; x++)
		{
			TilePart* tPart = this->GetPart(x, y);

			tPart->WriteToFile(model, mOffset, x, y);
		}
	}
}

void WritePngImage(const std::vector<Byte>& img_data, const std::wstring& output_name, const unsigned& width, const unsigned& height)
{
	std::vector<Byte> img_data_png;
	lodepng::State state;

	unsigned int error = lodepng::encode(img_data_png, img_data, width, height, state);
	if (!error)
	{
		if (lodepng::save_file(img_data_png, String::ToUtf8(output_name)))
		{
			DebugErrorL("Couldn't save image: ", output_name);
		}
	}
	else
	{
		DebugErrorL("Couldn't encode image: ", output_name);
	}
}

void Tile::WriteMaterials(const std::wstring& dir) const
{
	if (!ConvertSettings::ExportMaterials) return;

	DebugOutL("Writing materials...");
	ProgCounter::SetState(ProgState::WritingMaterialMaps, 0);

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

		const std::wstring out_name = dir + L"MaterialMap" + std::to_wstring(mat_id) + L".png";
		WritePngImage(material_map, out_name, (unsigned)gnd_width, (unsigned)gnd_height);
	}
}

void Tile::WriteColorMap(const std::wstring& dir) const
{
	if (!ConvertSettings::ExportMaterials) return;

	DebugOutL("Writing color map...");
	ProgCounter::SetState(ProgState::WritingColorMap, 0);

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

	const std::wstring color_map_path = dir + L"ColorMap.png";
	WritePngImage(color_map, color_map_path, (unsigned)col_width, (unsigned)col_height);
}

void Tile::SampleTextures(GroundTexture* tex1, GroundTexture* out_tex, const std::vector<float>& material_map, const std::size_t& gnd_width, const std::size_t& gnd_height) const
{
	constexpr const float mul_div = 1.0f / 255.0f;

	const std::size_t tex1_x = tex1->GetWidth();
	const std::size_t tex1_y = tex1->GetHeight();

	const std::size_t out_tex_x = out_tex->GetWidth();
	const std::size_t out_tex_y = out_tex->GetHeight();

	for (std::size_t y = 0; y < out_tex_y; y++)
	{
		const float pY = (float)y;

		const std::size_t p_tex1_y = y % tex1_y;
		const std::size_t p_tex2_y = y % out_tex_y;

		for (std::size_t x = 0; x < out_tex_x; x++)
		{
			const float pX = (float)x;

			const float sample_data = GetHeightPoint(material_map, out_tex_x, out_tex_y, gnd_width, gnd_height, pX, pY);
			if (sample_data == 0.0f) continue;

			const std::size_t p_tex1_x = x % tex1_x;
			const std::size_t p_tex2_x = x % out_tex_x;

			float r_sample = (float)tex1->GetByte(p_tex1_x, p_tex1_y, 0) * mul_div;
			float g_sample = (float)tex1->GetByte(p_tex1_x, p_tex1_y, 1) * mul_div;
			float b_sample = (float)tex1->GetByte(p_tex1_x, p_tex1_y, 2) * mul_div;

			if (sample_data != 1.0f)
			{
				const float def_r = (float)out_tex->GetByte(p_tex2_x, p_tex2_y, 0) * mul_div;
				const float def_g = (float)out_tex->GetByte(p_tex2_x, p_tex2_y, 1) * mul_div;
				const float def_b = (float)out_tex->GetByte(p_tex2_x, p_tex2_y, 2) * mul_div;

				r_sample = lerp(def_r, r_sample, sample_data);
				g_sample = lerp(def_g, g_sample, sample_data);
				b_sample = lerp(def_b, b_sample, sample_data);
			}

			out_tex->SetByte(x, y, 0, (Byte)(r_sample * 255.0f));
			out_tex->SetByte(x, y, 1, (Byte)(g_sample * 255.0f));
			out_tex->SetByte(x, y, 2, (Byte)(b_sample * 255.0f));
		}
	}
}

void Tile::FillGndTexture(GroundTexture* mGndTex, const std::size_t& tex_id)
{
	GroundTexture* pDefTex = GroundTextureDatabase::GetDefaultTexture(tex_id);
	pDefTex->LoadImageData();

	const std::size_t gnd_tex_x = mGndTex->GetWidth();
	const std::size_t gnd_tex_y = mGndTex->GetHeight();

	const std::size_t def_size_x = pDefTex->GetWidth();
	const std::size_t def_size_y = pDefTex->GetHeight();

	const std::size_t def_x_amount = gnd_tex_x / def_size_x;
	const std::size_t def_y_amount = gnd_tex_y / def_size_y;

	const std::size_t def_width_one = def_size_x;

	for (std::size_t y = 0; y < def_y_amount; y++)
	{
		const std::size_t real_y = y * def_size_y;

		for (std::size_t x = 0; x < def_x_amount; x++)
		{
			const std::size_t real_x = x * def_size_x;
			const std::size_t idx = real_x + real_y * gnd_tex_y;

			for (std::size_t z = 0; z < def_size_y; z++)
			{
				const std::size_t global_offset = (idx + z * gnd_tex_x) * 3;
				const std::size_t local_offset = (z * def_width_one) * 3;

				std::memcpy(mGndTex->Data() + global_offset, pDefTex->Data() + local_offset, def_width_one * 3);
			}
		}
	}

	pDefTex->Clear();
}

struct MaterialData
{
	std::vector<float> MatData;
	bool HasMatData;
};

void Tile::FillMaterialMap(std::array<MaterialData, 8>& mat_data) const
{
	const std::vector<long long> ground_data = this->GetGround();

	const std::size_t gnd_width = (std::size_t)this->Width * 64 + 1;
	const std::size_t gnd_height = (std::size_t)this->Height * 64 + 1;

	for (std::size_t mat_id = 0; mat_id < 8; mat_id++)
	{
		const std::size_t mat_offset = 8 * mat_id;

		MaterialData& cur_mat = mat_data[mat_id];

		bool& has_data = cur_mat.HasMatData;
		std::vector<float>& material_vec = cur_mat.MatData;
		material_vec.resize(gnd_width * gnd_height);

		for (std::size_t y = 0; y < gnd_height; y++)
		{
			for (std::size_t x = 0; x < gnd_width; x++)
			{
				const long long& cur_data = ground_data[x + y * gnd_width];
				const Byte cur_chunk = (Byte)(cur_data >> mat_offset);
				const float float_chunk = (float)cur_chunk / 255.0f;

				material_vec[x + y * gnd_width] = float_chunk;

				if (!has_data && cur_chunk != 0)
					has_data = true;
			}
		}
	}
}

static const std::wstring GroundTextureNames[] = { L"Dif", L"Asg", L"Nor" };
void Tile::WriteGroundTextures(const std::wstring& dir) const
{
	if (this->Type != 0 || !ConvertSettings::ExportGroundTextures) return;

	std::array<MaterialData, 8> MaterialMap = {};
	this->FillMaterialMap(MaterialMap);

	const std::size_t tex_width = 4096 * (std::size_t)this->Width;
	const std::size_t tex_height = 4096 * (std::size_t)this->Height;

	const std::size_t gnd_width2 = (std::size_t)this->Width * 64;
	const std::size_t gnd_height2 = (std::size_t)this->Height * 64;

	for (std::size_t texture_id = 0; texture_id < 3; texture_id++)
	{
		const std::size_t writing_gnd_idx = static_cast<std::size_t>(ProgState::FillingGndDif) + (3 * texture_id);

		GroundTexture gnd_tex;
		gnd_tex.AllocateMemory(tex_width, tex_height);

		ProgCounter::SetState(static_cast<ProgState>(writing_gnd_idx), 8);
		Tile::FillGndTexture(&gnd_tex, texture_id);

		ProgCounter::SetState(static_cast<ProgState>(writing_gnd_idx + 1), 8);
		for (std::size_t mat_id = 0; mat_id < 8; mat_id++)
		{
			const MaterialData& cur_mat = MaterialMap[mat_id];
			if (cur_mat.HasMatData)
			{
				GroundTexture* pGndTex = GroundTextureDatabase::GetTexture(mat_id, texture_id);
				pGndTex->LoadImageData();

				this->SampleTextures(pGndTex, &gnd_tex, cur_mat.MatData, gnd_width2, gnd_height2);

				pGndTex->Clear();
			}

			ProgCounter::ProgressValue++;
		}

		ProgCounter::SetState(static_cast<ProgState>(writing_gnd_idx + 2), 0);
		gnd_tex.WriteToFile(dir + L"GroundTexture_" + GroundTextureNames[texture_id] + L".jpg");
	}
}

void Tile::WriteToFile(const std::wstring& dir_path, const std::wstring& file_name) const
{
	const std::wstring output_path = dir_path + file_name + L".obj";

	std::ofstream output_model(output_path);
	if (output_model.is_open())
	{
		const std::wstring mtl_name = file_name + L".mtl";
		const std::wstring mtl_path = dir_path + mtl_name;

		if (ConvertSettings::ExportMaterials)
		{
			const std::string mtl_header = "mtllib " + String::ToUtf8(mtl_name) + "\n";
			output_model.write(mtl_header.c_str(), mtl_header.size());
		}

		{
			WriterOffsetData offset_data;

			{
				const std::vector<float> pHeightArray = this->GetVertexHeight();

				this->WriteTerrain(output_model, offset_data, pHeightArray);
				this->WriteClutter(output_model, offset_data, pHeightArray);
			}

			this->WriteAssets(output_model, offset_data);

			output_model.close();
		}

		this->WriteMtlFile(mtl_path);

		this->WriteMaterials(dir_path);
		this->WriteColorMap (dir_path);
		this->WriteGroundTextures(dir_path);
	}

	DebugOutL("Finished!");
}

void Tile::WriteMtlFile(const std::wstring& path) const
{
	if (!ConvertSettings::ExportMaterials) return;

	std::ofstream oMtl(path);
	if (!oMtl.is_open()) return;

	DebugOutL("Writing an mtl file...");
	ProgCounter::SetState(ProgState::WritingMtlFile, 0);

	std::unordered_map<std::string, ObjectTexData> tData = {};

	for (const TilePart* tPart : this->Tiles)
	{
		tPart->FillTextureMap(tData);

		ProgCounter::ProgressMax = tData.size();
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

		ProgCounter::ProgressValue++;
	}
}