#include "pch.h"
#include "Application.h"
#include "Core/Loader.h"
#include "Core/Exporter.h"

#include "Renderer/OpenGLRenderer.h"

namespace tTexture {

	Log* Application::s_Logger = nullptr;
	uint32_t Application::s_ApplicationCount = 0;

	Application::Application(Log::LogLevel logLevel, bool onlineApplication)
	{
		if (!s_Logger)
			s_Logger->Init(logLevel);

		if (!onlineApplication)
			m_Renderer = std::make_optional(std::make_unique<OpenGLRenderer>());

		s_ApplicationCount++;
	}

	Application::~Application()
	{
		s_ApplicationCount--;

		if (s_ApplicationCount == 0)
			delete s_Logger;
	}

	std::shared_ptr<Texture2D> Application::LoadTexture2D(const char* filepath, uint32_t imageChannels, bool flipOnLoad)
	{
		Loader loader(filepath, imageChannels, flipOnLoad);
		loader.SetApplicationCallback(this);

		return loader.LoadImageFromFile();
	}

	std::shared_ptr<TextureCube> Application::LoadTextureCube(const char* filepath, uint32_t imageChannels, CubeFormat format, bool flipOnLoad)
	{
		if (!m_Renderer.has_value() && format == CubeFormat::EQUIRECTANGULAR)
		{
			TTEX_CORE_ERROR("tTexture cannot load Equirectangular texture using an online application.\n\
							Please use the offline application to convert the image and store the result to disk");
			TTEX_CORE_ASSERT(false, "");

			return std::make_shared<TextureCube>();
		}
		else
		{
			Loader loader(filepath, imageChannels, flipOnLoad);
			loader.SetApplicationCallback(this);

			return loader.LoadCubeMapFromFile(format);
		}
	}

	std::shared_ptr<tTexture::TextureCube> Application::CreateIrradiance(const char* filepath, uint32_t imageChannels, CubeFormat format, bool flipOnLoad)
	{
		Loader loader(filepath, imageChannels, flipOnLoad);
		loader.SetApplicationCallback(this);
		std::shared_ptr<TextureCube> texture = loader.LoadCubeMapFromFile(format);

		return CreateIrradiance(texture);
	}

	std::shared_ptr<tTexture::TextureCube> Application::CreateIrradiance(const std::shared_ptr<TextureCube> sourceTexture)
	{
		auto& renderer = GetRenderer();
		return renderer->CreateIrradianceMap(sourceTexture);
	}

	std::shared_ptr<tTexture::Texture2D> Application::CreateBRDF(uint32_t size)
	{
		auto& renderer = GetRenderer();
		return renderer->CreateBRDF(size);
	}

	void Application::ExportTexture(const char* outputFilepath, const std::shared_ptr<Texture2D>& texture)
	{
		Exporter exporter(outputFilepath);
		exporter.WriteToDisk(texture);
	}

	void Application::ExportTexture(const char* outputFilepath, const std::shared_ptr<TextureCube>& texture)
	{
		Exporter exporter(outputFilepath);
		exporter.WriteToDisk(texture);
	}

	void Application::SetRendererResolution(uint32_t resolution)
	{	
		if (m_Renderer.has_value())
			m_Renderer.value()->SetRendererResolution(resolution);
		else
			TTEX_CORE_WARN("Application cannot set Renderer resolution");
	}

	const std::unique_ptr<tTexture::OpenGLRenderer>& Application::GetRenderer() const
	{
		TTEX_CORE_ASSERT(m_Renderer.has_value(), "Application:No renderer instance. Make sure you are running offline mode");
		return m_Renderer.value();
	}

}