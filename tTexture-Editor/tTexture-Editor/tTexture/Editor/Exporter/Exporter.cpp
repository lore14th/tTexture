#include "pch.h"
#include "Exporter.h"

#include <stbi_image_write/stb_image_write.h>

namespace tTexture {

	Exporter::Exporter(const std::string& outputFilepath)
		: m_Filepath(outputFilepath)
	{
		m_OutputFormat = RetrieveOutputFormat(outputFilepath);
		TTEX_ASSERT(m_OutputFormat != OutputFormat::NONE, "Exporter: invalid output format. Choose a different file extension");
	}

	void Exporter::WriteToDisk(const std::shared_ptr<Texture2D>& texture) const
	{
		TTEX_ASSERT(m_OutputFormat != OutputFormat::NONE, "Exporter: invalid output format");

		switch (m_OutputFormat)
		{
			case OutputFormat::Tga: StoreTGAStbi(texture);		break;
			case OutputFormat::Png:	StorePngStbi(texture);		break;
			case OutputFormat::Jpg: StoreJpgStbi(texture);		break;
			default: TTEX_ERROR("Exporter:Texture not exported, invalid output format");
		}
	}

	void Exporter::WriteToDisk(const std::shared_ptr<TextureCube>& texture) const
	{
		TTEX_ASSERT(m_OutputFormat != OutputFormat::NONE, "Exporter: invalid output format");
		std::shared_ptr<Texture2D> crossTexture = ConvertToHCross(texture);

		switch (m_OutputFormat)
		{
			case OutputFormat::Tga: StoreTGAStbi(crossTexture);		break;
			case OutputFormat::Png:	StorePngStbi(crossTexture);		break;
			case OutputFormat::Jpg: StoreJpgStbi(crossTexture);		break;
			default: TTEX_ERROR("Exporter:Texture not exported, invalid output format");
		}	
	}
	
	Exporter::OutputFormat Exporter::RetrieveOutputFormat(const std::string& filepath)
	{
		size_t extensionFlag = filepath.find_last_of(".");
		std::string_view extension(filepath.c_str() + extensionFlag, filepath.length() - extensionFlag);

		if (extension == ".tga")
			return OutputFormat::Tga;
		else if (extension == ".png")
			return OutputFormat::Png;
		else if (extension == ".jpg")
			return OutputFormat::Jpg;

		return OutputFormat::NONE;
	}

	std::shared_ptr<tTexture::Texture2D> Exporter::ConvertToHCross(const std::shared_ptr<TextureCube>& sourceTexture) const
	{
		if (m_OutputFormat == OutputFormat::Tga)
			return PrepareHCrossNoAlpha(sourceTexture);
		else
			return PrepareHCross(sourceTexture);
	}

	std::shared_ptr<Texture2D> Exporter::PrepareHCross(const std::shared_ptr<TextureCube>& sourceTexture) const
	{
		TTEX_ASSERT(sourceTexture->Data.Width == sourceTexture->Data.Height, "Exporter: TextureCube cannot be converted. Non square faces");
		std::shared_ptr<Texture2D> result = std::make_shared<Texture2D>();

		const uint32_t faceSize = sourceTexture->Data.Width;
		const int32_t width = 4 * faceSize;
		const int32_t height = 3 * faceSize;
		const int32_t bpp = sourceTexture->Data.Bpp;

		result->Data.Width = width;
		result->Data.Height = height;
		result->Data.Bpp = bpp;
		result->Image.Allocate(width * height * bpp);
		result->Image.ZeroInitialze(); // write black to every pixel

		for (int32_t y = 0; y < height; y++)
		{
			for (int32_t x = 0; x < width; x++)
			{
				Face face = SelectFace(faceSize, x, y);
				if (face != Face::NONE) // read from actual pixels
				{
					std::pair<uint32_t, uint32_t> readCoords = GetCoordinatesRelativeToFace(x, y, faceSize, face);

					for (byte channel = 0; channel < bpp; channel++)
						result->Image[(x + y * width) * bpp + channel] = sourceTexture->Images[(int32_t)face][(readCoords.first + readCoords.second * faceSize) * sourceTexture->Data.Bpp + channel];
				}
			}
		}
		return result;
	}

