#include "pch.h"
#include "NativeLoader.h"

namespace tTexture::Native {

	void NativeLoader::ReadSquareFace(Face face, CubeFormat format, const Texture2D& sourceImage, TextureCube& result)
	{
		// Generate i, j coordinates to read the source texture
		uint32_t faceSize = result.Data.Width;
		uint32_t faceIndex = (uint32_t)face;
		std::pair<uint32_t, uint32_t> sourceUV = GetFaceLimits(format, face, faceSize);

		for (uint32_t y = 0; y < faceSize; y++)
		{
			uint32_t yOffset = y + sourceUV.second;
			for (uint32_t x = 0; x < faceSize; x++)
			{
				uint32_t xOffset = x + sourceUV.first;

				// if source bpp matches desired bpp, we just need to read all the channels
				if (sourceImage.Data.Bpp == result.Data.Bpp)
				{
					uint32_t bpp = sourceImage.Data.Bpp;
					TTEX_CORE_ASSERT(bpp == 3 || bpp == 4, "Loader:Invalid bpp: {0}", bpp);

					for (byte channel = 0; channel < bpp; channel++)
						result.Images[faceIndex][(x + y * faceSize) * bpp + channel] = sourceImage.Image[(xOffset + yOffset * sourceImage.Data.Width) * bpp + channel];
				}
				else if (sourceImage.Data.Bpp == 3 && result.Data.Bpp == 4) // we need to add the alpha channel
				{
					byte alphaValue = 0;
					uint32_t newBPP = result.Data.Bpp;

					result.Images[faceIndex][(x + y * faceSize) * newBPP + 0] = sourceImage.Image[(xOffset + yOffset * sourceImage.Data.Width) * sourceImage.Data.Bpp + 0];
					result.Images[faceIndex][(x + y * faceSize) * newBPP + 1] = sourceImage.Image[(xOffset + yOffset * sourceImage.Data.Width) * sourceImage.Data.Bpp + 1];
					result.Images[faceIndex][(x + y * faceSize) * newBPP + 2] = sourceImage.Image[(xOffset + yOffset * sourceImage.Data.Width) * sourceImage.Data.Bpp + 2];
					result.Images[faceIndex][(x + y * faceSize) * newBPP + 3] = alphaValue;
				}
				else
					TTEX_CORE_ASSERT("Loader:Invalid convertion. source bpp: {0}, result bpp: {1}", sourceImage.Data.Bpp, result.Data.Bpp);

			}
		}
	}

	void NativeLoader::FlipFaceVertically(Face face, TextureCube& result)
	{
		uint32_t faceIndex = (uint32_t)face;

		uint32_t faceSize = result.Data.Width;
		uint32_t bpp = result.Data.Bpp;

		uint32_t low = 0;
		uint32_t high = (faceSize * faceSize * bpp) - bpp;

		while (low < high)
		{
			for (uint32_t channel = 0; channel < bpp; channel++)
			{
				byte tempValue = result.Images[faceIndex][high + channel];
				result.Images[faceIndex][high + channel] = result.Images[faceIndex][low + channel];
				result.Images[faceIndex][low + channel] = tempValue;
			}

			low += bpp;
			high -= bpp;
		}
	}

}