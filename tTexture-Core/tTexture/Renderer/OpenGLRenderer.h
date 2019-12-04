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

		void RenderEquirectangularTexture(const Texture2D& source, TextureCube& result);

		void Init();
		void Shutdown();

		std::pair<uint32_t, uint32_t> GetWindowSize() const { return m_Context->GetWindowSize(); }

	private:
		void CreateCube();
		void ConvertAndSetTexture(const Texture2D& texture);

		void GetEquirectagularView(uint32_t faceIndex, glm::mat4& view);

		void ConvertResultToCubeMap(const std::shared_ptr<Renderer::OpenGLTextureCube>& source, TextureCube& result);

	private:
		uint32_t m_VertexArray;
		std::unique_ptr<Renderer::OpenGLContext> m_Context;

		std::unique_ptr<Renderer::OpenGLShader> m_Shader;
		std::unique_ptr<Renderer::OpenGLVertexBuffer> m_CubeVertexBuffer;
		std::unique_ptr<Renderer::OpenGLIndexBuffer> m_CubeIndexBuffer;
		std::unique_ptr<Renderer::OpenGLFramebuffer> m_Framebuffer;

		std::optional<std::unique_ptr<Renderer::OpenGLTexture2D>> m_SourceTexture;
		std::optional<std::unique_ptr<Renderer::OpenGLTextureCube>> m_SourceTextureCube;

		glm::mat4 m_CaptureProjection;
		const uint16_t m_FaceIndices[6] = { 1, 0, 2, 3, 4, 5 };
	};

}