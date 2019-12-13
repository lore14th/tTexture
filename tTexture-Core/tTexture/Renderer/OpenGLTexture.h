#pragma once

#include "Core/Core.h"

namespace tTexture::Renderer {

	class OpenGLTexture2D
	{
	public:
		OpenGLTexture2D(int32_t width, int32_t height, int32_t bpp);
		OpenGLTexture2D(const std::shared_ptr<Texture2D>& texture);
		~OpenGLTexture2D() = default;

		void Bind(uint32_t slot) const;
		void GenerateMips() const;

		std::shared_ptr<Texture2D> ConvertToTexture() const;
 
		uint32_t GetWidth() const { return m_Data.Width; }
		uint32_t GetHeight() const { return m_Data.Height; }
		uint32_t GetBpp() const { return m_Data.Bpp; }

		uint32_t GetRendererID() const { return m_RendererID; }
	private:
		uint32_t m_RendererID;
		TextureData m_Data;

		uint32_t m_MipLevels; 
	};

	class OpenGLTextureCube
	{
	public:
		OpenGLTextureCube(int32_t faceSize, uint32_t bpp = 4);
		OpenGLTextureCube(const std::shared_ptr<TextureCube>& texture);
		~OpenGLTextureCube() = default;

		void Bind(uint32_t slot) const;
		void GenerateMips() const;

		std::shared_ptr<TextureCube> ConvertToTextureCube(uint32_t mipLevel = 0) const;
		
		int32_t GetFaceSize() const { return m_FaceSize; }
		uint32_t GetBpp() const { return m_Bpp; }
		uint32_t GetMipLevels() const { return m_MipLevels; }
		
		uint32_t GetRendererID() const { return m_RendererID; }
	private:
		uint32_t m_RendererID;

		int32_t m_FaceSize;
		uint32_t m_Bpp;
		uint32_t m_MipLevels;
	};

}