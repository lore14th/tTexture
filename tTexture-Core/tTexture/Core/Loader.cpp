#include "pch.h"
#include "Loader.h"
#include <stb/stb_image.h>

#include "Core/NativeLoader/HCrossLoader.h"
#include "Core/NativeLoader/VCrossLoader.h"
#include "Core/NativeLoader/EquirectangularLoader.h"

namespace tTexture {

	Loader::Loader(const std::string& filepath, uint32_t desiredChannels, bool flipOnLoad)
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

		// Create a Native Loader to convert the image
		std::unique_ptr <tTexture::Native::NativeLoader> loader;
		switch (format)
		{
			case CubeFormat::HCROSS: loader = std::make_unique<Native::HCrossLoader>(); break;
			case CubeFormat::VCROSS: loader = std::make_unique<Native::VCrossLoader>(); break;
			case CubeFormat::EQUIRECTANGULAR: loader = std::make_unique<Native::EquirectangularLoader>(); break;
		}
		loader->ConvertToCubeMap(sourceImage, result);
	}

	void Loader::FreeImageBuffer(byte* buffer)
	{
		if (buffer)
			stbi_image_free(buffer);
	}

}
