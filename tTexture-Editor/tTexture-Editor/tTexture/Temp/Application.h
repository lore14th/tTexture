#pragma once

#include "Core/Log.h"

#include <optional>

namespace tTexture {

	class OpenGLRenderer;
	class OnlineApplication;
	class OfflineApplication;

	class Application
	{
	public:
		Application(Log::LogLevel logLevel);
		virtual ~Application();

		static std::unique_ptr<OnlineApplication> CreateOnlineApplication(Log::LogLevel logLevel);
		static std::unique_ptr<OfflineApplication> CreateOfflineApplication(Log::LogLevel logLevel);
	private:
		static uint32_t s_ApplicationCount;
		static Log* s_Logger;
	};

	class OnlineApplication : public Application
	{
	public:
		OnlineApplication(Log::LogLevel logLevel);
		virtual ~OnlineApplication() = default;

		std::shared_ptr<Texture2D> LoadTexture2D(const char* filepath, uint32_t imageChannels, bool flipOnLoad = false);
		std::shared_ptr<TextureCube> LoadTextureCube(const char* filepath, uint32_t imageChannels, CubeFormat format, bool flipOnLoad = false);
		std::shared_ptr<PrefilteredTextureCube> LoadPrefilteredTexture(const char* filepath, uint32_t imageChannels, uint32_t mipCount, CubeFormat format = CubeFormat::HCROSS, bool flipOnLoad = false);
	};

	class OfflineApplication : public Application
	{
	public:
		OfflineApplication(Log::LogLevel logLevel);
		virtual ~OfflineApplication() = default;

		std::shared_ptr<Texture2D> LoadTexture2D(const char* filepath, uint32_t imageChannels, bool flipOnLoad = false);
		std::shared_ptr<TextureCube> LoadTextureCube(const char* filepath, uint32_t imageChannels, CubeFormat format, bool flipOnLoad = false);
		std::shared_ptr<PrefilteredTextureCube> LoadPrefilteredTexture(const char* filepath, uint32_t imageChannels, uint32_t mipCount, CubeFormat format = CubeFormat::HCROSS, bool flipOnLoad = false);

		std::shared_ptr<TextureCube> CreateIrradiance(const char* filepath, uint32_t imageChannels, CubeFormat format, bool flipOnLoad = false);
		std::shared_ptr<TextureCube> CreateIrradiance(const std::shared_ptr<TextureCube>& sourceTexture);

		std::shared_ptr<tTexture::PrefilteredTextureCube> PrefilterEnvironmentMap(const char* filepath, uint32_t imageChannels, CubeFormat format, bool flipOnLoad = false);
		std::shared_ptr<tTexture::PrefilteredTextureCube> PrefilterEnvironmentMap(const std::shared_ptr<tTexture::TextureCube>& equirectangular) const;

		std::shared_ptr<Texture2D> CreateBRDF(uint32_t size);

		void ExportTexture(const char* outputFilepath, const std::shared_ptr<Texture2D>& texture);
		void ExportTexture(const char* outputFilepath, const std::shared_ptr<TextureCube>& texture);
		void ExportTexture(const char* outputFilepath, const std::shared_ptr<PrefilteredTextureCube>& texture);

		// Set the resolution for texture renderer.
		void SetRendererResolution(uint32_t resolution);		 
	private:
		std::unique_ptr<OpenGLRenderer> m_Renderer;

		friend class Loader; // Loader need to access GetRenderer()
	};

}