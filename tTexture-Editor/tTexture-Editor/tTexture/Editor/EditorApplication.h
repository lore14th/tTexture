#pragma once

#include <tTexture/tTexture-Core.h>

namespace tTexture {

	class OpenGLRenderer;

	class EditorApplication
	{
	public:
		EditorApplication();
		virtual ~EditorApplication() = default;

		std::shared_ptr<Texture2D> LoadTexture2D(const char* filepath, uint32_t fileChannels, bool flipOnLoad = false);
		std::shared_ptr<TextureCube> LoadTextureCube(const char* filepath, uint32_t fileChannels, CubeFormat format, bool flipOnLoad = false);
		std::shared_ptr<PrefilteredTextureCube> LoadPrefilteredTextureHCross(const char* filepath, uint32_t fileChannels, uint32_t mipCount, bool flipOnLoad = false);

		std::shared_ptr<TextureCube> CreateIrradiance(const char* filepath, uint32_t fileChannels, CubeFormat format, bool flipOnLoad = false);
		std::shared_ptr<TextureCube> CreateIrradiance(const std::shared_ptr<TextureCube>& sourceTexture);

		std::shared_ptr<PrefilteredTextureCube> PrefilterEnvironmentMap(const char* filepath, uint32_t fileChannels, CubeFormat format, bool flipOnLoad = false);
		std::shared_ptr<PrefilteredTextureCube> PrefilterEnvironmentMap(const std::shared_ptr<TextureCube>& equirectangular) const;

		std::shared_ptr<Texture2D> CreateBRDF(uint32_t size);

		void ExportTexture(const char* outputFilepath, const std::shared_ptr<Texture2D>& texture);
		void ExportTexture(const char* outputFilepath, const std::shared_ptr<TextureCube>& texture);
		void ExportTexture(const char* outputFilepath, const std::shared_ptr<PrefilteredTextureCube>& texture);

		// Set the resolution for texture renderer.
		void SetRendererResolution(uint32_t resolution);
	private:
		std::shared_ptr<TextureCube> LoadHCrossTextureCube(const char* filepath, uint32_t fileChannels, bool flipOnLoad);
		std::shared_ptr<TextureCube> LoadVCrossTextureCube(const char* filepath, uint32_t fileChannels, bool flipOnLoad);
		std::shared_ptr<TextureCube> LoadEquirectangularTextureCube(const char* filepath, uint32_t fileChannels, bool flipOnLoad);
	private:
		std::unique_ptr<OpenGLRenderer> m_Renderer;
		std::unique_ptr<tTexture::CoreApplication> m_CoreLibrary;
	};

}
