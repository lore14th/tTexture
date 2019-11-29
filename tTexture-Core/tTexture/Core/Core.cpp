#include "pch.h"
#include "Core.h"

namespace tTexture {

	TextureData::TextureData()
		: TextureData(-1, -1, -1)
	{
	}

	TextureData::TextureData(int32_t width, int32_t height, int32_t bpp)
		: Width(width), Height(height), Bpp(bpp)
	{
	}

	Texture2D::Texture2D(const Texture2D& other)
	{
		TTEX_CORE_TRACE("Texture2D Copied");
		memcpy(this, &other, sizeof(Texture2D));
	}

	Texture2D::~Texture2D()
	{
		TTEX_CORE_TRACE("Texture2D deleted");
	}

	TextureCube::TextureCube(const TextureCube& other)
	{
		TTEX_CORE_TRACE("TextureCube Copied");
		memcpy(this, &other, sizeof(TextureCube));
	}

	TextureCube::~TextureCube()
	{
		TTEX_CORE_TRACE("Texture2D deleted");
	}

	std::pair<uint32_t, uint32_t> GetFaceLimits(CubeFormat format, Face face, uint32_t faceSize)
	{
		TTEX_ASSERT(format != CubeFormat::EQUIRECTANGULAR, "Loader:Cannot generate coordinates for Equirectangular conversion");
		switch (format)
		{
			case CubeFormat::HCROSS:
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
			} break;
			case tTexture::CubeFormat::VCROSS:
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
			} break;
		}

		return { 0, 0 };
	}

	std::pair<uint32_t, uint32_t> GetCoordinatesRelativeToFace(uint32_t x, uint32_t y, uint32_t faceSize, Face face)
	{
		std::pair<uint32_t, uint32_t> faceLimits = GetFaceLimits(CubeFormat::HCROSS, face, faceSize);
		return { x - faceLimits.first, y - faceLimits.second };
	}

	void FlipImageHorizontally(Texture2D& texture)
	{
		const uint32_t width = texture.Data.Width;
		const uint32_t height = texture.Data.Height;
		const uint32_t bpp = texture.Data.Bpp;

		const uint32_t h = height / 2;
		for(uint32_t y = 0; y < h; y++) 
		{
			for (uint32_t x = 0; x < width; x++)
			{
				uint32_t targetRow = height - y;

				uint32_t sourceIndex = x + y * height;
				uint32_t targetIndex = x + targetRow * height;

				for (uint32_t channel = 0; channel < bpp; channel++)
				{
					byte tempValue = texture.Image.Data[sourceIndex * bpp + channel];
					texture.Image.Data[sourceIndex * bpp + channel] = texture.Image.Data[targetIndex * bpp + channel];
					texture.Image.Data[targetIndex * bpp + channel] = tempValue;
				}
			}
		}
	}

	void FlipImageVertically(Texture2D& texture)
	{
		uint32_t width = texture.Data.Width;
		uint32_t height = texture.Data.Height;
		uint32_t bpp = texture.Data.Bpp;

		uint32_t low = 0;
		uint32_t high = (width * height * bpp) - bpp;

		while (low < high)
		{
			for (uint32_t channel = 0; channel < bpp; channel++)
			{
				byte tempValue = texture.Image[high + channel];
				texture.Image[high + channel] = texture.Image[low + channel];
				texture.Image[low + channel] = tempValue;
			}

			low += bpp;
			high -= bpp;
		}
	}

	void RemoveAlphaChannel(Texture2D& texture)
	{
		if (texture.Data.Bpp == 4)
		{
			Texture2D outTexture;

			outTexture.Data.Width = texture.Data.Width;
			outTexture.Data.Height = texture.Data.Height;
			outTexture.Data.Bpp = 3;

			for (int32_t y = 0; y < texture.Data.Height; y++)
			{
				for (int32_t x = 0; x < texture.Data.Width; x++)
				{
					outTexture.Image[(x + y * outTexture.Data.Height) * 3 + 0] = texture.Image[(x + y * texture.Data.Height) * texture.Data.Bpp + 0];
					outTexture.Image[(x + y * outTexture.Data.Height) * 3 + 1] = texture.Image[(x + y * texture.Data.Height) * texture.Data.Bpp + 1];
					outTexture.Image[(x + y * outTexture.Data.Height) * 3 + 2] = texture.Image[(x + y * texture.Data.Height) * texture.Data.Bpp + 2];
				}
			}
			texture = outTexture;
		}
	}

}

