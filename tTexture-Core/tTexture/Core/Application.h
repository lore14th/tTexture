#pragma once

#include "Core/Log.h"

#include <optional>

namespace tTexture {

	class OpenGLRenderer;

	class Application
	{
	public:
		Application(Log::LogLevel logLevel, bool onlineApplication = true);
		~Application();

		std::shared_ptr<Texture2D> LoadTexture2D(const char* filepath, uint32_t imageChannels, bool flipOnLoad = false);
		std::shared_ptr<TextureCube> LoadTextureCube(const char* filepath, uint32_t imageChannels, CubeFormat format, bool flipOnLoad = false);

		std::shared_ptr<TextureCube> CreateIrradiance(const char* filepath, uint32_t imageChannels, CubeFormat format, bool flipOnLoad = false);
		std::shared_ptr<TextureCube> CreateIrradiance(const std::shared_ptr<TextureCube> sourceTexture);

		std::shared_ptr<Texture2D> CreateBRDF(uint32_t size);

		void ExportTexture(const char* outputFilepath, const std::shared_ptr<Texture2D>& texture);
		void ExportTexture(const char* outputFilepath, const std::shared_ptr<TextureCube>& texture);

		// Set the resolution for texture renderer.
		void SetRendererResolution(uint32_t resolution);
	private:
		// Note: use this method (even inside this class) to get access to the Renderer.
		// this checks if the renderer exists and returns it
		const std::unique_ptr<OpenGLRenderer>& GetRenderer() const;
		 
	private:
		std::optional<std::unique_ptr<OpenGLRenderer>> m_Renderer;

		static uint32_t s_ApplicationCount;
		static Log* s_Logger;

		friend class Loader; // Loader need to access GetRenderer()
	};

}