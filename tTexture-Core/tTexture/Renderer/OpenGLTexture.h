#pragma once

#include "Core/Core.h"

namespace tTexture::Renderer {

	class OpenGLTexture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::shared_ptr<Texture2D>& texture);
		~OpenGLTexture2D() = default;

		void Bind(uint32_t slot) const;

		uint32_t GetWidth() const { return m_Texture->Data.Width; }
		uint32_t GetHeight() const { return m_Texture->Data.Height; }
		uint32_t GetBpp() const { return m_Texture->Data.Bpp; }

		uint32_t GetRendererID() const { return m_RendererID; }
	private:
		uint32_t m_RendererID;
		std::shared_ptr<Texture2D> m_Texture;

		uint32_t m_MipLevels; 
	};

	class OpenGLTextureCube
	{
	public:
		OpenGLTextureCube(uint32_t faceSize);
		OpenGLTextureCube(const std::shared_ptr<TextureCube>& texture);
		~OpenGLTextureCube() = default;

		void Bind(uint32_t slot) const;
		void GenerateMips() const;

		std::shared_ptr<TextureCube> ConvertToTextureCube();
		
		uint32_t GetFaceSize() const { return m_FaceSize; }
		uint32_t GetMipLevels() const { return m_MipLevels; }
		uint32_t GetBpp() const { return m_Texture->Data.Bpp; }
		
		uint32_t GetRendererID() const { return m_RendererID; }
	private:
		inline void CreateTextureCommand();
		void CreateEmptyTexture();
	private:
		uint32_t m_RendererID;
		std::shared_ptr<TextureCube> m_Texture;

		uint32_t m_FaceSize;
		uint32_t m_MipLevels;
	};

}