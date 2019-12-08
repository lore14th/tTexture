#include "pch.h"
#include "NativeLoader.h"

#include "Renderer/OpenGLRenderer.h"

namespace tTexture::Native {

	// -- Native Loader --

	void NativeLoader::ReadSquareFace(Face face, CubeFormat format, const std::shared_ptr<Texture2D>& sourceImage, std::shared_ptr<TextureCube>& result)
	{
		// Generate i, j coordinates to read the source texture
		uint32_t faceSize = result->Data.Width;
		uint32_t faceIndex = (uint32_t)face;
		std::pair<uint32_t, uint32_t> sourceUV = GetFaceLimits(format, face, faceSize);

		for (uint32_t y = 0; y < faceSize; y++)
		{
			uint32_t yOffset = y + sourceUV.second;
			for (uint32_t x = 0; x < faceSize; x++)
			{
				uint32_t xOffset = x + sourceUV.first;

				// if source bpp matches desired bpp, we just need to read all the channels
				if (sourceImage->Data.Bpp == result->Data.Bpp)
				{
					uint32_t bpp = sourceImage->Data.Bpp;
					TTEX_CORE_ASSERT(bpp == 3 || bpp == 4, "Loader:Invalid bpp: {0}", bpp);

					for (byte channel = 0; channel < bpp; channel++)
						result->Images[faceIndex][(x + y * faceSize) * bpp + channel] = sourceImage->Image[(xOffset + yOffset * sourceImage->Data.Width) * bpp + channel];
				}
				else if (sourceImage->Data.Bpp == 3 && result->Data.Bpp == 4) // we need to add the alpha channel
				{
					byte alphaValue = 0;
					uint32_t newBPP = result->Data.Bpp;

					result->Images[faceIndex][(x + y * faceSize) * newBPP + 0] = sourceImage->Image[(xOffset + yOffset * sourceImage->Data.Width) * sourceImage->Data.Bpp + 0];
					result->Images[faceIndex][(x + y * faceSize) * newBPP + 1] = sourceImage->Image[(xOffset + yOffset * sourceImage->Data.Width) * sourceImage->Data.Bpp + 1];
					result->Images[faceIndex][(x + y * faceSize) * newBPP + 2] = sourceImage->Image[(xOffset + yOffset * sourceImage->Data.Width) * sourceImage->Data.Bpp + 2];
					result->Images[faceIndex][(x + y * faceSize) * newBPP + 3] = alphaValue;
				}
				else
					TTEX_CORE_ASSERT("Loader:Invalid convertion. source bpp: {0}, result bpp: {1}", sourceImage->Data.Bpp, result->Data.Bpp);

			}
		}
	}

	void NativeLoader::FlipFaceVertically(Face face, std::shared_ptr<TextureCube>& result)
	{
		uint32_t faceIndex = (uint32_t)face;

		uint32_t faceSize = result->Data.Width;
		uint32_t bpp = result->Data.Bpp;

		uint32_t low = 0;
		uint32_t high = (faceSize * faceSize * bpp) - bpp;

		while (low < high)
		{
			for (uint32_t channel = 0; channel < bpp; channel++)
			{
				byte tempValue = result->Images[faceIndex][high + channel];
				result->Images[faceIndex][high + channel] = result->Images[faceIndex][low + channel];
				result->Images[faceIndex][low + channel] = tempValue;
			}

			low += bpp;
			high -= bpp;
		}
	}

	// -- HCross Loader --

