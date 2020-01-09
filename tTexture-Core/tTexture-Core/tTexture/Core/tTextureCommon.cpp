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
		: Data { width, height, bpp }
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

	std::pair<uint32_t, uint32_t> GetFaceLimits(CubeFormat format, Face face, uint32_t faceSize)
	{
		TTEX_ASSERT(format != CubeFormat::EQUIRECTANGULAR, "Loader:Cannot generate coordinates for Equirectangular conversion");
		if (format == CubeFormat::HCROSS)
		{
			switch (face)
			{
				case Face::POS_X: return { 0, faceSize };
				case Face::NEG_X: return { 2 * faceSize, faceSize };
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
				case Face::POS_X: return { 0, faceSize };
				case Face::NEG_X: return { 2 * faceSize, faceSize };
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

	std::shared_ptr<tTexture::Texture2D> AddAlphaChannel(const std::shared_ptr<Texture2D>& texture)
	{
		if (texture->Data.Bpp == 3)
		{
			const uint32_t width = texture->Data.Width;
			const uint32_t height = texture->Data.Height;
			const uint32_t bpp = 4;

			std::shared_ptr<Texture2D> outTexture = std::make_shared<Texture2D>(width, height, bpp);
			outTexture->AllocateTexture();

			for (int32_t y = 0; y < height; y++)
			{
				for (int32_t x = 0; x < width; x++)
				{
					outTexture->Image.Data[(x + y * height) * bpp + 0] = texture->Image.Data[(x + y * height) * 3 + 0];
					outTexture->Image.Data[(x + y * height) * bpp + 1] = texture->Image.Data[(x + y * height) * 3 + 1];
					outTexture->Image.Data[(x + y * height) * bpp + 2] = texture->Image.Data[(x + y * height) * 3 + 2];
					outTexture->Image.Data[(x + y * height) * bpp + 3] = 255;
				}
			}
		}
		return texture;
	}

	std::shared_ptr<Texture2D> RemoveAlphaChannel(const std::shared_ptr<Texture2D>& texture)
	{
		if (texture->Data.Bpp == 4)
		{
			const uint32_t width = texture->Data.Width;
			const uint32_t height = texture->Data.Height;
			const uint32_t bpp = 3;

			// create a rgb texture
			std::shared_ptr<Texture2D> outTexture = std::make_shared<Texture2D>(width, height, bpp);
			outTexture->AllocateTexture();

			for (int32_t y = 0; y < texture->Data.Height; y++)
			{
				for (int32_t x = 0; x < texture->Data.Width; x++)
				{
					for(byte channel = 0; channel < bpp; channel++)
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

}

