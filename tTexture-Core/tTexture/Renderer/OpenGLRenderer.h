#pragma once

#include "Core/Core.h"

#include "Renderer/OpenGLContext.h"
#include "Renderer/OpenGLBuffer.h"
#include "Renderer/OpenGLShader.h"
#include "Renderer/OpenGLTexture.h"
#include "Renderer/OpenGLFramebuffer.h"

#include <optional>
#include <glm/glm.hpp>

namespace tTexture {

	class OpenGLRenderer
	{
	public:
		OpenGLRenderer();
		~OpenGLRenderer();

		std::shared_ptr<TextureCube> RenderEquirectangularTexture(const std::shared_ptr<Texture2D>& source);
		std::shared_ptr<TextureCube> CreateIrradianceMap(const std::shared_ptr<TextureCube>& source);

		std::pair<uint32_t, uint32_t> GetWindowSize() const { return m_Context->GetWindowSize(); }
	private:
		void Init();
		void Shutdown();
		void CreateCube();

		glm::mat4 GetEquirectagularView(uint32_t faceIndex);
		std::shared_ptr<TextureCube> ConvertResultToCubeMap(const std::shared_ptr<Renderer::OpenGLTextureCube>& source);
	private:
		uint32_t m_VertexArray;
		std::unique_ptr<Renderer::OpenGLContext> m_Context;

		std::unique_ptr<Renderer::OpenGLVertexBuffer> m_CubeVertexBuffer;
		std::unique_ptr<Renderer::OpenGLIndexBuffer> m_CubeIndexBuffer;
		std::unique_ptr<Renderer::OpenGLFramebuffer> m_Framebuffer;

		glm::mat4 m_CaptureProjection;
		const float m_Exposure, m_OutputAlpha;
		const uint32_t m_TextureSlot;
	};

}