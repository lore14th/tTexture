#pragma once

#include "Core/NativeLoader/NativeLoader.h"
#include "Renderer/OpenGLRenderer.h"

namespace tTexture::Native {

	class Application;

	class EquirectangularLoader : public NativeLoader
	{
	public:
		EquirectangularLoader(const std::unique_ptr<OpenGLRenderer>& renderer);
		~EquirectangularLoader() = default;

		virtual void ConvertToCubeMap(const Texture2D& sourceImage, TextureCube& result) const override;
	private:
		const std::unique_ptr<OpenGLRenderer>& m_Renderer;
	};

}