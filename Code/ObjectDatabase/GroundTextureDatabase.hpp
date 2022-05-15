#pragma once

#include <array>
#include <vector>
#include <string>

#include "Utils/ByteImpl.hpp"
#include "Utils/Json.hpp"

class GroundTexture
{
	std::size_t mSizeX, mSizeY;
	std::wstring mTexturePath;
	std::vector<Byte> mImageData;

public:
	GroundTexture() = default;
	GroundTexture(const std::wstring& path);
	GroundTexture(const GroundTexture&) = delete;
	GroundTexture(GroundTexture&&) = delete;
	GroundTexture(GroundTexture&) = delete;
	~GroundTexture();

	void AllocateMemory(const std::size_t& xSize, const std::size_t& ySize);

	bool LoadImageData();
	void Clear();

	void SetPath(const std::wstring& path);

	std::size_t GetWidth() const;
	std::size_t GetHeight() const;

	Byte* Data();

	Byte GetByte(const std::size_t& pX, const std::size_t& pY, const std::size_t& index) const;
	void SetByte(const std::size_t& pX, const std::size_t& pY, const std::size_t& index, const Byte& b);

	void WriteToFile(const std::wstring& path, const int& quality = 90) const;
};

using GroundTexBundle = std::array<GroundTexture*, 3>;

class GroundTextureDatabase
{
	static GroundTexBundle DefaultTex;
	static std::array<GroundTexBundle, 8> TexStorage;

	GroundTextureDatabase() = default;

	static void LoadTextureData(const nlohmann::json& tObj, GroundTexBundle& tBundle);
public:
	static void Initialize();
	static void ClearTextureDatabase();

	static GroundTexture* GetDefaultTexture(const std::size_t& index);
	static GroundTexture* GetTexture(const std::size_t& index, const std::size_t& type);
};