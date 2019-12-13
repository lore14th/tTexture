#include "pch.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>

namespace tTexture::Renderer {

	static GLenum GetInternalFormat(uint32_t bpp)
	{
		if (bpp == 3)
			return GL_SRGB8;
		else
			return GL_RGBA8;
	}

	static GLenum GetPixelFormat(uint32_t bpp)
	{
		if (bpp == 3)
			return GL_RGB;
		else
			return GL_RGBA;
	}

	// ------------------------------------------------------
	// -- OpenGLTexture2D
	// ------------------------------------------------------
	OpenGLTexture2D::OpenGLTexture2D(int32_t width, int32_t height, int32_t bpp)
		: m_RendererID(0), m_Data({ width, height, bpp })
	{
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_MipLevels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		glTexImage2D(GL_TEXTURE_2D, 0, GetInternalFormat(m_Data.Bpp), width, height, 0, GetPixelFormat(m_Data.Bpp), GL_UNSIGNED_BYTE, nullptr);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::shared_ptr<Texture2D>& texture)
		: m_RendererID(0), m_Data({ texture->Data.Width, texture->Data.Height, texture->Data.Bpp })
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		m_MipLevels = CalculateMipMapCount(m_Data.Width, m_Data.Height);

		glTextureStorage2D(m_RendererID, m_MipLevels, GetInternalFormat(m_Data.Bpp), m_Data.Width, m_Data.Height);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, m_MipLevels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Data.Width, m_Data.Height, GetPixelFormat(m_Data.Bpp), GL_UNSIGNED_BYTE, texture->Image.Data);
		glGenerateTextureMipmap(m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTexture2D::GenerateMips() const
	{
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	std::shared_ptr<tTexture::Texture2D> OpenGLTexture2D::ConvertToTexture() const
	{
		std::shared_ptr<Texture2D> result = std::make_shared<Texture2D>(m_Data.Width, m_Data.Height, m_Data.Bpp);
		result->AllocateTexture();

		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glGetTexImage(GL_TEXTURE_2D, 0, m_Data.Bpp == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, result->Image.Data);
		glBindTexture(GL_TEXTURE_2D, 0);

		return result;
	}

	// ------------------------------------------------------
	// -- OpenGLTextureCube
	// ------------------------------------------------------

	OpenGLTextureCube::OpenGLTextureCube(int32_t faceSize, uint32_t bpp)
		: m_RendererID(0), m_Bpp(bpp), m_FaceSize(faceSize)
	{
		m_MipLevels = CalculateMipMapCount(m_FaceSize, m_FaceSize);

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_MipLevels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

		GLenum internalFormat = GetInternalFormat(m_Bpp);
		GLenum pixelFormat = GetPixelFormat(m_Bpp);

		for (uint32_t i = 0; i < 6; i++)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, m_FaceSize, m_FaceSize, 0, pixelFormat, GL_UNSIGNED_BYTE, nullptr);

		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	OpenGLTextureCube::OpenGLTextureCube(const std::shared_ptr<TextureCube>& texture)
		: m_RendererID(0), m_FaceSize(0), m_Bpp(texture->Data.Bpp)
	{
		TTEX_CORE_ASSERT(texture->Data.Width == texture->Data.Height, "OpenGLTextureCube:non square faces");
		m_FaceSize = texture->Data.Width;
		m_MipLevels = CalculateMipMapCount(m_FaceSize, m_FaceSize);

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

		GLenum internalFormat = GetInternalFormat(m_Bpp);
		GLenum pixelFormat = GetPixelFormat(m_Bpp);

		for (uint32_t i = 0; i < texture->Images.size(); i++)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, m_FaceSize, m_FaceSize, 0, pixelFormat, GL_UNSIGNED_BYTE, texture->Images[(int32_t)tTexture::Face::POS_X + i].Data);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	void OpenGLTextureCube::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTextureCube::GenerateMips() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	std::shared_ptr<TextureCube> OpenGLTextureCube::ConvertToTextureCube(uint32_t mipLevel) const
	{
		uint32_t faceSize = m_FaceSize * std::pow(0.5, mipLevel);

		std::shared_ptr<TextureCube> result = std::make_shared<TextureCube>(faceSize, m_Bpp);
		result->AllocateTexture();

		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
		for (uint32_t i = 0; i < 6; i++)
			glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mipLevel, m_Bpp == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, result->Images[i].Data);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		return result;
	}

}
