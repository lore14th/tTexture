#pragma once

#include "Core/tTextureCommon.h"

namespace tTexture {

	class CoreExporter
	{
	public:
		CoreExporter(const std::string& outputFilepath);
		~CoreExporter() = default;

		void WriteToDisk(const std::shared_ptr<Texture2D>& texture) const;
		void WriteToDisk(const std::shared_ptr<TextureCube>& texture, CubeFormat outputFormat) const;
		void WriteToDisk(const std::shared_ptr<PrefilteredTextureCube> textures) const;

	private:
		std::shared_ptr<Texture2D> ConvertTextureCubeToFormat(const std::shared_ptr<TextureCube>& sourceTexture, CubeFormat desiredFormat) const;
		std::shared_ptr<Texture2D> ConvertToHCross(const std::shared_ptr<TextureCube>& sourceTexture, const uint32_t bpp) const;
		std::shared_ptr<Texture2D> ConvertToVCross(const std::shared_ptr<TextureCube>& sourceTexture, const uint32_t bpp) const;
		std::shared_ptr<Texture2D> ConvertToEquirectangular(const std::shared_ptr<TextureCube>& sourceTexture) const;

	private:
		std::shared_ptr<Texture2D> ConvertToPrefilteredCross(const std::shared_ptr<PrefilteredTextureCube>& textures) const;
		void InsertMipCross(const std::shared_ptr<Texture2D>& mipTexture, uint32_t faceSize, std::shared_ptr<Texture2D>& result, uint32_t xOffset) const;

	private: // stbi depended functions
		void StoreInternal(const std::shared_ptr<Texture2D>& texture) const;

		void StoreTGAStbi(const std::shared_ptr<Texture2D>& texture) const;
		void StorePngStbi(const std::shared_ptr<Texture2D>& texture) const;
		void StoreJpgStbi(const std::shared_ptr<Texture2D>& texture) const;
	private:
		std::string m_Filepath;
		OutputFormat m_OutputFormat;

		const uint32_t m_jpgMaxQuality = 100;
	};

}
