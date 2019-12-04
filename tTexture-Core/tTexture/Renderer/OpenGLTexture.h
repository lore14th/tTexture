#pragma once

#include "Core/Core.h"

namespace tTexture::Renderer {

	class OpenGLTexture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const Texture2D& texture);
		~OpenGLTexture2D() = default;

		void Bind(uint32_t slot) const;

		uint32_t GetWidth() const { return m_Texture.Data.Width; }
		uint32_t GetHeight() const { return m_Texture.Data.Height; }
		uint32_t GetBpp() const { return m_Texture.Data.Bpp; }

		uint32_t GetRendererID() const { return m_RendererID; }
	private:
		uint32_t m_RendererID;
		Texture2D m_Texture;

		uint32_t m_MipLevels; 
	};

	class OpenGLTextureCube
	{
	public:
		OpenGLTextureCube(uint32_t faceSize);
		OpenGLTextureCube(const TextureCube& texture);
		~OpenGLTextureCube() = default;

		void Bind(uint32_t slot) const;
		byte* GetPixels(uint32_t face, uint32_t& size) const;

		uint32_t GetFaceSize() const { return m_FaceSize; }
		uint32_t GetBpp() const { return m_Texture.Data.Bpp; }

		uint32_t GetRendererID() const { return m_RendererID; }
	private:
		inline void CreateTextureCommand();
		void CreateEmptyTexture();

	private:
		uint32_t m_RendererID;
		TextureCube m_Texture;

		uint32_t m_FaceSize;
		uint32_t m_MipLevels;
	};

}