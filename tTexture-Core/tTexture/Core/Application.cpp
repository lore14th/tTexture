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

	void Application::LoadTexture2D(const char* filepath, uint32_t imageChannels, Texture2D& result, bool flipOnLoad)
	{
		Loader loader(filepath, imageChannels, flipOnLoad);
		loader.SetApplicationCallback(this);

		loader.LoadImageFromFile(result);
	}

	void Application::LoadTextureCube(const char* filepath, uint32_t imageChannels, CubeFormat format, TextureCube& result, bool flipOnLoad)
	{
		if (m_OnlineApplication && format == CubeFormat::EQUIRECTANGULAR)
		{
			TTEX_CORE_ERROR("tTexture cannot load Equirectangular texture using an online application.\n\
							Please use the offline application to convert the image and store the result to disk");
			TTEX_CORE_ASSERT(false, "");
		}
		else
		{
			Loader loader(filepath, imageChannels, flipOnLoad);
			loader.SetApplicationCallback(this);

			loader.LoadCubeMapFromFile(format, result);
		}
	}

	void Application::ExportTexture(const char* outputFilepath, const Texture2D& texture)
	{
		Exporter exporter(outputFilepath);
		exporter.WriteToDisk(texture);
	}

	void Application::ExportTexture(const char* outputFilepath, const TextureCube& texture)
	{
		Exporter exporter(outputFilepath);
		exporter.WriteToDisk(texture);
	}

}