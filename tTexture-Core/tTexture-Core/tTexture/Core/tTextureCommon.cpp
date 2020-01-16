#include "pch.h"
#include "tTextureCommon.h"

#include "Debug/tTextureAssert.h"

namespace tTexture {

	TextureData::TextureData(int32_t width, int32_t height, int32_t bpp)
		: Width(width), Height(height), Bpp(bpp)
	{
	}

	Texture2D::Texture2D(const Texture2D& other)
	{
		TTEX_CORE_TRACE("Texture2D Copied");
		memcpy(this, &other, sizeof(Texture2D));
	}

	Texture2D::Texture2D(int32_t width, int32_t height, int32_t bpp)
		: Data{ width, height, bpp }
	{
	}

	void Texture2D::AllocateTexture()
	{
		Image.Allocate(Data.Width * Data.Height * Data.Bpp);
		Image.ZeroInitialze();
	}

	std::string Texture2D::ToString() const
	{
		std::stringstream os;
		os << "Width: " << Data.Width << "\tHeight: " << Data.Height << "\tBpp: " << Data.Bpp << std::endl;
		return os.str();
	}

	TextureCube::TextureCube(const TextureCube& other)
	{
		TTEX_CORE_TRACE("TextureCube Copied");
		memcpy(this, &other, sizeof(TextureCube));
	}

	TextureCube::TextureCube(int32_t faceSize, int32_t bpp)
		: Data{ faceSize, faceSize, bpp }
	{
	}

	void TextureCube::AllocateTexture()
	{
		for (auto& image : Images)
		{
			image.Allocate(Data.Width * Data.Height * Data.Bpp);
			image.ZeroInitialze();
		}
	}

	std::string TextureCube::ToString() const
	{
		std::stringstream os;
		os << "Width: " << Data.Width << "\tHeight: " << Data.Height << "\tBpp: " << Data.Bpp << std::endl;
		return os.str();
	}

	OutputFormat RetrieveOutputFormat(const std::string& filepath)
	{
		size_t extensionFlag = filepath.find_last_of(".");
		std::string_view extension(filepath.c_str() + extensionFlag, filepath.length() - extensionFlag);

		if (extension == ".tga")
			return OutputFormat::Tga;
		else if (extension == ".png")
			return OutputFormat::Png;
		else if (extension == ".jpg")
			return OutputFormat::Jpg;

		return OutputFormat::NONE;
	}

	tTexture::Face SelectFace(uint32_t x, uint32_t y, uint32_t faceSize)
	{
		if (y < faceSize) // TOP Row
		{
			if (x < faceSize || x > 2 * faceSize)
				return Face::NONE;
			else
				return Face::POS_Y;
		}
		else if (y > 2 * faceSize - 1) //BOTTOM Row
		{
			if (x < faceSize || x > 2 * faceSize)
				return Face::NONE;
			else
				return Face::NEG_Y;
		}
		else // Middle Row
		{
			if (x < faceSize)			return Face::NEG_X;
			else if (x < 2 * faceSize)	return Face::POS_Z;
			else if (x < 3 * faceSize)	return Face::POS_X;
			else						return Face::NEG_Z;
		}
	}

	std::pair<uint32_t, uint32_t> GetFaceLimits(CubeFormat format, Face face, uint32_t faceSize)
	{
		TTEX_ASSERT(format != CubeFormat::EQUIRECTANGULAR, "Loader:Cannot generate coordinates for Equirectangular conversion");
		if (format == CubeFormat::HCROSS)
		{
			switch (face)
			{
			case Face::POS_X: return { 2 * faceSize, faceSize };
			case Face::NEG_X: return { 0, faceSize };
			case Face::POS_Y: return { faceSize, 0 };
			case Face::NEG_Y: return { faceSize, 2 * faceSize };
			case Face::POS_Z: return { faceSize, faceSize };
			case Face::NEG_Z: return { 3 * faceSize, faceSize };
			}
		}
		else if (format == CubeFormat::VCROSS)
		{
			switch (face)
			{
			case Face::POS_X: return { 2 * faceSize, faceSize };
			case Face::NEG_X: return { 0, faceSize };
			case Face::POS_Y: return { faceSize, 0 };
			case Face::NEG_Y: return { faceSize, 2 * faceSize };
			case Face::POS_Z: return { faceSize, faceSize };
			case Face::NEG_Z: return { faceSize, 3 * faceSize };
			}
		}
		return { 0, 0 };
	}

	std::pair<uint32_t, uint32_t> GetCoordinatesRelativeToFace(uint32_t x, uint32_t y, uint32_t faceSize, Face face)
	{
		std::pair<uint32_t, uint32_t> faceLimits = GetFaceLimits(CubeFormat::HCROSS, face, faceSize);
		return { x - faceLimits.first, y - faceLimits.second };
	}

	std::shared_ptr<Texture2D> RemoveAlphaChannel(const std::shared_ptr<Texture2D>& texture)
	{
		if (texture->Data.Bpp == 4)
		{
			const int32_t width = texture->Data.Width;
			const int32_t height = texture->Data.Height;
			const int32_t bpp = 3;

			// create a rgb texture
			std::shared_ptr<Texture2D> outTexture = std::make_shared<Texture2D>(width, height, bpp);
			outTexture->AllocateTexture();

			for (int32_t y = 0; y < height; y++)
			{
				for (int32_t x = 0; x < width; x++)
				{
					for (byte channel = 0; channel < bpp; channel++)
						outTexture->Image[(x + y * height) * bpp + channel] = texture->Image[(x + y * height) * texture->Data.Bpp + channel];
				}
			}
			return outTexture;
		}
		return texture;
	}

	int32_t CalculateMipMapCount(int32_t width, int32_t height)
	{
		int32_t levels = 1;
		while ((width | height) >> levels)
		{
			levels++;
		}
		return levels;
	}

	int32_t CalculatePrefilterTextureWidth(uint32_t mipCount)
	{
		uint32_t mipFaceSize = 1; // smallest mip has size 1
		uint32_t result = 0;
		for (uint32_t i = 0; i < mipCount; i++)
		{
			result += 4 * mipFaceSize;		// every mip introduces for times its faceSize
			mipFaceSize = mipFaceSize * 2;
		}
		return result;
	}

	int32_t CalculatePrefilterTextureMipCount(int32_t width, int32_t height)
	{
		uint32_t faceSize = height / 3;
		uint32_t mipCount = CalculateMipMapCount(faceSize, faceSize);
		int32_t expectedWidth = CalculatePrefilterTextureWidth(mipCount);

		return expectedWidth == width ? mipCount : -1;
	}

}
