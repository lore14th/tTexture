#include "pch.h"
#include "Loader.h"
#include "Editor/Core-Editor/Renderer/OpenGLRenderer.h"

#include <tTexture/Core/CoreLoader.h>

namespace tTexture {

	VCrossLoader::VCrossLoader(const std::string& filepath)
		: m_Filepath(filepath)
	{
		TTEX_ASSERT(!m_Filepath.empty(), "Loader: You must provide a valid filepath");
	}

	std::shared_ptr<TextureCube> VCrossLoader::LoadVCrossFromFile() const
	{
		// Loads the image from file as a Texture2D
		tTexture::CoreLoader loader(m_Filepath, false);
		std::shared_ptr<Texture2D> sourceImage = loader.LoadImageFromFile(false);

		const uint32_t faceSize = sourceImage->Data.Width / 3;
		const uint32_t bpp = 4;

		TTEX_ASSERT(faceSize != 0, "Loader:Cubemap has invalid faceSize: {0}", faceSize);
		TTEX_ASSERT(faceSize == sourceImage->Data.Height / 4, "Loader:Cubemap faceSize error. Non square faces");

		std::shared_ptr<TextureCube> result = std::make_shared<TextureCube>(faceSize, bpp);
		result->AllocateTexture();

#ifdef TTEX_LOADER_SINGLE_THREAD
		CoreLoader::ReadSquareFace(Face::POS_X, sourceImage, result);
		CoreLoader::ReadSquareFace(Face::NEG_X, sourceImage, result);
		CoreLoader::ReadSquareFace(Face::POS_Y, sourceImage, result);
		CoreLoader::ReadSquareFace(Face::NEG_Y, sourceImage, result);
		CoreLoader::ReadSquareFace(Face::POS_Z, sourceImage, result);
		CoreLoader::ReadSquareFace(Face::NEG_Z, sourceImage, result);
#else
		std::array<std::thread*, 6> threads;

		threads[0] = new std::thread(CoreLoader::ReadSquareFace, Face::POS_X, CubeFormat::VCROSS, std::ref(sourceImage), std::ref(result));
		threads[1] = new std::thread(CoreLoader::ReadSquareFace, Face::NEG_X, CubeFormat::VCROSS, std::ref(sourceImage), std::ref(result));
		threads[2] = new std::thread(CoreLoader::ReadSquareFace, Face::POS_Y, CubeFormat::VCROSS, std::ref(sourceImage), std::ref(result));
		threads[3] = new std::thread(CoreLoader::ReadSquareFace, Face::NEG_Y, CubeFormat::VCROSS, std::ref(sourceImage), std::ref(result));
		threads[4] = new std::thread(CoreLoader::ReadSquareFace, Face::POS_Z, CubeFormat::VCROSS, std::ref(sourceImage), std::ref(result));
		threads[5] = new std::thread(CoreLoader::ReadSquareFace, Face::NEG_Z, CubeFormat::VCROSS, std::ref(sourceImage), std::ref(result));

		for (auto& t : threads)
			t->join();
#endif

		CoreLoader::FlipFaceVertically(Face::NEG_Z, result);
		return result;
	}

	// -- Equirectangular Loader --
	EquirectangularLoader::EquirectangularLoader(const std::string& filepath, const std::unique_ptr<OpenGLRenderer>& renderer)
		: m_Renderer(renderer), m_Filepath(filepath)
	{
		TTEX_ASSERT(m_Renderer, "EqirectangularLoader:Invalid Renderer!");
	}

	std::shared_ptr<TextureCube> EquirectangularLoader::LoadEquirectangularFromFile() const
	{
		tTexture::CoreLoader coreLoader(m_Filepath, false);
		std::shared_ptr<Texture2D> sourceTexture = coreLoader.LoadImageFromFile(false);

		return m_Renderer->RenderEquirectangularTexture(sourceTexture);
	}

}