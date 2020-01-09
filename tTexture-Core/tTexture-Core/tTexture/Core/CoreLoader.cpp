#include "pch.h"
#include "CoreLoader.h"

#include "Debug/tTextureAssert.h"

#include <stb_image/stb_image.h>

namespace tTexture {

	CoreLoader::CoreLoader(const std::string& filepath, bool flipOnLoad)
		: m_Filepath(filepath), m_FlipOnLoad(flipOnLoad)
	{
		TTEX_CORE_ASSERT(!m_Filepath.empty(), "Loader: You must provide a valid filepath");;
	}

	std::shared_ptr<tTexture::Texture2D> CoreLoader::LoadImageFromFile(bool addAlpha)
	{
		TTEX_CORE_INFO("Loader: Loading Texture {0}, Flip on Load = {1}", m_Filepath, m_FlipOnLoad);

		TextureData data;
		stbi_set_flip_vertically_on_load((int)m_FlipOnLoad);
		byte* pixels = stbi_load(m_Filepath.c_str(), &data.Width, &data.Height, &data.Bpp, STBI_default);

		std::shared_ptr<tTexture::Texture2D> texture = PrepareTexture2D(pixels, data);

		if (addAlpha)
			texture = AddAlphaChannel(texture);

		return texture;
	}

	std::shared_ptr<tTexture::TextureCube> CoreLoader::LoadHCrossFromFile()
	{
		// Loads the image from file as a Texture2D
		std::shared_ptr<Texture2D> sourceImage = LoadImageFromFile(false);

		const uint32_t faceSize = sourceImage->Data.Width / 4;
		const uint32_t bpp = 4;

		TTEX_CORE_ASSERT(faceSize != 0, "Loader:Cubemap has invalid faceSize: {0}", faceSize);
		TTEX_CORE_ASSERT(faceSize == sourceImage->Data.Height / 3, "Loader:Cubemap faceSize error. Non square faces");

		std::shared_ptr<TextureCube> result = std::make_shared<TextureCube>(faceSize, bpp);
		result->AllocateTexture();

#ifdef TTEX_LOADER_SINGLE_THREAD
		ReadSquareFace(Face::POS_X, CubeFormat::HCROSS, sourceImage, result);
		ReadSquareFace(Face::NEG_X, CubeFormat::HCROSS, sourceImage, result);
		ReadSquareFace(Face::POS_Y, CubeFormat::HCROSS, sourceImage, result);
		ReadSquareFace(Face::NEG_Y, CubeFormat::HCROSS, sourceImage, result);
		ReadSquareFace(Face::POS_Z, CubeFormat::HCROSS, sourceImage, result);
		ReadSquareFace(Face::NEG_Z, CubeFormat::HCROSS, sourceImage, result);
#else
		std::array<std::thread*, 6> threads;

		// Note: thread takes in args by value(no matter the function signature), use std::ref to pass by reference
		threads[0] = new std::thread(ReadSquareFace, Face::POS_X, CubeFormat::HCROSS, std::ref(sourceImage), std::ref(result));
		threads[1] = new std::thread(ReadSquareFace, Face::NEG_X, CubeFormat::HCROSS, std::ref(sourceImage), std::ref(result));
		threads[2] = new std::thread(ReadSquareFace, Face::POS_Y, CubeFormat::HCROSS, std::ref(sourceImage), std::ref(result));
		threads[3] = new std::thread(ReadSquareFace, Face::NEG_Y, CubeFormat::HCROSS, std::ref(sourceImage), std::ref(result));
		threads[4] = new std::thread(ReadSquareFace, Face::POS_Z, CubeFormat::HCROSS, std::ref(sourceImage), std::ref(result));
		threads[5] = new std::thread(ReadSquareFace, Face::NEG_Z, CubeFormat::HCROSS, std::ref(sourceImage), std::ref(result));

		for (auto& t : threads)
			t->join();
#endif

		return result;
	}

	std::shared_ptr<PrefilteredTextureCube> CoreLoader::LoadPrefilteredTextureHCrossFromFile()
	{
		// Load mip Zero
		CoreLoader mipZeroLoader(GeneratePrefilteredTextureFilepath(m_Filepath, 0), m_FlipOnLoad);
		std::shared_ptr<TextureCube> mipZero = mipZeroLoader.LoadHCrossFromFile();

		// Allocate the result based on the mip count
		uint32_t mipLevels = CalculateMipMapCount(mipZero->Data.Width, mipZero->Data.Height);
		std::shared_ptr<tTexture::PrefilteredTextureCube> result = std::make_shared<tTexture::PrefilteredTextureCube>();
		result->Allocate(mipLevels);

		// insert mip Zero into result
		result->PushLevel(mipZero);

		// Load all the other mip levels
		for (uint32_t mipLevel = 1; mipLevel < mipLevels; mipLevel++)
		{
			CoreLoader loader(GeneratePrefilteredTextureFilepath(m_Filepath, mipLevel), m_FlipOnLoad);
			result->PushLevel(loader.LoadHCrossFromFile());
		}

		return result;
	}

	std::shared_ptr<Texture2D> CoreLoader::PrepareTexture2D(byte* source, const TextureData& data)
	{
		std::shared_ptr<Texture2D> result = std::make_shared<Texture2D>(data.Width, data.Height, data.Bpp);
		result->Image.Data = source;
		result->Image.Size = data.Width * data.Height * data.Bpp;
		return result;
	}

	void CoreLoader::FlipFaceVertically(Face face, std::shared_ptr<TextureCube>& result)
	{
		uint32_t faceIndex = (uint32_t)face;

		uint32_t faceSize = result->Data.Width; // texture cube has width == height
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

	void CoreLoader::ReadSquareFace(Face face, CubeFormat format, const std::shared_ptr<Texture2D>& sourceImage, std::shared_ptr<TextureCube>& result)
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
					result->Images[faceIndex][(x + y * faceSize) * newBPP + 3] = 255;
				}
				else
					TTEX_CORE_ASSERT("Loader:Invalid convertion. source bpp: {0}, result bpp: {1}", sourceImage->Data.Bpp, result->Data.Bpp);
			}
		}
	}

	std::string CoreLoader::GeneratePrefilteredTextureFilepath(const std::string& baseFilepath, const uint32_t mipLevel)
	{
		size_t dotPosition = baseFilepath.find_last_of(".");
		std::string path(baseFilepath.c_str(), dotPosition);
		std::string extension(baseFilepath.c_str() + dotPosition, baseFilepath.length() - dotPosition);

		std::stringstream ss;
		ss << path << "-" << std::to_string(mipLevel) << extension;
		return ss.str();
	}

}
