#pragma once

#include "Core/tTextureCommon.h"

#include <optional>

namespace tTexture {

	// TODO: future features
	//channels == 1 -> R8
	//channels == 2 -> R8G8

	//channels == 3 -> R8G8B8A8
	//channels == 4 -> R8G8B8A8

	// This Loader only loads Texture
	class CoreLoader
	{
	public:
		CoreLoader(const std::string& filepath, bool flipOnLoad);
		~CoreLoader() = default;

		// returns a texture with the same bpp of the image that is loading. 
		//If you want to force RGBA, set addAlpha to true.
		std::shared_ptr<Texture2D> LoadImageFromFile(bool addAlpha = false);

		// returns a RGBA image spitted up into cube faces.
		// you can use the Loader::Face enum to access the data inside result.Images
		std::shared_ptr<TextureCube> LoadHCrossFromFile();

		// returns all the mips from a Prefilter texture. RGBA
		std::shared_ptr<PrefilteredTextureCube> LoadPrefilteredTexture();
	private:
		// return RGBA
		std::shared_ptr<TextureCube> AnalyzeHCross(const std::shared_ptr<Texture2D>& sourceImage, const uint32_t faceSize, const uint32_t xStart = 0) const;
		byte* AddAlphaChannel(byte* source, TextureData& data);

	public:
		// flips vertically the selected face
		static void FlipFaceVertically(Face face, std::shared_ptr<TextureCube>& result);

		// reads a face of a texture cube from a texture 2D. Call this function once per every face of the cube.
		// NOTE: this stores the texture cube image data in rgba format
		static void ReadSquareFace(Face face, CubeFormat format, const std::shared_ptr<Texture2D>& sourceImage, std::shared_ptr<TextureCube>& result, const uint32_t xStart = 0);
	private:
		std::string m_Filepath;
		bool m_FlipOnLoad;
	};

}