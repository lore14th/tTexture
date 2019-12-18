#pragma once

#include "Core/Core.h"

namespace tTexture {

	//channels == 1 -> R8
	//channels == 2 -> R8G8
	//channels == 3 -> R8G8B8A8
	//channels == 4 -> R8G8B8A8

	// This Loader only loads Texture
	class CoreLoader
	{
	public:
		CoreLoader(const std::string& filepath, uint32_t fileChannels, bool flipOnLoad);
		~CoreLoader() = default;

		// returns a 4 channel texture
		std::shared_ptr<Texture2D> LoadImageFromFile();

		// returns a 4 channels image spitted up into cube faces.
		// you can use the Loader::Face enum to access the data inside result.Images
		std::shared_ptr<TextureCube> LoadHCrossFromFile();

		// returns a PrefileteredTextureCube. Every image has 4 channels
		std::shared_ptr<PrefilteredTextureCube> LoadPrefilteredTextureHCrossFromFile(uint32_t mipLevels);
	public:
		// flips vertically the selected face
		static void FlipFaceVertically(Face face, std::shared_ptr<TextureCube>& result);

		// reads a face of a texture cube from a texture 2D. Call this function once per every face of the cube.
		// NOTE: this stores the texture cube image data in rgba format
		static void ReadSquareFace(Face face, CubeFormat format, const std::shared_ptr<Texture2D>& sourceImage, std::shared_ptr<TextureCube>& result);

		// Creates a filepath to load a specific mip level from a pre-filtered texture base filepath
		static std::string GeneratePrefilteredTextureFilepath(const std::string& baseFilepath, const uint32_t mipLevel);
	private:
		std::shared_ptr<Texture2D> PrepareTexture2D(byte* source, const TextureData& data);
		byte* AddAlphaChannel(byte* source, TextureData& data);
	private:
		std::string m_Filepath;
		uint32_t m_FileChannels;
		bool m_FlipOnLoad;
	};

}