#include "pch.h"
#include "Application.h"
#include "Core/Loader.h"
#include "Core/Exporter.h"

#include "Renderer/OpenGLRenderer.h"

namespace tTexture {

	Log* Application::s_Logger = nullptr;
	uint32_t Application::s_ApplicationCount = 0;

	Application::Application(Log::LogLevel logLevel)
	{
		s_ApplicationCount++;

		if (!s_Logger)
			s_Logger->Init(logLevel);
	}

	Application::~Application()
	{
		s_ApplicationCount--;

		if (s_ApplicationCount == 0)
			delete s_Logger;
	}

	std::unique_ptr<tTexture::OnlineApplication> Application::CreateOnlineApplication(Log::LogLevel logLevel)
	{
		return std::make_unique<tTexture::OnlineApplication>(logLevel);
	}

	std::unique_ptr<tTexture::OfflineApplication> Application::CreateOfflineApplication(Log::LogLevel logLevel)
	{
		return std::make_unique<tTexture::OfflineApplication>(logLevel);
	}

	OnlineApplication::OnlineApplication(Log::LogLevel logLevel)
		: Application(logLevel)
	{
	}

	std::shared_ptr<tTexture::Texture2D> OnlineApplication::LoadTexture2D(const char* filepath, uint32_t imageChannels, bool flipOnLoad)
	{
		Loader loader(filepath, imageChannels, flipOnLoad);
		return loader.LoadImageFromFile();
	}

	std::shared_ptr<tTexture::TextureCube> OnlineApplication::LoadTextureCube(const char* filepath, uint32_t imageChannels, CubeFormat format, bool flipOnLoad)
	{
		if (format == CubeFormat::EQUIRECTANGULAR)
		{
			TTEX_CORE_ERROR("tTexture cannot load Equirectangular texture using an online application.\n\
							Please use the offline application to convert the image and store the result to disk");
			TTEX_CORE_ASSERT(false, "");
		}
		else
		{
			Loader loader(filepath, imageChannels, flipOnLoad);
			return loader.LoadCubeMapFromFile(format);
		}
	}

	// -- Offline Application --------------------------------

	OfflineApplication::OfflineApplication(Log::LogLevel logLevel)
		: Application(logLevel)
	{
		m_Renderer = std::make_unique<OpenGLRenderer>();
	}

	std::shared_ptr<Texture2D> OfflineApplication::LoadTexture2D(const char* filepath, uint32_t imageChannels, bool flipOnLoad)
	{
		Loader loader(filepath, imageChannels, flipOnLoad);
		loader.SetApplicationCallback(this);

		return loader.LoadImageFromFile();
	}

	std::shared_ptr<TextureCube> OfflineApplication::LoadTextureCube(const char* filepath, uint32_t imageChannels, CubeFormat format, bool flipOnLoad)
	{
		Loader loader(filepath, imageChannels, flipOnLoad);
		loader.SetApplicationCallback(this);

		return loader.LoadCubeMapFromFile(format);
	}

	std::shared_ptr<tTexture::TextureCube> OfflineApplication::CreateIrradiance(const char* filepath, uint32_t imageChannels, CubeFormat format, bool flipOnLoad)
	{
		Loader loader(filepath, imageChannels, flipOnLoad);
		loader.SetApplicationCallback(this);
		std::shared_ptr<TextureCube> texture = loader.LoadCubeMapFromFile(format);

		return CreateIrradiance(texture);
	}

	std::shared_ptr<tTexture::TextureCube> OfflineApplication::CreateIrradiance(const std::shared_ptr<TextureCube> sourceTexture)
	{
		return m_Renderer->CreateIrradianceMap(sourceTexture);
	}

	std::shared_ptr<tTexture::Texture2D> OfflineApplication::CreateBRDF(uint32_t size)
	{
		return m_Renderer->CreateBRDF(size);
	}

	void OfflineApplication::ExportTexture(const char* outputFilepath, const std::shared_ptr<Texture2D>& texture)
	{
		Exporter exporter(outputFilepath);
		exporter.WriteToDisk(texture);
	}

	void OfflineApplication::ExportTexture(const char* outputFilepath, const std::shared_ptr<TextureCube>& texture)
	{
		Exporter exporter(outputFilepath);
		exporter.WriteToDisk(texture);
	}

	void OfflineApplication::SetRendererResolution(uint32_t resolution)
	{	
		m_Renderer->SetRendererResolution(resolution);
	}

}
