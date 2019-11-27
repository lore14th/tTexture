#pragma once

#include "Core/Core.h"

namespace tTexture {

	class Loader
	{
	public:
		enum class Face { POS_X = 0, NEG_X, POS_Y = 2, NEG_Y = 3, POS_Z = 4, NEG_Z = 5};
	public:
		Loader(const std::string& filepath, bool flipOnLoad = false, uint32_t desiredChannels = 4);

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

		static std::pair<uint32_t, uint32_t> GetReadingCoordinates(CubeFormat format, Face face, uint32_t faceSize);
	private:
		std::string m_Filepath;
		uint32_t m_DesiredChannels;
		bool m_FlipOnLoad;
	};

}