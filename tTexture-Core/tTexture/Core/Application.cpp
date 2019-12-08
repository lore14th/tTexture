#include "pch.h"
#include "Application.h"
#include "Core/Loader.h"
#include "Core/Exporter.h"

#include "Renderer/OpenGLRenderer.h"

namespace tTexture {

	Application* Application::s_Instance = nullptr;

	Application::Application(bool onlineApplication)
		: m_OnlineApplication(onlineApplication)
	{
		TTEX_CORE_ASSERT(!s_Instance, "Application already exists");

		if(!onlineApplication)
			m_Renderer = std::make_unique<OpenGLRenderer>();

		s_Instance = this;
	}

	std::shared_ptr<Texture2D> Application::LoadTexture2D(const char* filepath, uint32_t imageChannels, bool flipOnLoad)
	{
		Loader loader(filepath, imageChannels, flipOnLoad);
		loader.SetApplicationCallback(this);

		return loader.LoadImageFromFile();
	}

	std::shared_ptr<TextureCube> Application::LoadTextureCube(const char* filepath, uint32_t imageChannels, CubeFormat format, bool flipOnLoad)
	{
		if (m_OnlineApplication && format == CubeFormat::EQUIRECTANGULAR)
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

}