	std::shared_ptr<TextureCube> HCrossLoader::ConvertToCubeMap(const std::shared_ptr<Texture2D>& sourceImage) const
	{
		//Note: Desired channels is 3 or 4 (asserted in Loader constructor)

		std::shared_ptr<TextureCube> result = std::make_shared<TextureCube>();

		const uint32_t faceSize = sourceImage->Data.Width / 4;
		const uint32_t bpp = 4;

		TTEX_CORE_ASSERT(faceSize != 0, "Loader:Cubemap has invalid faceSize: {0}", faceSize);
		TTEX_CORE_ASSERT(faceSize == sourceImage->Data.Height / 3, "Loader:Cubemap faceSize error. Non square faces");

		for (auto& face : result->Images)
			face.Allocate(faceSize * faceSize * bpp);

		result->Data.Width = faceSize;
		result->Data.Height = faceSize;
		result->Data.Bpp = bpp;

#ifdef TTEX_LOADER_SINGLE_THREAD
		ReadSquareFace(Face::POS_X, sourceImage, result);
		ReadSquareFace(Face::NEG_X, sourceImage, result);
		ReadSquareFace(Face::POS_Y, sourceImage, result);
		ReadSquareFace(Face::NEG_Y, sourceImage, result);
		ReadSquareFace(Face::POS_Z, sourceImage, result);
		ReadSquareFace(Face::NEG_Z, sourceImage, result);
#else
		std::array<std::thread*, 6> threads;

		// Note: thread takes in args by value(no matter the function signature), use std::ref to pass by reference
		threads[0] = new std::thread(NativeLoader::ReadSquareFace, Face::POS_X, CubeFormat::HCROSS, std::ref(sourceImage), std::ref(result));
		threads[1] = new std::thread(NativeLoader::ReadSquareFace, Face::NEG_X, CubeFormat::HCROSS, std::ref(sourceImage), std::ref(result));
		threads[2] = new std::thread(NativeLoader::ReadSquareFace, Face::POS_Y, CubeFormat::HCROSS, std::ref(sourceImage), std::ref(result));
		threads[3] = new std::thread(NativeLoader::ReadSquareFace, Face::NEG_Y, CubeFormat::HCROSS, std::ref(sourceImage), std::ref(result));
		threads[4] = new std::thread(NativeLoader::ReadSquareFace, Face::POS_Z, CubeFormat::HCROSS, std::ref(sourceImage), std::ref(result));
		threads[5] = new std::thread(NativeLoader::ReadSquareFace, Face::NEG_Z, CubeFormat::HCROSS, std::ref(sourceImage), std::ref(result));

		for (auto& t : threads)
			t->join();
#endif

		return result;
	}

	// -- VCross Loader --

	std::shared_ptr<TextureCube> VCrossLoader::ConvertToCubeMap(const std::shared_ptr<Texture2D>& sourceImage) const
	{
		// Desired channels is 3 or 4 (asserted in Loader constructor)

		std::shared_ptr<TextureCube> result = std::make_shared<TextureCube>();

		const uint32_t faceSize = sourceImage->Data.Width / 3;
		const uint32_t bpp = 4;

		TTEX_CORE_ASSERT(faceSize != 0, "Loader:Cubemap has invalid faceSize: {0}", faceSize);
		TTEX_CORE_ASSERT(faceSize == sourceImage->Data.Height / 4, "Loader:Cubemap faceSize error. Non square faces");

		for (auto& face : result->Images)
			face.Allocate(faceSize * faceSize * bpp);

		result->Data.Width = faceSize;
		result->Data.Height = faceSize;
		result->Data.Bpp = bpp;

#ifdef TTEX_LOADER_SINGLE_THREAD
		ReadSquareFace(Face::POS_X, sourceImage, result);
		ReadSquareFace(Face::NEG_X, sourceImage, result);
		ReadSquareFace(Face::POS_Y, sourceImage, result);
		ReadSquareFace(Face::NEG_Y, sourceImage, result);
		ReadSquareFace(Face::POS_Z, sourceImage, result);
		ReadSquareFace(Face::NEG_Z, sourceImage, result);
#else
		std::array<std::thread*, 6> threads;

		threads[0] = new std::thread(NativeLoader::ReadSquareFace, Face::POS_X, CubeFormat::VCROSS, std::ref(sourceImage), std::ref(result));
		threads[1] = new std::thread(NativeLoader::ReadSquareFace, Face::NEG_X, CubeFormat::VCROSS, std::ref(sourceImage), std::ref(result));
		threads[2] = new std::thread(NativeLoader::ReadSquareFace, Face::POS_Y, CubeFormat::VCROSS, std::ref(sourceImage), std::ref(result));
		threads[3] = new std::thread(NativeLoader::ReadSquareFace, Face::NEG_Y, CubeFormat::VCROSS, std::ref(sourceImage), std::ref(result));
		threads[4] = new std::thread(NativeLoader::ReadSquareFace, Face::POS_Z, CubeFormat::VCROSS, std::ref(sourceImage), std::ref(result));
		threads[5] = new std::thread(NativeLoader::ReadSquareFace, Face::NEG_Z, CubeFormat::VCROSS, std::ref(sourceImage), std::ref(result));

		for (auto& t : threads)
			t->join();
#endif
		FlipFaceVertically(Face::NEG_Z, result);
		return result;
	}

	// -- Equirectangular Loader --

	EquirectangularLoader::EquirectangularLoader(const std::unique_ptr<OpenGLRenderer>& renderer)
		: m_Renderer(renderer)
	{
		TTEX_CORE_ASSERT(m_Renderer, "EqirectangularLoader:Invalid Renderer!");
	}

	std::shared_ptr<TextureCube> EquirectangularLoader::ConvertToCubeMap(const std::shared_ptr<Texture2D>& sourceImage) const
	{
		//return m_Renderer->RenderEquirectangularTexture(sourceImage);
		return std::make_shared<TextureCube>();
	}

}