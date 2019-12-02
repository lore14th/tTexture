#include "pch.h"
#include "Application.h"
#include "Core/Loader.h"
#include "Core/Exporter.h"

#include "Renderer/OpenGLRenderer.h"

namespace tTexture {

	Application::Application()
	{
		m_Renderer = std::make_unique<OpenGLRenderer>();
	}

	void Application::LoadTexture2D(const char* filepath, uint32_t imageChannels, Texture2D& result, bool flipOnLoad)
	{
		Loader loader(filepath, imageChannels, flipOnLoad);
		loader.LoadImageFromFile(result);
	}

	void Application::LoadTextureCube(const char* filepath, uint32_t imageChannels, CubeFormat format, TextureCube& result, bool flipOnLoad)
	{
		Loader loader(filepath, imageChannels, flipOnLoad);
		loader.LoadCubeMapFromFile(format, result);
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