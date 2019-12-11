#include "pch.h"
#include "Exporter.h"

#include <stb/stb_image_write.h>

namespace tTexture {

	Exporter::Exporter(const std::string& outputFilepath)
		: m_Filepath(outputFilepath)
	{
		m_OutputFormat = RetrieveOutputFormat(outputFilepath);
		TTEX_CORE_ASSERT(m_OutputFormat != OutputFormat::NONE, "Exporter: invalid output format. Choose a different file extension");
	}

	void Exporter::WriteToDisk(const std::shared_ptr<Texture2D>& texure)
	{
		TTEX_CORE_ASSERT(m_OutputFormat != OutputFormat::NONE, "Exporter: invalid output format");

		switch (m_OutputFormat)
		{
			case OutputFormat::Tga: StoreTGAStbi(texure);		break;
			//case OutputFormat::Png:	
		}
	}

	void Exporter::WriteToDisk(const std::shared_ptr<TextureCube>& texture)
	{
		TTEX_CORE_ASSERT(m_OutputFormat != OutputFormat::NONE, "Exporter: invalid output format");

		// TODO: Support multiple output formats
		std::shared_ptr<Texture2D> convertedTexture = PrepareHCross(texture);

		switch (m_OutputFormat)
		{
			case OutputFormat::Tga: StoreTGAStbi(convertedTexture);		break;
			//case OutputFormat::Png:	
		}	
	}
	
	Exporter::OutputFormat Exporter::RetrieveOutputFormat(const std::string& filepath)
	{
		size_t extensionFlag = filepath.find_last_of(".");
		std::string_view extension(filepath.c_str() + extensionFlag, filepath.length() - extensionFlag);

		// TODO: Support multiple output formats
		if (extension == ".tga")
			return OutputFormat::Tga;
		//else if ...

		return OutputFormat::NONE;
	}

	std::shared_ptr<Texture2D> Exporter::PrepareHCross(const std::shared_ptr<TextureCube>& sourceTexture)
	{
		TTEX_CORE_ASSERT(sourceTexture->Data.Bpp == 4, "Exporter: sourceTexture must have bpp set to 4. bpp: {0}", sourceTexture->Data.Bpp);
		TTEX_CORE_ASSERT(sourceTexture->Data.Width == sourceTexture->Data.Height, "Exporter: TextureCube cannot be converted. Non square faces");

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
					
					for(byte channel = 0; channel < bpp; channel++)
						result->Image[(x + y * width) * bpp + channel] = sourceTexture->Images[(int32_t)face][(readCoords.first + readCoords.second * faceSize) * sourceTexture->Data.Bpp + channel];
				}
			}
		}

		return result;
	}

	Face Exporter::SelectFace(uint32_t faceSize, uint32_t x, uint32_t y)
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

	void Exporter::StoreTGAStbi(const std::shared_ptr<Texture2D>& texture)
	{
		std::shared_ptr<Texture2D> outputTexture = RemoveAlphaChannel(texture);
		stbi_write_tga(m_Filepath.c_str(), outputTexture->Data.Width, outputTexture->Data.Height, outputTexture->Data.Bpp, outputTexture->Image.Data);
	}

}

