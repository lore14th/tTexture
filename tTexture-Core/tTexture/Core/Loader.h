#pragma once

#include "Core/Core.h"

namespace tTexture {

	class Loader
	{
	public:
		Loader(const std::string& filepath, uint32_t desiredChannels = 4, bool flipOnLoad = false);

		void LoadImageFromFile(Texture2D& result);
		void LoadCubeMapFromFile(TextureCubeFormat format, TextureCube& result);

		static void FreeImageBuffer(byte* buffer);
	private:
		static uint32_t ConvertHCross(const Texture2D& sourceImage, TextureCube& result);
		static uint32_t ConvertVCross(const Texture2D& sourceImage, TextureCube& result);

		static void ReadSquareFace(const byte*& image, TextureData& imageData, uint32_t faceSize, std::array<byte*, 6>& faces, uint32_t faceIndex, uint32_t i, uint32_t j);
		static void FlipFaceVertically(uint32_t faceSize, uint32_t bpp, uint32_t faceIndex, std::array<byte*, 6>& result);
	private:
		std::string m_Filepath;
		uint32_t m_DesiredChannels;
		bool m_FlipOnLoad;
	};

}