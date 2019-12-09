#pragma once

#include <optional>

namespace tTexture {

	class OpenGLRenderer;

	class Application
	{
	public:
		Application(bool onlineApplication = true);
		~Application() = default;

		std::shared_ptr<Texture2D> LoadTexture2D(const char* filepath, uint32_t imageChannels, bool flipOnLoad = false);
		std::shared_ptr<TextureCube> LoadTextureCube(const char* filepath, uint32_t imageChannels, CubeFormat format, bool flipOnLoad = false);

		std::shared_ptr<TextureCube> CreateIrradiance(const char* filepath, uint32_t imageChannels, CubeFormat format, bool flipOnLoad = false);
		std::shared_ptr<TextureCube> CreateIrradiance(const std::shared_ptr<TextureCube> sourceTexture);

		void ExportTexture(const char* outputFilepath, const std::shared_ptr<Texture2D>& texture);
		void ExportTexture(const char* outputFilepath, const std::shared_ptr<TextureCube>& texture);

	private:
		const std::unique_ptr<OpenGLRenderer>& GetRenderer() const;
		 
	private:
		std::optional<std::unique_ptr<OpenGLRenderer>> m_Renderer;
		bool m_OnlineApplication;

		friend class Loader; // Loader need to access m_Renderer
	};

}