#pragma once

#include <tTexture/tTexture-Core.h>

namespace tTexture {

	class OpenGLRenderer;
	enum class BRDFType;

	class EditorApplication
	{		
	public:
		EditorApplication(tTexture::Debug::Log::LogLevel level = Debug::Log::LogLevel::Trace);
		virtual ~EditorApplication() = default;

		// Loads a texture 2D from file. This returns RGB/RGBA texture. If you want to load a RGB texture as RGBA, set addAlpha to true
		// * filepath:		path to the actual file on disk.
		// * add alpha:		if you want to add the alpha channel to an rgb texture. By default is false.
		// * flipOnLoad:	if you want to flip the texture vertically. By default is false.
		std::shared_ptr<Texture2D> LoadTexture2D(const char* filepath, bool addAlpha = false, bool flipOnLoad = false) const;
		// Loads a texture cube(HCross) from file. This returns a RGBA texture.
		// * filepath:		path to the actual file on disk.
		// * format:		cube format of the source image.
		std::shared_ptr<TextureCube> LoadTextureCube(const char* filepath, CubeFormat format) const;
		// Loads all the mip maps from a texture cube(HCross). All mips are loaded as RGBA textures.
		// * filepath:		path to the actual file on disk.
		std::shared_ptr<PrefilteredTextureCube> LoadPrefilteredTextureHCross(const char* filepath);

		// Creates Irradiance Map from TextureCube
		// * filepath:		path to the actual file on disk.
		// * format:		cube format of the source image.
		std::shared_ptr<TextureCube> CreateIrradiance(const char* filepath, CubeFormat format);
		// Creates Irradiance Map from TextureCube
		// * sourceTexture:	textureCube used to create the irradiance map
		std::shared_ptr<TextureCube> CreateIrradiance(const std::shared_ptr<TextureCube>& sourceTexture);

		// Prefilter a TextureCube
		// * filepath:		path to the actual file on disk.
		// * format:		cube format of the source image.
		std::shared_ptr<PrefilteredTextureCube> PrefilterEnvironmentMap(const char* filepath, CubeFormat format);
		// Prefilter a TextureCube
		// * sourceTexture:	textureCube used to pre-filter
		std::shared_ptr<PrefilteredTextureCube> PrefilterEnvironmentMap(const std::shared_ptr<TextureCube>& sourceTexture) const;

		// Create a BRDF Texture Lut
		// * type:			type of BRDF function 
		// * size:			size of the output texture
		std::shared_ptr<Texture2D> CreateBRDF(BRDFType type, uint32_t size);

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

		// Set the resolution for texture renderer.
		// * resolution:		size of the cube face of the output texture
		void SetRendererResolution(uint32_t resolution);
	private:
		std::shared_ptr<TextureCube> LoadHCrossTextureCube(const char* filepath) const;
		std::shared_ptr<TextureCube> LoadVCrossTextureCube(const char* filepath) const;
		std::shared_ptr<TextureCube> LoadEquirectangularTextureCube(const char* filepath) const;
	private:
		std::unique_ptr<OpenGLRenderer> m_Renderer;
		std::unique_ptr<tTexture::CoreApplication> m_CoreLibrary;
	};

}
