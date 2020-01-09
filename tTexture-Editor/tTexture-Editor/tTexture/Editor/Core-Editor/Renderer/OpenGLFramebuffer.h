#pragma once

#include "Core/tTextureCommon.h"
#include "Editor/Core-Editor/Renderer/OpenGLTexture.h"

#include <optional>

namespace tTexture::Renderer {

	class OpenGLFramebuffer
	{
	public:
		OpenGLFramebuffer();
		~OpenGLFramebuffer();

		void BindRenderTarget(const std::shared_ptr<OpenGLTexture2D>& targetTexture, uint32_t mipLevel);
		void BindAndRenderToCubeFace(const std::shared_ptr<OpenGLTextureCube>& targetTexture, uint32_t faceIndex, uint32_t mipLevel);

		uint32_t GetRendererID() const { return m_RendererID; }
	private:
		void CheckCompleteness() const;
	private:
		uint32_t m_RendererID;
	};

}
