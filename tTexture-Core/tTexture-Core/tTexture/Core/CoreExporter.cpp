#include "pch.h"
#include "CoreExporter.h"
#include "Debug/tTextureAssert.h"

#include <stb_image/stb_image_write.h>

namespace tTexture {

	CoreExporter::CoreExporter(const std::string& outputFilepath)
		: m_Filepath(outputFilepath)
	{
		m_OutputFormat = RetrieveOutputFormat(outputFilepath);
		TTEX_CORE_ASSERT(m_OutputFormat != OutputFormat::NONE, "Exporter: invalid output format. Choose a different file extension");
	}

	void CoreExporter::WriteToDisk(const std::shared_ptr<Texture2D>& texture) const
	{
		TTEX_CORE_ASSERT(m_OutputFormat != OutputFormat::NONE, "Exporter: invalid output format");
		StoreInternal(texture);
	}

	void CoreExporter::WriteToDisk(const std::shared_ptr<TextureCube>& texture) const
	{
		TTEX_CORE_ASSERT(m_OutputFormat != OutputFormat::NONE, "Exporter: invalid output format");
		const uint32_t bpp = m_OutputFormat == OutputFormat::Tga ? 3 : 4;
		StoreInternal(ConvertToHCross(texture, bpp));
	}

	void CoreExporter::WriteToDisk(const std::shared_ptr<PrefilteredTextureCube> textures) const
	{
		std::shared_ptr<Texture2D> prefilteredCrossTexture = ConvertToPrefilteredCross(textures);
		StoreInternal(prefilteredCrossTexture);
	}

	std::shared_ptr<tTexture::Texture2D> CoreExporter::ConvertToHCross(const std::shared_ptr<TextureCube>& sourceTexture, const uint32_t bpp) const
	{
		TTEX_CORE_ASSERT(sourceTexture->Data.Width == sourceTexture->Data.Height, "Exporter: TextureCube cannot be converted. Non square faces");
		const uint32_t faceSize = sourceTexture->Data.Width;
		const int32_t width = 4 * faceSize;
		const int32_t height = 3 * faceSize;

		std::shared_ptr<Texture2D> result = std::make_shared<Texture2D>(width, height, bpp);
		result->Image.Allocate(width * height * bpp);
		result->Image.ZeroInitialze(); // write black to every pixel

		for (int32_t y = 0; y < height; y++)
		{
			for (int32_t x = 0; x < width; x++)
			{
				Face face = SelectFace(x, y, faceSize);
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

	std::shared_ptr<tTexture::Texture2D> CoreExporter::ConvertToPrefilteredCross(const std::shared_ptr<PrefilteredTextureCube>& textures) const
	{
		const int32_t width = CalculatePrefilterTextureWidth(textures->GetLevelsCount());
		const int32_t height = 3 * textures->GetLevel(0)->Data.Width; // 3 * mipZeroFaceSize
		const uint32_t bpp = textures->GetLevel(0)->Data.Bpp;

		std::shared_ptr<Texture2D> result = std::make_shared<Texture2D>(width, height, bpp);
		result->AllocateTexture();
		result->Image.ZeroInitialze();	// write every pixel black

		uint32_t xOffset = 0;
		for (uint32_t i = 0; i < textures->GetLevelsCount(); i++)
		{
			int32_t mipFaceSize = textures->GetLevel(i)->Data.Width;
			std::shared_ptr<Texture2D> mipHCross = ConvertToHCross(textures->GetLevel(i), bpp);
			InsertMipCross(mipHCross, mipFaceSize, result, xOffset);
			xOffset += 4 * mipFaceSize;
		}
		return result;
	}

	void CoreExporter::InsertMipCross(const std::shared_ptr<Texture2D>& mipTexture, uint32_t FS, std::shared_ptr<Texture2D>& result, uint32_t xOffset) const
	{
		const int32_t faceSize = mipTexture->Data.Width / 4;
		TTEX_CORE_ASSERT(faceSize == mipTexture->Data.Height / 3, "Non square faces");
		const byte bpp = mipTexture->Data.Bpp;
		TTEX_CORE_ASSERT(bpp == result->Data.Bpp, "Not same bpp");

		const int32_t xEndPoint = 4 * faceSize;
		const int32_t yEndPoint = 3 * faceSize;

		for (int32_t y = 0; y < yEndPoint; y++)
		{
			for (int32_t x = 0; x < xEndPoint; x++)
			{
				for (byte channel = 0; channel < bpp; channel++)
					result->Image[(x + xOffset + y * result->Data.Width) * bpp + channel] = mipTexture->Image[(x + y * mipTexture->Data.Width) * bpp + channel];
			}
		}
	}

	void CoreExporter::StoreInternal(const std::shared_ptr<Texture2D>& texture) const
	{
		switch (m_OutputFormat)
		{
		case OutputFormat::Tga: StoreTGAStbi(texture);		break;
		case OutputFormat::Png:	StorePngStbi(texture);		break;
		case OutputFormat::Jpg: StoreJpgStbi(texture);		break;
		default: TTEX_CORE_ERROR("Exporter:Texture not exported, invalid output format");
		}
	}

	void CoreExporter::StoreTGAStbi(const std::shared_ptr<Texture2D>& texture) const
	{
		std::shared_ptr<Texture2D> outTexture = RemoveAlphaChannel(texture);
		int32_t status = stbi_write_tga(m_Filepath.c_str(), outTexture->Data.Width, outTexture->Data.Height, outTexture->Data.Bpp, outTexture->Image.Data);
		TTEX_CORE_ASSERT(status != 0, "Exporter:Failed to write tga texture");

		TTEX_CORE_INFO("Exporter:TGA texture exporter to {0}", m_Filepath);
	}

	void CoreExporter::StorePngStbi(const std::shared_ptr<Texture2D>& texture) const
	{
		int32_t stride = texture->Data.Width * texture->Data.Bpp;
		int32_t status = stbi_write_png(m_Filepath.c_str(), texture->Data.Width, texture->Data.Height, texture->Data.Bpp, texture->Image.Data, stride);
		TTEX_CORE_ASSERT(status != 0, "Exporter:Failed to write png texture");

		TTEX_CORE_INFO("Exporter:PNG texture exporter to {0}", m_Filepath);
	}

	void CoreExporter::StoreJpgStbi(const std::shared_ptr<Texture2D>& texture) const
	{
		int32_t status = stbi_write_jpg(m_Filepath.c_str(), texture->Data.Width, texture->Data.Height, texture->Data.Bpp, texture->Image.Data, m_jpgMaxQuality);
		TTEX_CORE_ASSERT(status != 0, "Exporter:Failed to write jpg texture");

		TTEX_CORE_INFO("Exporter:JPG texture exporter to {0}", m_Filepath);
	}

}
