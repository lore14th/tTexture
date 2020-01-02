#pragma once

#include "Editor/Core-Editor/Renderer/OpenGLContext.h"
#include "Editor/Core-Editor/Renderer/OpenGLBuffer.h"
#include "Editor/Core-Editor/Renderer/OpenGLShader.h"
#include "Editor/Core-Editor/Renderer/OpenGLTexture.h"
#include "Editor/Core-Editor/Renderer/OpenGLFramebuffer.h"

#include <optional>
#include <glm/glm.hpp>

#include <tTexture/tTexture-Core.h>

namespace tTexture {

	class OpenGLRenderer
	{
	public:
		OpenGLRenderer();
		~OpenGLRenderer();

		std::shared_ptr<TextureCube> RenderEquirectangularTexture(const std::shared_ptr<Texture2D>& source) const;
		std::shared_ptr<TextureCube> CreateIrradianceMap(const std::shared_ptr<TextureCube>& source) const;
		std::shared_ptr<tTexture::PrefilteredTextureCube> PrefilterEnvironmentMap(const std::shared_ptr<tTexture::TextureCube>& source) const;

		std::shared_ptr<Texture2D> CreateBRDF(uint32_t size);

		std::pair<uint32_t, uint32_t> GetWindowSize() const { return m_Context->GetWindowSize(); }

		void SetRendererResolution(uint32_t resolution) { m_Resolution = resolution; }
	private:
		void Init();
		void Shutdown();

		void CreateCube();
		void CreateSquare();

		glm::mat4 GetEquirectagularView(uint32_t faceIndex) const;
		glm::mat4 GetIrradianceView(uint32_t faceIndex) const;
		glm::mat4 GetPrefilterView(uint32_t faceIndex) const;
	private:
		uint32_t m_VertexArray;
		uint32_t m_Resolution;
		std::unique_ptr<Renderer::OpenGLContext> m_Context;
		std::unique_ptr<Renderer::OpenGLFramebuffer> m_Framebuffer;

		std::unique_ptr<Renderer::OpenGLVertexBuffer> m_SquareVertexBuffer;
		std::unique_ptr<Renderer::OpenGLIndexBuffer> m_SquareIndexBuffer;

		std::unique_ptr<Renderer::OpenGLVertexBuffer> m_CubeVertexBuffer;
		std::unique_ptr<Renderer::OpenGLIndexBuffer> m_CubeIndexBuffer;

		glm::mat4 m_CaptureProjection;
		const float m_Exposure, m_OutputAlpha;
		const uint32_t m_TextureSlot;
	};

}
