#include "pch.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>

namespace tTexture::Renderer {

	static int32_t CalculateMipMapCount(int32_t width, int32_t height)
	{
		int32_t levels = 1;
		while ((width | height) >> levels)
		{
			levels++;
		}
		return levels;
	}

	static GLenum GetInternalFormat(uint32_t bpp)
	{
		if (bpp == 3)
			return GL_SRGB;
		else
			return GL_RGBA;
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

	OpenGLTexture2D::OpenGLTexture2D(const std::shared_ptr<Texture2D>& texture)
		: m_RendererID(0), m_Texture(texture)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		m_MipLevels = CalculateMipMapCount(m_Texture->Data.Width, m_Texture->Data.Height);

		glTextureStorage2D(m_RendererID, m_MipLevels, GetInternalFormat(m_Texture->Data.Bpp), m_Texture->Data.Width, m_Texture->Data.Height);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, m_MipLevels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Texture->Data.Width, m_Texture->Data.Height, GetPixelFormat(m_Texture->Data.Bpp), GL_UNSIGNED_BYTE, m_Texture->Image.Data);
		glGenerateTextureMipmap(m_RendererID);		
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_RendererID(0)
	{
		m_Texture->Data.Width = width;
		m_Texture->Data.Height = height;
		m_Texture->Data.Bpp = 4;

		m_MipLevels = CalculateMipMapCount(m_Texture->Data.Width, m_Texture->Data.Height);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, m_MipLevels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Texture->Data.Width, m_Texture->Data.Height, GetPixelFormat(m_Texture->Data.Bpp), GL_UNSIGNED_BYTE, nullptr);

		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	// ------------------------------------------------------
	// -- OpenGLTextureCube
	// ------------------------------------------------------

	OpenGLTextureCube::OpenGLTextureCube(uint32_t faceSize)
		: m_RendererID(0), m_FaceSize(faceSize), m_MipLevels(0)
	{
		m_Texture = std::make_shared<TextureCube>();

		m_Texture->Data.Width = m_FaceSize;
		m_Texture->Data.Height = m_FaceSize;
		m_Texture->Data.Bpp = 4;

		CreateEmptyTexture();
	}

	OpenGLTextureCube::OpenGLTextureCube(const std::shared_ptr<TextureCube>& texture)
		: m_RendererID(0), m_FaceSize(0), m_Texture(texture)
	{
		TTEX_CORE_ASSERT(m_Texture->Data.Width == m_Texture->Data.Height, "OpenGLTextureCube:non square faces");
		m_FaceSize = m_Texture->Data.Width;
		m_MipLevels = CalculateMipMapCount(m_FaceSize, m_FaceSize);

		CreateTextureCommand();
	}

	void OpenGLTextureCube::Bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
	}

	void OpenGLTextureCube::CreateTextureCommand()
	{
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

		GLenum internalFormat = GetInternalFormat(m_Texture->Data.Bpp);
		GLenum pixelFormat = GetPixelFormat(m_Texture->Data.Bpp);

		for (uint32_t i = 0; i < m_Texture->Images.size(); i++)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, m_FaceSize, m_FaceSize, 0, pixelFormat, GL_UNSIGNED_BYTE, m_Texture->Images[(int32_t)tTexture::Face::POS_X + i].Data);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLTextureCube::CreateEmptyTexture()
	{
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_MipLevels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

		GLenum internalFormat = GetInternalFormat(m_Texture->Data.Bpp);
		GLenum pixelFormat = GetPixelFormat(m_Texture->Data.Bpp);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalFormat, m_FaceSize, m_FaceSize, 0, pixelFormat, GL_UNSIGNED_BYTE, nullptr);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalFormat, m_FaceSize, m_FaceSize, 0, pixelFormat, GL_UNSIGNED_BYTE, nullptr);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalFormat, m_FaceSize, m_FaceSize, 0, pixelFormat, GL_UNSIGNED_BYTE, nullptr);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalFormat, m_FaceSize, m_FaceSize, 0, pixelFormat, GL_UNSIGNED_BYTE, nullptr);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalFormat, m_FaceSize, m_FaceSize, 0, pixelFormat, GL_UNSIGNED_BYTE, nullptr);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalFormat, m_FaceSize, m_FaceSize, 0, pixelFormat, GL_UNSIGNED_BYTE, nullptr);

		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLTextureCube::GenerateMips() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	std::shared_ptr<TextureCube> OpenGLTextureCube::ConvertToTextureCube()
	{
		std::shared_ptr<TextureCube> result = std::make_shared<TextureCube>();
		result->Data.Width = m_FaceSize;
		result->Data.Height = m_FaceSize;
		result->Data.Bpp = m_Texture->Data.Bpp;

		uint32_t allocationSize = m_FaceSize * m_FaceSize * m_Texture->Data.Bpp;

		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
		for (uint32_t i = 0; i < 6; i++)
		{
			result->Images[i].Allocate(allocationSize);
			glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_Texture->Data.Bpp == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, result->Images[i].Data);
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		return result;
	}

}
