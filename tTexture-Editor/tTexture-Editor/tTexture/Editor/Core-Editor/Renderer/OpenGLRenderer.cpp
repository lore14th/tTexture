#include "pch.h"
#include "OpenGLRenderer.h"

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

	static glm::vec4 GetMipColor(uint32_t mipLevel)
	{
		switch (mipLevel)
		{
			case 0: return glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);	// red
			case 1: return glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);	// blue
			case 2: return glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);	// white
			case 3: return glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);	// gray
			case 4: return glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);	// green
			case 5: return glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);	// magenta
			default: return glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
	}
#endif

	OpenGLRenderer::OpenGLRenderer()
		: m_VertexArray(0), m_Resolution(512), m_CaptureProjection(glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f)), 
		m_Exposure(1.0f), m_OutputAlpha(1.0f), m_TextureSlot(0)
	{
		m_Context = std::make_unique<Renderer::OpenGLContext>();
		m_Framebuffer = std::make_unique<Renderer::OpenGLFramebuffer>();

		Init();
	}

	OpenGLRenderer::~OpenGLRenderer()
	{
		Shutdown();
	}

	std::shared_ptr<TextureCube> OpenGLRenderer::RenderEquirectangularTexture(const std::shared_ptr<Texture2D>& source) const
	{
		// Create Target TextureCube
		std::shared_ptr<Renderer::OpenGLTextureCube> targetTexture = std::make_shared<Renderer::OpenGLTextureCube>(m_Resolution);

		// Create shader and set uniforms
		std::unique_ptr<Renderer::OpenGLShader> shader = std::make_unique<Renderer::OpenGLShader>("tTexture/Editor/Core-Editor/Renderer/shaders/Equirectangular.glsl");
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
		for (uint32_t i = 0; i < 6; i++)
		{
			m_Framebuffer->BindAndRenderToCubeFace(targetTexture, i, 0);
			m_Context->Clear();
		#ifdef TTEX_RENDERER_DEBUG
			m_Context->Clear(GetFaceColor(i));
		#endif
			shader->SetUniformMat4f("u_ViewMatrix", GetEquirectagularView(i));
			m_Context->DrawIndexed(m_CubeIndexBuffer->GetCount());
		}
	
		return targetTexture->ConvertToTextureCube();
	}

	std::shared_ptr<tTexture::TextureCube> OpenGLRenderer::CreateIrradianceMap(const std::shared_ptr<TextureCube>& source) const
	{
		// Create Target TextureCube
		std::shared_ptr<Renderer::OpenGLTextureCube> targetTexture = std::make_shared<Renderer::OpenGLTextureCube>(m_Resolution);

		// Create shader and set uniforms
		std::unique_ptr<Renderer::OpenGLShader> shader = std::make_unique<Renderer::OpenGLShader>("tTexture/Editor/Core-Editor/Renderer/shaders/Irradiance.glsl");
		shader->Bind();
		shader->SetUniformMat4f("u_ProjectionMatrix", m_CaptureProjection);
		shader->SetUniform1f("u_OutputAlpha", m_OutputAlpha);
		shader->SetUniform1i("u_Texture", m_TextureSlot);

		// Create and bind source texture
		std::unique_ptr<Renderer::OpenGLTextureCube> sourceTexture = std::make_unique<Renderer::OpenGLTextureCube>(source);
		sourceTexture->Bind(m_TextureSlot);

		m_CubeVertexBuffer->Bind();
		m_CubeIndexBuffer->Bind();

		for (uint32_t i = 0; i < 6; i++)
		{
			m_Framebuffer->BindAndRenderToCubeFace(targetTexture, i, 0);
			m_Context->Clear();
		#ifdef TTEX_RENDERER_DEBUG
			m_Context->Clear(GetFaceColor(i));
		#endif
			shader->SetUniformMat4f("u_ViewMatrix", GetIrradianceView(i));
			m_Context->DrawIndexed(m_CubeIndexBuffer->GetCount());
		}

		return targetTexture->ConvertToTextureCube();
	}

	std::shared_ptr<tTexture::PrefilteredTextureCube> OpenGLRenderer::PrefilterEnvironmentMap(const std::shared_ptr<tTexture::TextureCube>& source) const
	{
		// Create Target TextureCube
		std::shared_ptr<Renderer::OpenGLTextureCube> targetTexture = std::make_shared<Renderer::OpenGLTextureCube>(source->Data.Width);

		// Create shader and set uniforms
		std::unique_ptr<Renderer::OpenGLShader> shader = std::make_unique<Renderer::OpenGLShader>("tTexture/Editor/Core-Editor/Renderer/shaders/Prefilter.glsl");
		shader->Bind();
 		shader->SetUniformMat4f("u_ProjectionMatrix", m_CaptureProjection);
 		shader->SetUniform1f("u_OutputAlpha", m_OutputAlpha);
 		shader->SetUniform1i("u_Texture", m_TextureSlot);

		// Create and bind source texture
		std::unique_ptr<Renderer::OpenGLTextureCube> sourceTexture = std::make_unique<Renderer::OpenGLTextureCube>(source);

		m_CubeVertexBuffer->Bind();
		m_CubeIndexBuffer->Bind();

		// allocate the result texture
		const uint32_t maxMips = targetTexture->GetMipLevels();
		std::shared_ptr<PrefilteredTextureCube> result = std::make_shared<PrefilteredTextureCube>();
		result->Allocate(maxMips);

		for (uint32_t mipLevel = 0; mipLevel < maxMips; mipLevel++)
		{
			// Set level roughness
			shader->SetUniform1f("u_Roughness", (float)mipLevel / (float)maxMips);
			for (uint32_t i = 0; i < 6; i++)
			{
				m_Framebuffer->BindAndRenderToCubeFace(targetTexture, i, mipLevel);
				sourceTexture->Bind(m_TextureSlot);
				m_Context->Clear();
			#ifdef TTEX_RENDERER_DEBUG
				m_Context->Clear(GetMipColor(mipLevel));
			#endif
				shader->SetUniformMat4f("u_ViewMatrix", GetPrefilterView(i));
				m_Context->DrawIndexed(m_CubeIndexBuffer->GetCount());
			}

			result->PushLevel(targetTexture->ConvertToTextureCube(mipLevel));
		}
		return result;
	}

	std::shared_ptr<tTexture::Texture2D> OpenGLRenderer::CreateBRDF(BRDFType type, uint32_t size)
	{
		// TODO: depending on the type, change shader and uniforms

		// Create Target Texture2D
		std::shared_ptr<Renderer::OpenGLTexture2D> targetTexture = std::make_shared<Renderer::OpenGLTexture2D>(size, size, 4);

		// Create shader and set uniforms
		std::unique_ptr<Renderer::OpenGLShader> shader = std::make_unique<Renderer::OpenGLShader>("tTexture/Editor/Core-Editor//Renderer/shaders/BRDF.glsl");
		shader->Bind();
		
		m_SquareVertexBuffer->Bind();
		m_SquareIndexBuffer->Bind();
		
		m_Framebuffer->BindRenderTarget(targetTexture, 0);
		m_Context->Clear({ 0.8, 0.8, 0.2, 1 });
		m_Context->DrawIndexed(m_SquareIndexBuffer->GetCount());

		return targetTexture->ConvertToTexture();
	}

	void OpenGLRenderer::Init()
	{
		// Create a default vertex array
		glCreateVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		CreateCube();
		CreateSquare();
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

		static float cubeVertices[24 * 6] = {
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

		static uint32_t cubeIndices[36] = {
			 0,  1,  2,  0,  2,  3,
			 4,  5,  6,  4,  6,  7,
			 8,  9, 10,  8, 10, 11,
			12, 13, 14, 12, 14, 15,
			16, 17, 18, 16, 18, 19,
			20, 21, 22, 20, 22, 23
		};

		m_CubeVertexBuffer = std::make_unique<Renderer::OpenGLVertexBuffer>(layout, (uint32_t)sizeof(cubeVertices));
		m_CubeVertexBuffer->SetData(cubeVertices, (uint32_t)sizeof(cubeVertices));
		
		m_CubeIndexBuffer = std::make_unique<Renderer::OpenGLIndexBuffer>((uint32_t)sizeof(cubeIndices));
		m_CubeIndexBuffer->SetData(cubeIndices, (uint32_t)sizeof(cubeIndices));
	}

	void OpenGLRenderer::CreateSquare()
	{
		Renderer::VertexBufferLayout layout = {
			{ Renderer::ShaderDataType::Float3, "a_Position" },
			{ Renderer::ShaderDataType::Float2, "a_TexCoord" }
		};

		static float squareVertices[4 * 5] = {
			-1.0f, -1.0f, 0.1f,		0.0f, 0.0f,
			 1.0f, -1.0f, 0.1f,		1.0f, 0.0f,
			 1.0f,  1.0f, 0.1f,		1.0f, 1.0f,
			-1.0f,  1.0f, 0.1f,		0.0f, 1.0f,
		};

		static uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		m_SquareVertexBuffer = std::make_unique<Renderer::OpenGLVertexBuffer>(layout, (uint32_t)sizeof(squareVertices));
		m_SquareVertexBuffer->SetData(squareVertices, (uint32_t)sizeof(squareVertices));

		m_SquareIndexBuffer = std::make_unique<Renderer::OpenGLIndexBuffer>((uint32_t)sizeof(squareIndices));
		m_SquareIndexBuffer->SetData(squareIndices, (uint32_t)sizeof(squareIndices));
	}

	glm::mat4 OpenGLRenderer::GetEquirectagularView(uint32_t faceIndex) const
	{
		TTEX_CORE_ASSERT(faceIndex < 6, "Index out of bounds");
		switch (faceIndex)
		{
			case 0: return glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3( 0.0f, -1.0f, 0.0f));
			case 1: return glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3( 0.0f, -1.0f, 0.0f));
			case 2: return glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 1.0f,  0.0f, 0.0f));
			case 3: return glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(-1.0f,  0.0f, 0.0f));
			case 4: return glm::lookAt(glm::vec3(0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3( 0.0f, -1.0f, 0.0f));
			case 5: return glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3( 0.0f, -1.0f, 0.0f));
		}
		TTEX_CORE_ASSERT(false, "");
		return glm::mat4(1.0f);
	}

	glm::mat4 OpenGLRenderer::GetIrradianceView(uint32_t faceIndex) const
	{
		TTEX_CORE_ASSERT(faceIndex < 6, "Index out of bounds");
		switch (faceIndex)
		{
			case 0: return glm::lookAt(glm::vec3(0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f));
			case 1: return glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f));
			case 2: return glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f));
			case 3: return glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f));
			case 4: return glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f));
			case 5: return glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f));
		}
		TTEX_CORE_ASSERT(false, "");
		return glm::mat4(1.0f);
	}

	glm::mat4 OpenGLRenderer::GetPrefilterView(uint32_t faceIndex) const
	{
		TTEX_CORE_ASSERT(faceIndex < 6, "Index out of bounds");
		switch (faceIndex)
		{
			case 0: return glm::lookAt(glm::vec3(0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f));		break;
			case 1: return glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f));		break;
			case 2: return glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f));		break;
			case 3: return glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f));		break;
			case 4: return glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f));		break;
			case 5: return glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f));		break;
		}
		TTEX_CORE_ASSERT(false, "");
		return glm::mat4(1.0f);
	}

}