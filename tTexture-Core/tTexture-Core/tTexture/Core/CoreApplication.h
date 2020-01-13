#pragma once

#include "Debug/tTextureLog.h"

namespace tTexture {

	class CoreApplication
	{
	public:
		CoreApplication(Debug::Log::LogLevel logLevel = Debug::Log::LogLevel::None);
		virtual ~CoreApplication();

		// Loads a texture 2D from file. This returns RGB/RGBA texture. If you want to load a RGB texture as RGBA, set addAlpha to true
		// * filepath:		path to the actual file on disk.
		// * add alpha:		if you want to add the alpha channel to an rgb texture. By default is false.
		// * flipOnLoad:	if you want to flip the texture vertically. By default is false.
		std::shared_ptr<Texture2D> LoadTexture2D(const char* filepath, bool addAlpha = false, bool flipOnLoad = false);

		// Loads a texture cube(HCross) from file. This returns a RGBA texture.
		// * filepath:		path to the actual file on disk.
		std::shared_ptr<TextureCube> LoadHCrossFromFile(const char* filepath);

		// Loads all the mip maps from a texture cube(HCross). All mips are loaded as RGBA textures.
		// * baseFilepath:	path to the actual file on disk. Note that you should not inset the mip number in the filepath. (ex. texture.png -> { texture-0.png, texture-1.png ... })
		std::shared_ptr<PrefilteredTextureCube> LoadPrefilteredHCrossFromFile(const char* baseFilepath);

		// Export Texture2D to file
		// * outputFilepath:	filepath used to store the texture on disk
		// * texture:			texture to store
		void ExportTexture(const char* outputFilepath, const std::shared_ptr<Texture2D>& texture) const;
		// Export TextureCube to file
		// * outputFilepath:	filepath used to store the texture on disk
		// * texture:			texture to store
		// * outputFormat:		cube format of the output texture. [TODO] add VCross and Equirectangular
		void ExportTexture(const char* outputFilepath, const std::shared_ptr<TextureCube>& texture, CubeFormat outputFormat = CubeFormat::HCROSS) const;
		// Export PrefilteredTexture to file
		// * outputFilepath:	filepath used to store the texture on disk
		// * texture:			texture to store
		void ExportTexture(const char* outputFilepath, const std::shared_ptr<PrefilteredTextureCube>& texture) const;

#ifdef TTEX_APP
	private:
		static uint32_t s_ApplicationCount;
		static Debug::Log* s_Logger;
#endif
	};

}
