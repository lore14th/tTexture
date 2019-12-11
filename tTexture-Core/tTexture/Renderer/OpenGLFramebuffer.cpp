#include "pch.h"
#include "OpenGLFramebuffer.h"

#include "Core/Application.h"

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
		CreateFramebuffer();
	}

	OpenGLFramebuffer::OpenGLFramebuffer(uint32_t width, uint32_t height, FramebufferFormat format)
		: m_RendererID(0)
	{
		CreateFramebuffer();

		OpenGLRenderTarget renderTarget;
		renderTarget.ColorAttachment = std::make_shared<Renderer::OpenGLTexture2D>(width, height, 4);
		renderTarget.Width = width;
		renderTarget.Height = height;
		renderTarget.Format = format;
		CreateDepthAttachment(renderTarget);
		BindColorAttachmentAsRenderTarget(renderTarget);

		m_RenderTargets.push_back(renderTarget);

		CheckCompleteness();
		m_Complete = true; // no other attachment can be added
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
	}

	void OpenGLFramebuffer::Bind() const
	{
		if (!m_Complete)
			CheckCompleteness();

		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		// if every attachment shares the same dimensions, we can use the dimensions of the first attachment
		glViewport(0, 0, m_RenderTargets[0].Width, m_RenderTargets[0].Height);
	}

	void OpenGLFramebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// TODO: get window 

		uint32_t width = 1;
		uint32_t height = 1;
		glViewport(0, 0, width, height);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		// TODO: handle resizing
		TTEX_CORE_ASSERT(false, "Resize not implemented");

		// 		m_Width = width;
		// 		m_Height = height;
		// 
		// 		TextureProperties colorAttachmentProperties;
		// 		colorAttachmentProperties.Format = Texture::FramebufferToTextureFormat(m_Format);
		// 		colorAttachmentProperties.Sampler.Wrap = TextureWrap::Clamp_To_Edge;
		// 
		// 		m_ColorAttachment = Texture2D::Create(m_Width, m_Height, colorAttachmentProperties);
		// 
		// 		CreateFramebuffer();
	}

	void OpenGLFramebuffer::AddColorAttachment(std::shared_ptr<OpenGLTexture2D>& targetTexture, uint32_t mipLevel)
	{
		// TODO: handle mip level

		OpenGLRenderTarget renderTarget;
		renderTarget.ColorAttachment = targetTexture;
		renderTarget.Width = targetTexture->GetWidth();
		renderTarget.Height = targetTexture->GetHeight();
		renderTarget.Format = FramebufferFormat::RGBA8;
		CreateDepthAttachment(renderTarget);
		BindColorAttachmentAsRenderTarget(renderTarget);

		if (!m_Complete)
			m_RenderTargets.push_back(renderTarget);
		else
			TTEX_CORE_ASSERT(false, "OpenGLFramebuffer:No attachment can be added to this framebuffer");
		// this error occurs whether you have already bound a multi-target framebuffer, or you're using a single target framebuffer
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
	
	uint32_t OpenGLFramebuffer::GetWidth(uint32_t attachmentIndex) const
	{
		if (IsValidAttachmentIndex(attachmentIndex))
			return m_RenderTargets[attachmentIndex].Width;

		TTEX_CORE_ASSERT(IsValidAttachmentIndex(attachmentIndex), "OpenGLFramebuffer: index out of bounds");
		return 0;
	}

	uint32_t OpenGLFramebuffer::GetHeight(uint32_t attachmentIndex) const
	{
		if (IsValidAttachmentIndex(attachmentIndex))
			return m_RenderTargets[attachmentIndex].Height;

		TTEX_CORE_ASSERT(IsValidAttachmentIndex(attachmentIndex), "OpenGLFramebuffer: index out of bounds");
		return 0;
	}

	OpenGLFramebuffer::FramebufferFormat OpenGLFramebuffer::GetFormat(uint32_t attachmentIndex) const
	{
		if (IsValidAttachmentIndex(attachmentIndex))
			return m_RenderTargets[attachmentIndex].Format;

		TTEX_CORE_ASSERT(IsValidAttachmentIndex(attachmentIndex), "OpenGLFramebuffer: index out of bounds");
		return FramebufferFormat::None;
	}

	const std::optional<std::shared_ptr<OpenGLTexture2D>> OpenGLFramebuffer::GetColorAttachment(uint32_t attachmentIndex) const
	{
		if (IsValidAttachmentIndex(attachmentIndex))
			return std::optional(m_RenderTargets[attachmentIndex].ColorAttachment);

		TTEX_CORE_ASSERT(IsValidAttachmentIndex(attachmentIndex), "OpenGLFramebuffer: index out of bounds");
		return {};
	}

	bool OpenGLFramebuffer::IsValidAttachmentIndex(uint32_t attachmentIndex) const
	{
		return attachmentIndex < m_RenderTargets.size();
	}

	void OpenGLFramebuffer::CreateFramebuffer()
	{
		glGenFramebuffers(1, &m_RendererID);
	}

	void OpenGLFramebuffer::CheckCompleteness() const
	{
		TTEX_CORE_ASSERT(CheckSharedProperties(), "OpenGLFramebuffer:Not all the attachments share the same properties");

		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		GLenum framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		TTEX_CORE_ASSERT(framebufferStatus == GL_FRAMEBUFFER_COMPLETE, FramebufferError(framebufferStatus));

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::CreateDepthAttachment(OpenGLRenderTarget& renderTarget, uint32_t mipLevel)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		glGenTextures(1, &renderTarget.DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, renderTarget.DepthAttachment);

		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, renderTarget.Width, renderTarget.Height, 0,
			GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
		);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, renderTarget.DepthAttachment, mipLevel);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::BindColorAttachmentAsRenderTarget(OpenGLRenderTarget& renderTarget, uint32_t mipLevel) const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		const OpenGLTexture2D * colorAttachment = static_cast<const OpenGLTexture2D*>(renderTarget.ColorAttachment.get());
		glBindTexture(GL_TEXTURE_2D, colorAttachment->GetRendererID());

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, renderTarget.Width, renderTarget.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment->GetRendererID(), mipLevel);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	bool OpenGLFramebuffer::CheckSharedProperties() const
	{
		TTEX_CORE_ASSERT(m_RenderTargets.size() > 0, "OpenGLFramebuffer: no attachments");
		bool result = true;

		uint32_t width = m_RenderTargets[0].Width;
		uint32_t height = m_RenderTargets[0].Height;
		FramebufferFormat format = m_RenderTargets[0].Format;

		for (const OpenGLRenderTarget& renderTarget : m_RenderTargets)
			result = result && (renderTarget.Width == width && renderTarget.Height == height && renderTarget.Format == format);

		m_Complete = result;
		return result;
	}



}
