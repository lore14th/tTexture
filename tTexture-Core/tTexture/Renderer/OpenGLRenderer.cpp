#include "pch.h"
#include "OpenGLRenderer.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>

//#define TTEX_RENDERER_DEBUG

namespace tTexture {

	OpenGLRenderer::OpenGLRenderer()
		: m_VertexArray(0), m_CaptureProjection(glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f))
	{
		m_Context = std::make_unique<Renderer::OpenGLContext>();
		Init();
	}

	OpenGLRenderer::~OpenGLRenderer()
	{
		Shutdown();
	}

	void OpenGLRenderer::RenderEquirectangularTexture(const Texture2D& source, TextureCube& result)
	{
		std::shared_ptr<Renderer::OpenGLTextureCube> targetTexture = std::make_shared<Renderer::OpenGLTextureCube>(512);

		m_Shader = std::make_unique<Renderer::OpenGLShader>("../tTexture-Core/tTexture/Renderer/shaders/Equirectangular.glsl");
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_ProjectionMatrix", m_CaptureProjection);
		m_Shader->SetUniform1f("u_OutputAlpha", 1.0f);
		m_Shader->SetUniform1i("u_Texture", 0);
		m_Shader->SetUniform1f("u_Exposure", 1.0f);

	#ifdef TTEX_RENDERER_DEBUG
		m_Shader->SetUniform1f("u_OutputAlpha", 0.9f);
	#endif

		m_Framebuffer = std::make_unique<Renderer::OpenGLFramebuffer>();

		ConvertAndSetTexture(source);
		CreateCube();

		for (uint32_t i = 0; i < 6; i++)
		{
			m_Framebuffer->BindAndRenderToCubeFace(targetTexture, i, 0);
			m_Context->Clear();
#ifdef TEXTURE_RENDERER_DEBUG
			RenderCommand::Clear(GetFaceColor(i));
#endif
			glm::mat4 captureView(1.0f);
			GetEquirectagularView(i, captureView);
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_ViewMatrix", captureView);

			m_CubeVertexBuffer->Bind();
			m_CubeIndexBuffer->Bind();
			m_Context->DrawIndexed(m_CubeIndexBuffer->GetCount(), false);

		}
		m_Framebuffer->Unbind();

		ConvertResultToCubeMap(targetTexture, result);
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

	void OpenGLRenderer::ConvertAndSetTexture(const Texture2D& texture)
	{
		m_SourceTexture = std::make_optional(std::make_unique<Renderer::OpenGLTexture2D>(texture));
		m_SourceTexture.value()->Bind(0);
	}

	void OpenGLRenderer::GetEquirectagularView(uint32_t faceIndex, glm::mat4& view)
	{
		TTEX_CORE_ASSERT(faceIndex < 6, "Index out of bounds");

		switch (faceIndex)
		{
		case 0: view = glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));		break;
		case 1: view = glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));	break;
		case 2: view = glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));		break;
		case 3: view = glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f));	break;
		case 4: view = glm::lookAt(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));		break;
		case 5: view = glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));	break;
		}
	}

	void OpenGLRenderer::ConvertResultToCubeMap(const std::shared_ptr<Renderer::OpenGLTextureCube>& source, TextureCube& result)
	{
		result.Data.Width = source->GetFaceSize();
		result.Data.Height = source->GetFaceSize();
		result.Data.Bpp = source->GetBpp();

		for (uint32_t i = 0; i < result.Images.size(); i++)
		{
			uint32_t size = 0;
			result.Images[i].Data = source->GetPixels(i, size);
			result.Images[i].Size = size;
		}
	}

}

