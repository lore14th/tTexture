#include "pch.h"
#include "Loader.h"
#include "Core/Application.h"

#include <stb/stb_image.h>

#include "Core/NativeLoader/NativeLoader.h"

namespace tTexture {

	Loader::Loader(const std::string& filepath, uint32_t desiredChannels, bool flipOnLoad)
		: m_Filepath(filepath), m_DesiredChannels(desiredChannels), m_FlipOnLoad(flipOnLoad), m_ApplicationRef(nullptr)
	{
		TTEX_CORE_ASSERT(!m_Filepath.empty(), "Loader: You must provide a valid filepath");
		TTEX_CORE_ASSERT(m_DesiredChannels > 2 && m_DesiredChannels < 5, "TextureLoader: desiredChannels must be 3 or 4. desiredChannels = {0}", m_DesiredChannels);
	}

	std::shared_ptr<Texture2D> Loader::LoadImageFromFile()
	{
		TTEX_CORE_INFO("Loader: Loading Texture {0}, Flip on Load = {1}", m_Filepath, m_FlipOnLoad);
		std::shared_ptr<Texture2D> result = std::make_shared<Texture2D>();

		stbi_set_flip_vertically_on_load((int)m_FlipOnLoad);
		byte* pixels = stbi_load(m_Filepath.c_str(), &result->Data.Width, &result->Data.Height, &result->Data.Bpp, m_DesiredChannels == 3 ? STBI_rgb : STBI_rgb_alpha);

		result->Image.Allocate(result->Data.Width * result->Data.Height * result->Data.Bpp);
		result->Image.Data = pixels;

		return result;
	}

	std::shared_ptr<TextureCube> Loader::LoadCubeMapFromFile(CubeFormat format)
	{
		// Load image as a Texture2D
		std::shared_ptr<Texture2D> sourceImage = LoadImageFromFile();

		// Create a Native Loader to convert the image
		std::unique_ptr <tTexture::Native::NativeLoader> loader;
		switch (format)
		{
			case CubeFormat::HCROSS: loader = std::make_unique<Native::HCrossLoader>(); break;
			case CubeFormat::VCROSS: loader = std::make_unique<Native::VCrossLoader>(); break;
			case CubeFormat::EQUIRECTANGULAR: loader = std::make_unique<Native::EquirectangularLoader>(m_ApplicationRef->GetRenderer()); break;
		}
		return loader->ConvertToCubeMap(sourceImage);
	}

	void Loader::FreeImageBuffer(byte* buffer)
	{
		if (buffer)
			stbi_image_free(buffer);
	}

}
