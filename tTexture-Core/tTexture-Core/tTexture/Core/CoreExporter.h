#pragma once

#include "Core/tTextureCommon.h"

namespace tTexture {

	class CoreExporter
	{
	public:
		enum class OutputFormat { NONE = -1, Tga = 0, Png = 1, Jpg = 2 };
	public:
		CoreExporter(const std::string& outputFilepath);
		~CoreExporter() = default;

		void WriteToDisk(const std::shared_ptr<Texture2D>& texture) const;
		void WriteToDisk(const std::shared_ptr<TextureCube>& texture, CubeFormat outputFormat) const;

		static OutputFormat RetrieveOutputFormat(const std::string& filepath);
	private:
		Face SelectFace(uint32_t faceSize, uint32_t x, uint32_t y) const;

		std::shared_ptr<Texture2D> ConvertTextureCubeToFormat(const std::shared_ptr<TextureCube>& sourceTexture, CubeFormat desiredFormat);

		std::shared_ptr<Texture2D> ConvertToHCross(const std::shared_ptr<TextureCube>& sourceTexture) const;
		std::shared_ptr<Texture2D> PrepareHCross(const std::shared_ptr<TextureCube>& sourceTexture) const;
		std::shared_ptr<Texture2D> PrepareHCrossNoAlpha(const std::shared_ptr<TextureCube>& sourceTexture) const;

		std::shared_ptr<Texture2D> ConvertToVCross(const std::shared_ptr<TextureCube>& sourceTexture) const;
		std::shared_ptr<Texture2D> ConvertToEquirectangular(const std::shared_ptr<TextureCube>& sourceTexture) const;

	private: // stbi depended functions
		void StoreTGAStbi(const std::shared_ptr<Texture2D>& texture) const;
		void StorePngStbi(const std::shared_ptr<Texture2D>& texture) const;
		void StoreJpgStbi(const std::shared_ptr<Texture2D>& texture) const;
	private:
		std::string m_Filepath;
		OutputFormat m_OutputFormat;

		const uint32_t m_jpgMaxQuality = 100;
	};

}
