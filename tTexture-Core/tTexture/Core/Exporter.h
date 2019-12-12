#pragma once

#include "Core/Core.h"

namespace tTexture {

	class Exporter
	{
	private:
		enum class OutputFormat { NONE = -1, Tga = 0, Png = 1, Jpg = 2 };
	public:
		Exporter(const std::string& outputFilepath);
		~Exporter() = default;

		void WriteToDisk(const std::shared_ptr<Texture2D>& texture) const;
		void WriteToDisk(const std::shared_ptr<TextureCube>& texture) const;

	private:
		OutputFormat RetrieveOutputFormat(const std::string& filepath) const;
		Face SelectFace(uint32_t faceSize, uint32_t x, uint32_t y) const;

		std::shared_ptr<Texture2D> ConvertToHCross(const std::shared_ptr<TextureCube>& sourceTexture) const;
		std::shared_ptr<Texture2D> PrepareHCross(const std::shared_ptr<TextureCube>& sourceTexture) const;
		std::shared_ptr<Texture2D> PrepareHCrossNoAlpha(const std::shared_ptr<TextureCube>& sourceTexture) const;		
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