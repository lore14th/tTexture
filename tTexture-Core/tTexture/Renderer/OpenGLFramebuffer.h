#pragma once

#include "Core/Core.h"
#include "Renderer/OpenGLTexture.h"

#include <optional>

namespace tTexture::Renderer {

	class OpenGLFramebuffer
	{
	public:
		enum class FramebufferFormat { None = 0, RGBA8 = 1};
		enum class ReadingFormat { None = 0, RGB8 = 1, BGR8 = 2, };
	public:
		OpenGLFramebuffer();
		OpenGLFramebuffer(uint32_t width, uint32_t height, FramebufferFormat format);
		~OpenGLFramebuffer();

		void Bind() const;
		void Unbind() const;

		void Resize(uint32_t width, uint32_t height);
		void AddColorAttachment(std::shared_ptr<OpenGLTexture2D>& targetTexture, uint32_t mipLevel) ;
		void BindAndRenderToCubeFace(const std::shared_ptr<OpenGLTextureCube>& targetTexture, uint32_t faceIndex, uint32_t mipLevel) ;

		byte* GetPixels(uint32_t& size, ReadingFormat format) const ;

		uint32_t GetWidth(uint32_t attachmentIndex) const ;
		uint32_t GetHeight(uint32_t attachmentIndex) const ;
		FramebufferFormat GetFormat(uint32_t attachmentIndex) const ;
		const std::optional<std::shared_ptr<OpenGLTexture2D>> GetColorAttachment(uint32_t attachmentIndex) const;

		bool IsValidAttachmentIndex(uint32_t attachmentIndex) const ;
		uint32_t GetAttachmentCount() const { return (uint32_t)m_RenderTargets.size(); }

		uint32_t GetRendererID() const { return m_RendererID; }
	private:
		void CreateFramebuffer();
		void CheckCompleteness() const;

	private:
		struct OpenGLRenderTarget
		{
			std::shared_ptr<OpenGLTexture2D> ColorAttachment;
			uint32_t DepthAttachment;

			uint32_t Width, Height;
			FramebufferFormat Format;
		};

	private:
		void CreateDepthAttachment(OpenGLRenderTarget& renderTarget, uint32_t mipLevel = 0);
		void BindColorAttachmentAsRenderTarget(OpenGLRenderTarget& renderTarget, uint32_t mipLevel = 0) const;

		bool CheckSharedProperties() const;
	private:
		uint32_t m_RendererID;
		mutable bool m_Complete = false; // if the Framebuffer is complete, no more attachment can be added

		std::vector<OpenGLRenderTarget> m_RenderTargets;
	};

}
