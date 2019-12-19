#include "pch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace tTexture::Renderer {

	static std::string FramebufferError(GLenum framebufferStatus)
	{
		switch (framebufferStatus)
		{
			case GL_FRAMEBUFFER_UNDEFINED:						return "GL_FRAMEBUFFER_UNDEFINED";
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:			return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:	return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:			return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:			return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
			case GL_FRAMEBUFFER_UNSUPPORTED:					return "GL_FRAMEBUFFER_UNSUPPORTED";
			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:			return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:		return "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
		}
		return "Unknown FramebufferError";
	}

	OpenGLFramebuffer::OpenGLFramebuffer()
		: m_RendererID(0)
	{
		glGenFramebuffers(1, &m_RendererID);
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
	}

	void OpenGLFramebuffer::BindRenderTarget(const std::shared_ptr<OpenGLTexture2D>& targetTexture, uint32_t mipLevel)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, targetTexture->GetRendererID(), mipLevel);

		uint32_t width = targetTexture->GetWidth() * (uint32_t)std::pow(0.5, mipLevel);
		uint32_t height = targetTexture->GetHeight() * (uint32_t)std::pow(0.5, mipLevel);
		TTEX_CORE_ASSERT(width > 0 && height > 0, "Invalid size");

		glViewport(0, 0, width, height);
	}

	void OpenGLFramebuffer::BindAndRenderToCubeFace(const std::shared_ptr<OpenGLTextureCube>& targetTexture, uint32_t faceIndex, uint32_t mipLevel)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex, targetTexture->GetRendererID(), mipLevel);

		uint32_t size = (uint32_t)(targetTexture->GetFaceSize() * std::pow(0.5, mipLevel));
		TTEX_CORE_ASSERT(size > 0, "Invalid size");

		glViewport(0, 0, size, size);
	}
	
	void OpenGLFramebuffer::CheckCompleteness() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		GLenum framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		TTEX_CORE_ASSERT(framebufferStatus == GL_FRAMEBUFFER_COMPLETE, FramebufferError(framebufferStatus));

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}
