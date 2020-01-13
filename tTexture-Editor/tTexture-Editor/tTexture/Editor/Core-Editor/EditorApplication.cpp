#include "pch.h"
#include "EditorApplication.h"

#include "Editor/Core-Editor/Loader/Loader.h"
#include "Editor/Core-Editor/Renderer/OpenGLRenderer.h"

#include <tTexture/tTexture-Core.h>
#include <tTexture/Core/CoreLoader.h> 
#include <tTexture/Core/CoreExporter.h>

namespace tTexture {

	EditorApplication::EditorApplication(tTexture::Debug::Log::LogLevel level)
	{
		m_Renderer = std::make_unique<OpenGLRenderer>();
		m_CoreLibrary = std::make_unique<tTexture::CoreApplication>(level);
	}

	std::shared_ptr<Texture2D> EditorApplication::LoadTexture2D(const char* filepath, bool addAlpha, bool flipOnLoad) const
	{
		return m_CoreLibrary->LoadTexture2D(filepath, addAlpha, flipOnLoad);
	}

	std::shared_ptr<TextureCube> EditorApplication::LoadTextureCube(const char* filepath, CubeFormat format) const
	{
		switch (format)
		{
			case CubeFormat::HCROSS: return LoadHCrossTextureCube(filepath);
			case CubeFormat::VCROSS: return LoadVCrossTextureCube(filepath);
			case CubeFormat::EQUIRECTANGULAR: return LoadEquirectangularTextureCube(filepath);
		}

		TTEX_ASSERT(false, "Invalid format");
		return std::make_shared<TextureCube>();
	}

	std::shared_ptr<PrefilteredTextureCube> EditorApplication::LoadPrefilteredTextureHCross(const char* baseFilepath)
	{
		return m_CoreLibrary->LoadPrefilteredHCrossFromFile(baseFilepath);
	}

	std::shared_ptr<tTexture::TextureCube> EditorApplication::CreateIrradiance(const char* filepath, CubeFormat format)
	{
		std::shared_ptr<TextureCube> texture = LoadTextureCube(filepath, format);
		return CreateIrradiance(texture);
	}

	std::shared_ptr<TextureCube> EditorApplication::CreateIrradiance(const std::shared_ptr<TextureCube>& sourceTexture)
	{
		TTEX_TIME_FUNCTION;
		return m_Renderer->CreateIrradianceMap(sourceTexture);
	}

	std::shared_ptr<Texture2D> EditorApplication::CreateBRDF(BRDFType type, uint32_t size)
	{
		TTEX_TIME_FUNCTION;
		return m_Renderer->CreateBRDF(type, size);
	}

	std::shared_ptr<PrefilteredTextureCube> EditorApplication::PrefilterEnvironmentMap(const std::shared_ptr<TextureCube>& sourceTexture) const
	{
		TTEX_TIME_FUNCTION;
		return m_Renderer->PrefilterEnvironmentMap(sourceTexture);
	}

	std::shared_ptr<PrefilteredTextureCube> EditorApplication::PrefilterEnvironmentMap(const char* filepath, CubeFormat format)
	{
		std::shared_ptr<TextureCube> texture = LoadTextureCube(filepath, format);
		return PrefilterEnvironmentMap(texture);
	}

	void EditorApplication::ExportTexture(const char* outputFilepath, const std::shared_ptr<Texture2D>& texture) const
	{
		TTEX_TIME_FUNCTION;
		CoreExporter exporter(outputFilepath);
		exporter.WriteToDisk(texture);
	}

	void EditorApplication::ExportTexture(const char* outputFilepath, const std::shared_ptr<TextureCube>& texture, CubeFormat outputFormat) const 
	{
		TTEX_TIME_FUNCTION;
		CoreExporter exporter(outputFilepath);
		exporter.WriteToDisk(texture, outputFormat);
	}

	void EditorApplication::ExportTexture(const char* outputFilepath, const std::shared_ptr<PrefilteredTextureCube>& texture) const
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

	std::shared_ptr<tTexture::TextureCube> EditorApplication::LoadHCrossTextureCube(const char* filepath) const
	{
		return m_CoreLibrary->LoadHCrossFromFile(filepath);
	}

	std::shared_ptr<tTexture::TextureCube> EditorApplication::LoadVCrossTextureCube(const char* filepath) const
	{
		TTEX_TIME_FUNCTION;
		VCrossLoader loader(filepath);
		return loader.LoadVCrossFromFile();
	}

	std::shared_ptr<tTexture::TextureCube> EditorApplication::LoadEquirectangularTextureCube(const char* filepath) const
	{
		TTEX_TIME_FUNCTION;
		EquirectangularLoader loader(filepath, m_Renderer);
		return loader.LoadEquirectangularFromFile();
	}

}
