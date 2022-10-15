#include "GroundTextureDatabase.hpp"

#include "Utils/String.hpp"
#include "ObjectDatabase/KeywordReplacer.hpp"

GroundTexture::GroundTexture(const std::wstring& path)
{
	this->SetPath(path);
}

GroundTexture::~GroundTexture()
{
	this->Clear();
}

bool GroundTexture::AllocateMemory(const int& xSize, const int& ySize)
{
	if (m_imageData)
		FreeImage_Unload(m_imageData);

	m_imageData = FreeImage_Allocate(xSize, ySize, 24);
	if (!m_imageData)
	{
		DebugErrorL("Couldn't allocate memory for size: ", xSize, ", ", ySize);
		return false;
	}

	m_width = xSize;
	m_height = ySize;

	return true;
}

void GroundTexture::Resize(const int& width, const int& height)
{
	if (!m_imageData)
		return;

	FIBITMAP* v_newImagePtr = FreeImage_Rescale(m_imageData, width, height);
	if (!v_newImagePtr)
	{
		DebugErrorL("Couldn't resize the ground texture (size: ", m_width, ", ", m_height, "; new_size: ", width, ", ", height, ")");
		return;
	}

	FreeImage_Unload(m_imageData);
	m_imageData = v_newImagePtr;

	this->m_width = width;
	this->m_height = height;
}

bool GroundTexture::LoadImageData()
{
	if (m_imageData)
	{
		DebugErrorL("Image data already exists!");
		return false;
	}

	FREE_IMAGE_FORMAT v_freeImageFormat = FreeImage_GetFileTypeU(m_texturePath.c_str());
	if (v_freeImageFormat == FIF_UNKNOWN)
		v_freeImageFormat = FreeImage_GetFIFFromFilenameU(m_texturePath.c_str());

	if (v_freeImageFormat == FIF_UNKNOWN)
	{
		DebugErrorL("Couldn't identity the image format (path: ", m_texturePath, ")");
		return false;
	}

	m_imageData = FreeImage_LoadU(v_freeImageFormat, m_texturePath.c_str());
	if (!m_imageData)
	{
		DebugErrorL("Couldn't load the specified texture: ", m_texturePath);
		return false;
	}

	if (FreeImage_GetBPP(m_imageData) != 24)
	{
		FIBITMAP* v_oldImagePtr = m_imageData;
		m_imageData = FreeImage_ConvertTo24Bits(m_imageData);
		if (!m_imageData)
		{
			DebugErrorL("Couldn't convert the image to 24 BPP");
		}

		FreeImage_Unload(v_oldImagePtr);
	}

	this->m_width = static_cast<int>(FreeImage_GetWidth(m_imageData));
	this->m_height = static_cast<int>(FreeImage_GetHeight(m_imageData));

	return true;
}

void GroundTexture::Clear()
{
	if (m_imageData)
	{
		FreeImage_Unload(m_imageData);
		m_imageData = nullptr;
	}
}

void GroundTexture::SetPath(const std::wstring& path)
{
	m_texturePath = KeywordReplacer::ReplaceKey(path);
	DebugOutL("TexPath: ", m_texturePath);
}

void GroundTexture::WriteToFile(const std::wstring& path, const int& quality) const
{
	if (!m_imageData)
		return;

	FreeImage_Save(FREE_IMAGE_FORMAT::FIF_JPEG, m_imageData, String::ToUtf8(path).c_str(), JPEG_QUALITYSUPERB);
}



GroundTexBundle GroundTextureDatabase::DefaultTex = {};
std::array<GroundTexBundle, 8> GroundTextureDatabase::TexStorage = {};

static const std::string objKeyVec[3] = { "Dif", "Asg", "Nor" };
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
	GroundTextureDatabase::ClearTextureDatabase();

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

void GroundTextureDatabase::ClearTextureDatabase()
{
	for (std::size_t a = 0; a < GroundTextureDatabase::TexStorage.size(); a++)
	{
		GroundTexBundle& m_Bundle = GroundTextureDatabase::TexStorage[a];

		for (std::size_t b = 0; b < m_Bundle.size(); b++)
		{
			if (!m_Bundle[b]) continue;

			delete m_Bundle[b];
			m_Bundle[b] = nullptr;
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