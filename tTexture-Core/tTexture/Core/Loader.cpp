#include "pch.h"
#include "Loader.h"
#include <stb_image/stb_image.h>

#ifndef TTEX_LOADER_SINGLE_THREAD
#include <thread>
#endif

namespace tTexture {

	Loader::Loader(const std::string& filepath, bool flipOnLoad, uint32_t desiredChannels)
		: m_Filepath(filepath), m_DesiredChannels(desiredChannels), m_FlipOnLoad(flipOnLoad)
	{
		TTEX_CORE_ASSERT(!m_Filepath.empty(), "Loader: You must provide a valid filepath");
		TTEX_CORE_ASSERT(m_DesiredChannels > 2 && m_DesiredChannels < 5, "TextureLoader: desiredChannels must be 3 or 4. desiredChannels = {0}", m_DesiredChannels);
	}

	void Loader::LoadImageFromFile(Texture2D& result)
	{
		TTEX_CORE_INFO("Loader: Loading Texture {0}, Flip on Load = {1}", m_Filepath, m_FlipOnLoad);

		stbi_set_flip_vertically_on_load((int)m_FlipOnLoad);
		byte* pixels = stbi_load(m_Filepath.c_str(), &result.Data.Width, &result.Data.Height, &result.Data.Bpp, m_DesiredChannels == 3 ? STBI_rgb : STBI_rgb_alpha);

		result.Image.Allocate(result.Data.Width * result.Data.Height * result.Data.Bpp);
		result.Image.Data = pixels;
	}

	void Loader::LoadCubeMapFromFile(CubeFormat format, TextureCube& result)
	{
		// Load image as a Texture2D
		Texture2D sourceImage;
		LoadImageFromFile(sourceImage);

		// Process image
		uint32_t faceSize = 0;
		switch (format)
		{
			case tTexture::CubeFormat::HCROSS: ConvertHCross(sourceImage, result);		break;
			case tTexture::CubeFormat::VCROSS: ConvertVCross(sourceImage, result);		break;
			case tTexture::CubeFormat::EQUIRECTANGULAR: TTEX_CORE_ASSERT(false, "Not supported right now");	break;
		}
	}

	void Loader::FreeImageBuffer(byte* buffer)
	{
		if (buffer)
			stbi_image_free(buffer);
	}

	void Loader::ConvertHCross(const Texture2D& sourceImage, TextureCube& result)
	{
		// Desired channels is 3 or 4 (asserted in the constructor)

		const uint32_t faceSize = sourceImage.Data.Width / 4;
		const uint32_t bpp = 4;

		TTEX_CORE_ASSERT(faceSize != 0, "Loader:Cubemap has invalid faceSize: {0}", faceSize);
		TTEX_CORE_ASSERT(faceSize == sourceImage.Data.Width / 3, "Loader:Cubemap faceSize error. Non square faces");

		for (auto& face : result.Images)
			face.Allocate(faceSize * faceSize * bpp);

		result.Data.Width = faceSize;
		result.Data.Height = faceSize;
		result.Data.Bpp = bpp;

	#ifdef TTEX_LOADER_SINGLE_THREAD
		ReadSquareFace(Face::POS_X, sourceImage, result);
		ReadSquareFace(Face::NEG_X, sourceImage, result);
		ReadSquareFace(Face::POS_Y, sourceImage, result);
		ReadSquareFace(Face::NEG_Y, sourceImage, result);
		ReadSquareFace(Face::POS_Z, sourceImage, result);
		ReadSquareFace(Face::NEG_Z, sourceImage, result);
	#else
		std::array<std::thread*, 6> threads;
		
		threads[0] = new std::thread(Loader::ReadSquareFace, Face::POS_X, CubeFormat::HCROSS, sourceImage, result);
		threads[1] = new std::thread(Loader::ReadSquareFace, Face::NEG_X, CubeFormat::HCROSS, sourceImage, result);
		threads[2] = new std::thread(Loader::ReadSquareFace, Face::POS_Y, CubeFormat::HCROSS, sourceImage, result);
		threads[3] = new std::thread(Loader::ReadSquareFace, Face::NEG_Y, CubeFormat::HCROSS, sourceImage, result);
		threads[4] = new std::thread(Loader::ReadSquareFace, Face::POS_Z, CubeFormat::HCROSS, sourceImage, result);
		threads[5] = new std::thread(Loader::ReadSquareFace, Face::NEG_Z, CubeFormat::HCROSS, sourceImage, result);
		
		for (auto& t : threads)
			t->join();
	#endif
	}

	void Loader::ConvertVCross(const Texture2D& sourceImage, TextureCube& result)
	{
		// Desired channels is 3 or 4 (asserted in the constructor)

		const uint32_t faceSize = sourceImage.Data.Width / 3;
		const uint32_t bpp = 4;

		TTEX_CORE_ASSERT(faceSize != 0, "Loader:Cubemap has invalid faceSize: {0}", faceSize);
		TTEX_CORE_ASSERT(faceSize == sourceImage.Data.Width / 4, "Loader:Cubemap faceSize error. Non square faces");

		for (auto& face : result.Images)
			face.Allocate(faceSize * faceSize * bpp);

		result.Data.Width = faceSize;
		result.Data.Height = faceSize;
		result.Data.Bpp = bpp;

#ifdef TTEX_LOADER_SINGLE_THREAD
		ReadSquareFace(Face::POS_X, sourceImage, result);
		ReadSquareFace(Face::NEG_X, sourceImage, result);
		ReadSquareFace(Face::POS_Y, sourceImage, result);
		ReadSquareFace(Face::NEG_Y, sourceImage, result);
		ReadSquareFace(Face::POS_Z, sourceImage, result);
		ReadSquareFace(Face::NEG_Z, sourceImage, result);
#else
		std::array<std::thread*, 6> threads;
		
		threads[0] = new std::thread(Loader::ReadSquareFace, Face::POS_X, CubeFormat::VCROSS, sourceImage, result);
		threads[1] = new std::thread(Loader::ReadSquareFace, Face::NEG_X, CubeFormat::VCROSS, sourceImage, result);
		threads[2] = new std::thread(Loader::ReadSquareFace, Face::POS_Y, CubeFormat::VCROSS, sourceImage, result);
		threads[3] = new std::thread(Loader::ReadSquareFace, Face::NEG_Y, CubeFormat::VCROSS, sourceImage, result);
		threads[4] = new std::thread(Loader::ReadSquareFace, Face::POS_Z, CubeFormat::VCROSS, sourceImage, result);
		threads[5] = new std::thread(Loader::ReadSquareFace, Face::NEG_Z, CubeFormat::VCROSS, sourceImage, result);
		
		for (auto& t : threads)
			t->join();
#endif
		FlipFaceVertically(Face::NEG_Y, result);
	}

	void Loader::ReadSquareFace(Face face, CubeFormat format, const Texture2D& sourceImage, TextureCube& result)
	{
		// Generate i, j coordinates to read the source texture
		uint32_t faceSize = result.Data.Width;
		uint32_t faceIndex = (uint32_t)face;
		std::pair<uint32_t, uint32_t> sourceUV = GetReadingCoordinates(format, face, faceSize);

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

	void Loader::FlipFaceVertically(Face face, TextureCube& result)
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

	std::pair<uint32_t, uint32_t> Loader::GetReadingCoordinates(CubeFormat format, Face face, uint32_t faceSize)
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

}
