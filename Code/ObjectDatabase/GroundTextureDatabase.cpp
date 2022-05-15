#include "GroundTextureDatabase.hpp"

#include "Utils/String.hpp"
#include "ObjectDatabase/KeywordReplacer.hpp"

#include <stb/stb_image.h>
#include <lodepng/lodepng.h>
#include <libjpeg/jpeglib.h>

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
	this->mImageData.resize(xSize * ySize * 3);
}

bool GroundTexture::LoadImageData()
{
	if (!mImageData.empty()) return false;

	int img_x, img_y;
	int num_channels;

	Byte* temp_ptr = stbi_load(String::ToUtf8(mTexturePath).c_str(), &img_x, &img_y, &num_channels, 3);
	if (!temp_ptr)
	{
		DebugErrorL("Couldn't load the specified texture: ", mTexturePath);
		return false;
	}

	this->AllocateMemory((std::size_t)img_x, (std::size_t)img_y);
	std::memcpy(mImageData.data(), temp_ptr, mSizeX * mSizeY * 3);

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

Byte* GroundTexture::Data()
{
	return mImageData.data();
}

Byte GroundTexture::GetByte(const std::size_t& pX, const std::size_t& pY, const std::size_t& index) const
{
	return mImageData[3 * mSizeX * pY + 3 * pX + index];
}

void GroundTexture::SetByte(const std::size_t& pX, const std::size_t& pY, const std::size_t& index, const Byte& b)
{
	mImageData[3 * mSizeX * pY + 3 * pX + index] = b;
}

void GroundTexture::WriteToFile(const std::wstring& path, const int& quality) const
{
	/* This struct contains the JPEG compression parameters and pointers to
	 * working space (which is allocated as needed by the JPEG library).
	 * It is possible to have several such structures, representing multiple
	 * compression/decompression processes, in existence at once.  We refer
	 * to any one struct (and its associated working data) as a "JPEG object".
	 */
	struct jpeg_compress_struct cinfo;
	/* This struct represents a JPEG error handler.  It is declared separately
	 * because applications often want to supply a specialized error handler
	 * (see the second half of this file for an example).  But here we just
	 * take the easy way out and use the standard error handler, which will
	 * print a message on stderr and call exit() if compression fails.
	 * Note that this struct must live as long as the main JPEG parameter
	 * struct, to avoid dangling-pointer problems.
	 */
	struct jpeg_error_mgr jerr;
	/* More stuff */
	FILE* outfile;		/* target file */
	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
	int row_stride;		/* physical row width in image buffer */
	errno_t open_result; /* a result of fopen_s function */

	/* Step 1: allocate and initialize JPEG compression object */

	/* We have to set up the error handler first, in case the initialization
	 * step fails.  (Unlikely, but it could happen if you are out of memory.)
	 * This routine fills in the contents of struct jerr, and returns jerr's
	 * address which we place into the link field in cinfo.
	 */
	cinfo.err = jpeg_std_error(&jerr);
	/* Now we can initialize the JPEG compression object. */
	jpeg_create_compress(&cinfo);

	/* Step 2: specify data destination (eg, a file) */
	/* Note: steps 2 and 3 can be done in either order. */

	/* Here we use the library-supplied code to send compressed data to a
	 * stdio stream.  You can also write your own code to do something else.
	 * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
	 * requires it in order to write binary files.
	 */
	if ((open_result = fopen_s(&outfile, String::ToUtf8(path).c_str(), "wb")) != 0)
	{
		DebugOutL("Can't open ", path);
		return;
	}

	jpeg_stdio_dest(&cinfo, outfile);

	/* Step 3: set parameters for compression */

	/* First we supply a description of the input image.
	 * Four fields of the cinfo struct must be filled in:
	 */
	cinfo.image_width = (unsigned int)mSizeX; 	/* image width and height, in pixels */
	cinfo.image_height = (unsigned int)mSizeY;
	cinfo.input_components = 3;		/* # of color components per pixel */
	cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
	/* Now use the library's routine to set default compression parameters.
	 * (You must set at least cinfo.in_color_space before calling this,
	 * since the defaults depend on the source color space.)
	 */
	jpeg_set_defaults(&cinfo);
	/* Now you can set any non-default parameters you wish to.
	 * Here we just illustrate the use of quality (quantization table) scaling:
	 */
	jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

	/* Step 4: Start compressor */

	/* TRUE ensures that we will write a complete interchange-JPEG file.
	 * Pass TRUE unless you are very sure of what you're doing.
	 */
	jpeg_start_compress(&cinfo, TRUE);

	/* Step 5: while (scan lines remain to be written) */
	/*           jpeg_write_scanlines(...); */

	/* Here we use the library's state variable cinfo.next_scanline as the
	 * loop counter, so that we don't have to keep track ourselves.
	 * To keep things simple, we pass one scanline per call; you can pass
	 * more if you wish, though.
	 */
	row_stride = (int)mSizeX * 3;	/* JSAMPLEs per row in image_buffer */
	
	while (cinfo.next_scanline < cinfo.image_height)
	{
		/* jpeg_write_scanlines expects an array of pointers to scanlines.
		 * Here the array is only one element long, but you could pass
		 * more than one scanline at a time if that's more convenient.
		 */
		row_pointer[0] = (JSAMPROW)&mImageData[cinfo.next_scanline * row_stride];
		(void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	/* Step 6: Finish compression */

	jpeg_finish_compress(&cinfo);
	/* After finish_compress, we can close the output file. */
	fclose(outfile);

	/* Step 7: release JPEG compression object */

	/* This is an important step since it will release a good deal of memory. */
	jpeg_destroy_compress(&cinfo);

	/* And we're done! */
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