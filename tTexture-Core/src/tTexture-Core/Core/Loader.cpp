#include "Loader.h"

#include <stb_image/stb_image.h>

namespace tTexture {

	Loader::Loader(const std::string& filepath, uint32_t desiredChannels, bool flipOnLoad)
		: m_Filepath(filepath), m_DesiredChannels(desiredChannels), m_FlipOnLoad(flipOnLoad)
	{
		TTEX_CORE_ASSERT(m_Filepath != std::string(), "TextureLoader: You must provide a valid filepath");
		TTEX_CORE_ASSERT(m_DesiredChannels > 2 && m_DesiredChannels < 5, "TextureLoader: desiredChannels must be 3 or 4. desiredChannels = {0}", m_DesiredChannels);
	}

	void Loader::LoadImageFromFile(Texture2D& result)
	{
		TTEX_CORE_INFO("TextureLoader: Loading Texture {0}, Flip on Load = {1}", m_Filepath, m_FlipOnLoad);

		stbi_set_flip_vertically_on_load((int)m_FlipOnLoad);
		result.Image = stbi_load(m_Filepath.c_str(), &result.Data.Width, &result.Data.Height, &result.Data.Bpp, m_DesiredChannels == 3 ? STBI_rgb : STBI_rgb_alpha);
	}

	void Loader::LoadCubeMapFromFile(TextureCubeFormat format, TextureCube& result)
	{
		// Assert its not Equirectangular

		// Load image as a Texture2D
		// Process image
	}

	void Loader::FreeImageBuffer(byte* buffer)
	{
		if (buffer)
			stbi_image_free(buffer);
	}

	uint32_t Loader::ConvertHCross(const Texture2D& sourceImage, TextureCube& result)
	{
		return 0;
	}

	uint32_t Loader::ConvertVCross(const Texture2D& sourceImage, TextureCube& result)
	{
		return 0;
	}

	void Loader::ReadSquareFace(const byte*& image, TextureData& imageData, uint32_t faceSize, std::array<byte*, 6>& faces, uint32_t faceIndex, uint32_t i, uint32_t j)
	{

	}

	void Loader::FlipFaceVertically(uint32_t faceSize, uint32_t bpp, uint32_t faceIndex, std::array<byte*, 6>& result)
	{

	}

}