	std::shared_ptr<tTexture::Texture2D> Exporter::PrepareHCrossNoAlpha(const std::shared_ptr<TextureCube>& sourceTexture) const
	{
		TTEX_ASSERT(sourceTexture->Data.Bpp == 4, "Exporter: sourceTexture must have bpp set to 4. bpp: {0}", sourceTexture->Data.Bpp);
		TTEX_ASSERT(sourceTexture->Data.Width == sourceTexture->Data.Height, "Exporter: TextureCube cannot be converted. Non square faces");

		std::shared_ptr<Texture2D> result = std::make_shared<Texture2D>();

		const uint32_t faceSize = sourceTexture->Data.Width;
		const int32_t width = 4 * faceSize;
		const int32_t height = 3 * faceSize;
		const int32_t bpp = 3;

		result->Data.Width = width;
		result->Data.Height = height;
		result->Data.Bpp = bpp;
		result->Image.Allocate(width * height * bpp);
		result->Image.ZeroInitialze(); // write black to every pixel

		for (int32_t y = 0; y < height; y++)
		{
			for (int32_t x = 0; x < width; x++)
			{
				Face face = SelectFace(faceSize, x, y);
				if (face != Face::NONE) // read from actual pixels
				{
					std::pair<uint32_t, uint32_t> readCoords = GetCoordinatesRelativeToFace(x, y, faceSize, face);

					for (byte channel = 0; channel < bpp; channel++)
						result->Image[(x + y * width) * bpp + channel] = sourceTexture->Images[(int32_t)face][(readCoords.first + readCoords.second * faceSize) * sourceTexture->Data.Bpp + channel];
				}
			}
		}

		return result;
	}

	Face Exporter::SelectFace(uint32_t faceSize, uint32_t x, uint32_t y) const
	{
		if (y < faceSize) // TOP Row
		{
			if (x < faceSize || x > 2 * faceSize)	
				return Face::NONE;
			else 
				return Face::POS_Y;
		}
		else if (y > 2 * faceSize - 1) //BOTTOM Row
		{
			if (x < faceSize || x > 2 * faceSize)
				return Face::NONE;
			else
				return Face::NEG_Y;
		}
		else // Middle Row
		{
			if (x < faceSize)			return Face::POS_X;
			else if (x < 2 * faceSize)	return Face::POS_Z;
			else if (x < 3 * faceSize)	return Face::NEG_X;
			else						return Face::NEG_Z;
		}
	}

	void Exporter::StoreTGAStbi(const std::shared_ptr<Texture2D>& texture) const
	{
		std::shared_ptr<Texture2D> outTexture = RemoveAlphaChannel(texture);
		int32_t status = stbi_write_tga(m_Filepath.c_str(), outTexture->Data.Width, outTexture->Data.Height, outTexture->Data.Bpp, outTexture->Image.Data);
		TTEX_ASSERT(status != 0, "Exporter:Failed to write tga texture");

		TTEX_INFO("Exporter:TGA texture exporter to {0}", m_Filepath);
	}

	void Exporter::StorePngStbi(const std::shared_ptr<Texture2D>& texture) const
	{
		int32_t stride = texture->Data.Width * texture->Data.Bpp;
		int32_t status = stbi_write_png(m_Filepath.c_str(), texture->Data.Width, texture->Data.Height, texture->Data.Bpp, texture->Image.Data, stride);
		TTEX_ASSERT(status != 0, "Exporter:Failed to write png texture");

		TTEX_INFO("Exporter:PNG texture exporter to {0}", m_Filepath);
	}

	void Exporter::StoreJpgStbi(const std::shared_ptr<Texture2D>& texture) const
	{
		int32_t status = stbi_write_jpg(m_Filepath.c_str(), texture->Data.Width, texture->Data.Height, texture->Data.Bpp, texture->Image.Data, m_jpgMaxQuality);
		TTEX_ASSERT(status != 0, "Exporter:Failed to write jpg texture");

		TTEX_INFO("Exporter:JPG texture exporter to {0}", m_Filepath);
	}

}
