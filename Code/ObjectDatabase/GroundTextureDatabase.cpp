#include "GroundTextureDatabase.hpp"

#include "Utils/String.hpp"
#include "ObjectDatabase/KeywordReplacer.hpp"

#include <stb/stb_image.h>
#include <lodepng/lodepng.h>

#include "Console.hpp"

GroundTexture::GroundTexture(const std::wstring& path)
{
	this->SetPath(path);
}

GroundTexture::~GroundTexture()
{
	this->Clear();
}

void GroundTexture::AllocateMemory(const std::size_t& xSize, const std::size_t& ySize)
{
	this->mSizeX = xSize;
	this->mSizeY = ySize;

	this->mImageData.clear();
	this->mImageData.resize(xSize * ySize * 4);
}

bool GroundTexture::LoadImageData()
{
	if (!mImageData.empty()) return false;

	int img_x, img_y;
	int num_channels;

	Byte* temp_ptr = stbi_load(String::ToUtf8(mTexturePath).c_str(), &img_x, &img_y, &num_channels, 4);
	if (!temp_ptr)
	{
		DebugErrorL("Couldn't load the specified texture: ", mTexturePath);
		return false;
	}

	this->AllocateMemory((std::size_t)img_x, (std::size_t)img_y);
	std::memcpy(mImageData.data(), temp_ptr, mSizeX * mSizeY * 4);

	stbi_image_free(temp_ptr);

	return true;
}

void GroundTexture::Clear()
{
	mImageData.clear();
}

void GroundTexture::SetPath(const std::wstring& path)
{
	this->mTexturePath = KeywordReplacer::ReplaceKey(path);
	DebugOutL("TexPath: ", this->mTexturePath);
}

std::size_t GroundTexture::GetWidth() const
{
	return mSizeX;
}

std::size_t GroundTexture::GetHeight() const
{
	return mSizeY;
}

Byte GroundTexture::GetByte(const std::size_t& pX, const std::size_t& pY, const std::size_t& index) const
{
	const std::size_t real_p_x = pX % mSizeX;
	const std::size_t real_p_y = pY % mSizeY;

	return mImageData[4 * mSizeX * real_p_y + 4 * real_p_x + index];
}

void GroundTexture::SetByte(const std::size_t& pX, const std::size_t& pY, const std::size_t& index, const Byte& b)
{
	const std::size_t real_p_x = pX % mSizeX;
	const std::size_t real_p_y = pY % mSizeY;

	mImageData[4 * mSizeX * real_p_y + 4 * real_p_x + index] = b;
}

void GroundTexture::WriteToFile(const std::wstring& path) const
{
	std::vector<Byte> img_data_png;
	lodepng::State state;

	unsigned int error = lodepng::encode(img_data_png, mImageData, (unsigned)mSizeX, (unsigned)mSizeY, state);
	if (!error)
	{
		if (lodepng::save_file(img_data_png, String::ToUtf8(path)))
		{
			DebugErrorL("Couldn't save image: ", path);
		}
	}
	else
	{
		DebugErrorL("Couldn't encode image: ", path);
	}
}



GroundTexBundle GroundTextureDatabase::DefaultTex = {};
std::array<GroundTexBundle, 8> GroundTextureDatabase::TexStorage = {};

static const std::string objKeyVec[] = {"Dif", "Asg", "Nor"};
void GroundTextureDatabase::LoadTextureData(const nlohmann::json& tObj, GroundTexBundle& tBundle)
{
	for (std::size_t a = 0; a < 3; a++)
	{
		const std::string& cur_val = objKeyVec[a];
		const auto& tex_obj = JsonReader::Get(tObj, cur_val);

		const std::wstring wstr_tex = tex_obj.is_string() ? String::ToWide(tex_obj.get<std::string>()) : L"";

		tBundle[a] = new GroundTexture(wstr_tex);
	}
}

void GroundTextureDatabase::Initialize()
{
	const auto& tex_data = JsonReader::LoadParseJson(L"./Resources/GroundTextures.json");
	if (!tex_data.is_object()) return;

	const auto& def_textures = JsonReader::Get(tex_data, "DefaultTexture");
	if (def_textures.is_object())
		GroundTextureDatabase::LoadTextureData(def_textures, GroundTextureDatabase::DefaultTex);

	const auto& tex_list = JsonReader::Get(tex_data, "GroundTextureList");
	if (tex_list.is_array())
	{
		std::size_t mTexDataIdx = 0;

		for (const auto& mTexObj : tex_list)
		{
			if (!mTexObj.is_object()) continue;

			GroundTextureDatabase::LoadTextureData(mTexObj, GroundTextureDatabase::TexStorage[mTexDataIdx++]);

			if (mTexDataIdx == 8) break;
		}
	}
}

GroundTexture* GroundTextureDatabase::GetDefaultTexture(const std::size_t& index)
{
	return GroundTextureDatabase::DefaultTex[index];
}

GroundTexture* GroundTextureDatabase::GetTexture(const std::size_t& index, const std::size_t& type)
{
	return GroundTextureDatabase::TexStorage[index][type];
}