#pragma once

namespace tTexture {

	class OpenGLRenderer;

	class Application
	{
	public:
		Application(bool onlineApplication = true);
		~Application() = default;

		void LoadTexture2D(const char* filepath, uint32_t imageChannels, Texture2D& result, bool flipOnLoad = false);
		void LoadTextureCube(const char* filepath, uint32_t imageChannels, CubeFormat format, TextureCube& result, bool flipOnLoad = false);

		void ExportTexture(const char* outputFilepath, const Texture2D& texture);
		void ExportTexture(const char* outputFilepath, const TextureCube& texture);
		
		static inline Application& Get() { return *s_Instance; }
	private:
		OpenGLRenderer& GetRenderer() { return *m_Renderer; }
 
	private:
		std::unique_ptr<OpenGLRenderer> m_Renderer;
		bool m_OnlineApplication;

		static Application* s_Instance;

		friend class Loader; // Loader need to access m_Renderer
	};

}