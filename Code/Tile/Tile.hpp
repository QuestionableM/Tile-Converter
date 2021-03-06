#pragma once

#include <vector>

#include "Tile/TilePart.hpp"

class GroundTexture;

class Tile
{
	int m_Version;
	int m_Type;
	long long m_CreatorId;
	int m_Width;
	int m_Height;
	std::vector<TilePart*> m_Tiles;

public:
	Tile(const int& width, const int& height);
	~Tile();

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
	std::vector<TileClutter*> GetClutter() const;
	std::vector<long long> GetGround() const;
	TilePart* GetPart(const int& x, const int& y) const;

private:
	Model* GenerateTerrainMesh(const std::vector<float>& height_map) const;
	void WriteTerrain(std::ofstream& model, WriterOffsetData& mOffset, const std::vector<float>& height_map) const;
	void WriteClutter(std::ofstream& model, WriterOffsetData& mOffset, const std::vector<float>& height_map) const;
	std::size_t GetAmountOfObjects() const;
	void WriteAssets(std::ofstream& model, WriterOffsetData& mOffset) const;
	void WriteMaterials(const std::wstring& dir) const;
	void WriteColorMap(const std::wstring& dir) const;

	static void FillGndTexture(GroundTexture* mGndTex, const std::size_t& tex_id);
	void FillMaterialMap(std::array<struct MaterialData, 8>& mat_data) const;
	void SampleTextures(GroundTexture* tex1, GroundTexture* out_tex, const std::vector<float>& material_map, const std::size_t& gnd_width, const std::size_t& gnd_height) const;
	void WriteGroundTextures(const std::wstring& dir) const;

public:
	void WriteToFile(const std::wstring& dir_path, const std::wstring& file_name) const;
	void WriteMtlFile(const std::wstring& path) const;
};