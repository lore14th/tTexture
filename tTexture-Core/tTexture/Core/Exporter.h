#pragma once

#include "Core/Core.h"

namespace tTexture {

	class Exporter
	{
	private: // TODO: add support to multiple formats
		enum class OutputFormat { NONE = -1, Tga = 0 };
	public:
		Exporter(const std::string& outputFilepath);
		~Exporter() = default;

		void WriteToDisk(const std::shared_ptr<Texture2D>& texure);
		void WriteToDisk(const std::shared_ptr<TextureCube>& texture);

	private:
		OutputFormat RetrieveOutputFormat(const std::string& filepath);

		std::shared_ptr<Texture2D> PrepareHCross(const std::shared_ptr<TextureCube>& sourceTexture);
		Face SelectFace(uint32_t faceSize, uint32_t x, uint32_t y);
		
	private: // stbi depended functions
		void StoreTGAStbi(const std::shared_ptr<Texture2D>& texture);

	private:
		std::string m_Filepath;
		OutputFormat m_OutputFormat;
	};

}