#include "pch.h"
#include "OpenGLRenderer.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>

//#define TTEX_RENDERER_DEBUG

namespace tTexture {

#ifdef TTEX_RENDERER_DEBUG
	static glm::vec4 GetFaceColor(uint32_t faceIndex)
	{
		switch (faceIndex)
		{
			case 0: return glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);	// POS_X	red
			case 1: return glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);	// NEG_X	blue
			case 2: return glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);	// POS_Y	white
			case 3: return glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);	// NEG_Y	gray
			case 4: return glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);	// POS_Z	green
			case 5: return glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);	// NEG_Z	magenta
		}
		return glm::vec4(0, 0, 0, 1);
	}
#endif

	OpenGLRenderer::OpenGLRenderer()
		: m_VertexArray(0), m_Resolution(512), m_CaptureProjection(glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f)), 
		m_Exposure(1.0f), m_OutputAlpha(1.0f), m_TextureSlot(0)
	{
#ifdef TTEX_RENDERER_DEBUG
		m_TextureSlot = 0.9f;
#endif

		m_Context = std::make_unique<Renderer::OpenGLContext>();
		m_Framebuffer = std::make_unique<Renderer::OpenGLFramebuffer>();

		Init();
		CreateCube();
	}

	OpenGLRenderer::~OpenGLRenderer()
	{
		Shutdown();
	}

	std::shared_ptr<TextureCube> OpenGLRenderer::RenderEquirectangularTexture(const std::shared_ptr<Texture2D>& source)
	{
		// Create Target TextureCube
		std::shared_ptr<Renderer::OpenGLTextureCube> targetTexture = std::make_shared<Renderer::OpenGLTextureCube>(m_Resolution);

		// Create shader and set uniforms
		std::unique_ptr<Renderer::OpenGLShader> shader = std::make_unique<Renderer::OpenGLShader>("../tTexture-Core/tTexture/Renderer/shaders/Equirectangular.glsl");
		shader->Bind();
		shader->SetUniformMat4f("u_ProjectionMatrix", m_CaptureProjection);
		shader->SetUniform1f("u_OutputAlpha", m_OutputAlpha);
		shader->SetUniform1i("u_Texture", m_TextureSlot);
		shader->SetUniform1f("u_Exposure", m_Exposure);

		// Create and bind source texture
		std::unique_ptr<Renderer::OpenGLTexture2D> sourceTexture = std::make_unique<Renderer::OpenGLTexture2D>(source);
		sourceTexture->Bind(m_TextureSlot);

		m_CubeVertexBuffer->Bind();
		m_CubeIndexBuffer->Bind();
		// Render texture
		m_Context->Clear(glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));
		for (uint32_t i = 0; i < 6; i++)
		{
			m_Framebuffer->BindAndRenderToCubeFace(targetTexture, i, 0);
			m_Context->Clear();
		#ifdef TTEX_RENDERER_DEBUG
			m_Context->Clear(GetFaceColor(i));
		#endif
			shader->SetUniformMat4f("u_ViewMatrix", GetEquirectagularView(i));
			m_Context->DrawIndexed(m_CubeIndexBuffer->GetCount(), true);
		}
		m_Framebuffer->Unbind();
		return targetTexture->ConvertToTextureCube();
	}

	std::shared_ptr<tTexture::TextureCube> OpenGLRenderer::CreateIrradianceMap(const std::shared_ptr<TextureCube>& source)
	{
		// Create Target TextureCube
		std::shared_ptr<Renderer::OpenGLTextureCube> targetTexture = std::make_shared<Renderer::OpenGLTextureCube>(m_Resolution);

		// Create shader and set uniforms
		std::unique_ptr<Renderer::OpenGLShader> shader = std::make_unique<Renderer::OpenGLShader>("../tTexture-Core/tTexture/Renderer/shaders/Irradiance.glsl");
		shader->Bind();
		shader->SetUniformMat4f("u_ProjectionMatrix", m_CaptureProjection);
		shader->SetUniform1f("u_OutputAlpha", m_OutputAlpha);
		shader->SetUniform1i("u_Texture", m_TextureSlot);
		shader->SetUniform1f("u_Exposure", m_Exposure);

		// Create and bind source texture
		std::unique_ptr<Renderer::OpenGLTextureCube> sourceTexture = std::make_unique<Renderer::OpenGLTextureCube>(source);
		sourceTexture->Bind(m_TextureSlot);

		m_CubeVertexBuffer->Bind();
		m_CubeIndexBuffer->Bind();

		m_Context->Clear(glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));
		for (uint32_t i = 0; i < 6; i++)
		{
			m_Framebuffer->BindAndRenderToCubeFace(targetTexture, i, 0);
			m_Context->Clear();
		#ifdef TTEX_RENDERER_DEBUG
			m_Context->Clear(GetFaceColor(i));
		#endif
			shader->SetUniformMat4f("u_ViewMatrix", GetIrradianceView(i));
			m_Context->DrawIndexed(m_CubeIndexBuffer->GetCount(), true);
		}
		m_Framebuffer->Unbind();

		return targetTexture->ConvertToTextureCube();
	}

	void OpenGLRenderer::Init()
	{
		// Create a default vertex array
		glCreateVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);
	}

	void OpenGLRenderer::Shutdown()
	{
		glDeleteVertexArrays(1, &m_VertexArray);
	}

	void OpenGLRenderer::CreateCube()
	{
		Renderer::VertexBufferLayout layout = {
			{ Renderer::ShaderDataType::Float3, "a_Position" },
		};

		static float vertices[24 * 6] = {
		// Front Face
			-1.0f, -1.0f,  1.0f,	 0.0f,  0.0f,  -1.0f,
			-1.0f,  1.0f,  1.0f,	 0.0f,  0.0f,  -1.0f,
			 1.0f,  1.0f,  1.0f,	 0.0f,  0.0f,  -1.0f,
			 1.0f, -1.0f,  1.0f,	 0.0f,  0.0f,  -1.0f,
		 // Back Face				 	    		   
			-1.0f, -1.0f, -1.0f,	 0.0f,  0.0f, 1.0f,
			 1.0f, -1.0f, -1.0f,	 0.0f,  0.0f, 1.0f,
			 1.0f,  1.0f, -1.0f,	 0.0f,  0.0f, 1.0f,
			-1.0f,  1.0f, -1.0f,	 0.0f,  0.0f, 1.0f,
		 // TopFace					 				   
			-1.0f,  1.0f, -1.0f,	 0.0f,  -1.0f,  0.0f,
			 1.0f,  1.0f, -1.0f,	 0.0f,  -1.0f,  0.0f,
			 1.0f,  1.0f,  1.0f,	 0.0f,  -1.0f,  0.0f,
			-1.0f,  1.0f,  1.0f,	 0.0f,  -1.0f,  0.0f,
		  // Bottom Face						   	   
			-1.0f, -1.0f, -1.0f,	 0.0f, 1.0f,  0.0f,
			-1.0f, -1.0f,  1.0f,	 0.0f, 1.0f,  0.0f,
			 1.0f, -1.0f,  1.0f,	 0.0f, 1.0f,  0.0f,
			 1.0f, -1.0f, -1.0f,	 0.0f, 1.0f,  0.0f,
		  // Right Face							   	   
			 1.0f, -1.0f, -1.0f,	-1.0f,  0.0f,  0.0f,
			 1.0f, -1.0f,  1.0f,	-1.0f,  0.0f,  0.0f,
			 1.0f,  1.0f,  1.0f,	-1.0f,  0.0f,  0.0f,
			 1.0f,  1.0f, -1.0f,	-1.0f,  0.0f,  0.0f,
		// Left Face					    	   	   
			-1.0f, -1.0f, -1.0f,	1.0f,  0.0f,  0.0f,
			-1.0f,  1.0f, -1.0f,	1.0f,  0.0f,  0.0f,
			-1.0f,  1.0f,  1.0f,	1.0f,  0.0f,  0.0f,
			-1.0f, -1.0f,  1.0f,	1.0f,  0.0f,  0.0f,
		};

		static uint32_t indices[36] = {
			 0,  1,  2,  0,  2,  3,
			 4,  5,  6,  4,  6,  7,
			 8,  9, 10,  8, 10, 11,
			12, 13, 14, 12, 14, 15,
			16, 17, 18, 16, 18, 19,
			20, 21, 22, 20, 22, 23
		};

		m_CubeVertexBuffer = std::make_unique<Renderer::OpenGLVertexBuffer>(layout, sizeof(vertices));
		m_CubeVertexBuffer->SetData(vertices, sizeof(vertices));
		
		m_CubeIndexBuffer = std::make_unique<Renderer::OpenGLIndexBuffer>(sizeof(indices));
		m_CubeIndexBuffer->SetData(indices, sizeof(indices));
	}

	glm::mat4 OpenGLRenderer::GetEquirectagularView(uint32_t faceIndex)
	{
		TTEX_CORE_ASSERT(faceIndex < 6, "Index out of bounds");
		switch (faceIndex)
		{
			case 0: return glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3( 0.0f, -1.0f, 0.0f));
			case 1: return glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3( 0.0f, -1.0f, 0.0f));
			case 2: return glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 1.0f,  0.0f, 0.0f));
			case 3: return glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(-1.0f,  0.0f, 0.0f));
			case 4: return glm::lookAt(glm::vec3(0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3( 0.0f, -1.0f, 0.0f));
			case 5: return glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3( 0.0f, -1.0f, 0.0f));
		}
		TTEX_CORE_ASSERT(false, "");
		return glm::mat4(1.0f);
	}

	glm::mat4 OpenGLRenderer::GetIrradianceView(uint32_t faceIndex)
	{
		TTEX_CORE_ASSERT(faceIndex < 6, "Index out of bounds");
		switch (faceIndex)
		{
			case 0: return glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f));
			case 1: return glm::lookAt(glm::vec3(0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f));
			case 2: return glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f));
			case 3: return glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f));
			case 4: return glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f));
			case 5: return glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f));
		}
		TTEX_CORE_ASSERT(false, "");
		return glm::mat4(1.0f);
	}

}

