#include "pch.h"
#include "EquirectangularLoader.h"

#ifndef TTEX_LOADER_SINGLE_THREAD
#include <thread>
#endif

namespace tTexture::Native {

	EquirectangularLoader::EquirectangularLoader(const std::unique_ptr<OpenGLRenderer>& renderer)
		: m_Renderer(renderer)
	{
		TTEX_CORE_ASSERT(m_Renderer, "EqirectangularLoader:Invalid Renderer!");
	}

	void EquirectangularLoader::ConvertToCubeMap(const Texture2D& sourceImage, TextureCube& result) const
	{
		m_Renderer->RenderEquirectangularTexture(sourceImage, result);
	}


}
