#include "pch.h"
#include "EditorApplication.h"

#include "Editor/Loader/Loader.h"
#include "Editor/Exporter/Exporter.h"
#include "Editor/Renderer/OpenGLRenderer.h"

#include <tTexture/tTexture-Core.h>
#include <tTexture/Core/CoreLoader.h> 

namespace tTexture {

	EditorApplication::EditorApplication()
	{
		auto logLevel = tTexture::Debug::Log::LogLevel::Trace;

		m_Renderer = std::make_unique<OpenGLRenderer>();
		m_CoreLibrary = std::make_unique<tTexture::CoreApplication>(logLevel);
	}

	std::shared_ptr<Texture2D> EditorApplication::LoadTexture2D(const char* filepath, uint32_t fileChannels, bool flipOnLoad)
	{
		return m_CoreLibrary->LoadTexture2D(filepath, fileChannels, flipOnLoad);
	}

	std::shared_ptr<TextureCube> EditorApplication::LoadTextureCube(const char* filepath, uint32_t fileChannels, CubeFormat format, bool flipOnLoad)
	{
		switch (format)
		{
			case CubeFormat::HCROSS: return LoadHCrossTextureCube(filepath, fileChannels, flipOnLoad);
			case CubeFormat::VCROSS: return LoadVCrossTextureCube(filepath, fileChannels, flipOnLoad);
			case CubeFormat::EQUIRECTANGULAR: return LoadEquirectangularTextureCube(filepath, fileChannels, flipOnLoad);
		}

		TTEX_ASSERT(false, "Invalid format");
		return std::make_shared<TextureCube>();
	}

	std::shared_ptr<PrefilteredTextureCube> EditorApplication::LoadPrefilteredTextureHCross(const char* filepath, uint32_t fileChannels, uint32_t mipCount, bool flipOnLoad)
	{
		return m_CoreLibrary->LoadPrefilteredHCrossFromFile(filepath, fileChannels, mipCount, flipOnLoad);
	}

	std::shared_ptr<tTexture::TextureCube> EditorApplication::CreateIrradiance(const char* filepath, uint32_t fileChannels, CubeFormat format, bool flipOnLoad)
	{
		std::shared_ptr<TextureCube> texture = LoadTextureCube(filepath, fileChannels, format, flipOnLoad);
		return CreateIrradiance(texture);
	}

	std::shared_ptr<TextureCube> EditorApplication::CreateIrradiance(const std::shared_ptr<TextureCube>& sourceTexture)
	{
		TTEX_TIME_FUNCTION;
		return m_Renderer->CreateIrradianceMap(sourceTexture);
	}

	std::shared_ptr<Texture2D> EditorApplication::CreateBRDF(uint32_t size)
	{
		TTEX_TIME_FUNCTION;
		return m_Renderer->CreateBRDF(size);
	}

	std::shared_ptr<PrefilteredTextureCube> EditorApplication::PrefilterEnvironmentMap(const std::shared_ptr<TextureCube>& equirectangular) const
	{
		TTEX_TIME_FUNCTION;
		return m_Renderer->PrefilterEnvironmentMap(equirectangular);
	}

	std::shared_ptr<PrefilteredTextureCube> EditorApplication::PrefilterEnvironmentMap(const char* filepath, uint32_t fileChannels, CubeFormat format, bool flipOnLoad)
	{
		std::shared_ptr<TextureCube> texture = LoadTextureCube(filepath, fileChannels, format, flipOnLoad);
		return PrefilterEnvironmentMap(texture);
	}

	void EditorApplication::ExportTexture(const char* outputFilepath, const std::shared_ptr<Texture2D>& texture)
	{
		TTEX_TIME_FUNCTION;
		Exporter exporter(outputFilepath);
		exporter.WriteToDisk(texture);
	}

	void EditorApplication::ExportTexture(const char* outputFilepath, const std::shared_ptr<TextureCube>& texture)
	{
		TTEX_TIME_FUNCTION;
		Exporter exporter(outputFilepath);
		exporter.WriteToDisk(texture);
	}

	void EditorApplication::ExportTexture(const char* outputFilepath, const std::shared_ptr<PrefilteredTextureCube>& texture)
	{
		TTEX_TIME_FUNCTION;
		for (uint32_t mipLevel = 0; mipLevel < texture->GetLevelsCount(); mipLevel++)
		{
			std::string mipFilepath = tTexture::CoreLoader::GeneratePrefilteredTextureFilepath(outputFilepath, mipLevel);
			ExportTexture(mipFilepath.c_str(), texture->GetLevel(mipLevel));
		}
	}

	void EditorApplication::SetRendererResolution(uint32_t resolution)
	{
		TTEX_TIME_FUNCTION;
		m_Renderer->SetRendererResolution(resolution);
	}

	std::shared_ptr<tTexture::TextureCube> EditorApplication::LoadHCrossTextureCube(const char* filepath, uint32_t fileChannels, bool flipOnLoad)
	{
		return m_CoreLibrary->LoadHCrossFromFile(filepath, fileChannels, flipOnLoad);
	}

	std::shared_ptr<tTexture::TextureCube> EditorApplication::LoadVCrossTextureCube(const char* filepath, uint32_t fileChannels, bool flipOnLoad)
	{
		TTEX_TIME_FUNCTION;
		VCrossLoader loader(filepath, fileChannels, flipOnLoad);
		return loader.LoadVCrossFromFile();
	}

	std::shared_ptr<tTexture::TextureCube> EditorApplication::LoadEquirectangularTextureCube(const char* filepath, uint32_t fileChannels, bool flipOnLoad)
	{
		TTEX_TIME_FUNCTION;
		EquirectangularLoader loader(filepath, fileChannels, flipOnLoad, m_Renderer);
		return loader.LoadEquirectangularFromFile();
	}

}
