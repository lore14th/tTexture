#pragma once

#include "Core/Core.h"

namespace tTexture {

	class Loader
	{
	public:
		Loader(const std::string& filepath, uint32_t desiredChannels = 3, bool flipOnLoad = false);

		void LoadImageFromFile(Texture2D& result);

		// returns a 4 channels image spitted up into cube faces.
		// you can use the Loader::Face enum to access the data inside result.Images
		void LoadCubeMapFromFile(CubeFormat format, TextureCube& result);

		static void FreeImageBuffer(byte* buffer);
	private:
		static void ConvertHCross(const Texture2D& sourceImage, TextureCube& result);
		static void ConvertVCross(const Texture2D& sourceImage, TextureCube& result);

		static void ReadSquareFace(Face face, CubeFormat format, const Texture2D& sourceImage, TextureCube& result);
		static void FlipFaceVertically(Face face, TextureCube& result);
	private:
		std::string m_Filepath;
		uint32_t m_DesiredChannels;
		bool m_FlipOnLoad;
	};

}