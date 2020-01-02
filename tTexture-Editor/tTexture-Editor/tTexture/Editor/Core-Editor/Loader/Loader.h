#pragma once

#include <tTexture/tTexture-Core.h>

namespace tTexture {

	class OpenGLRenderer;
	
	class VCrossLoader
	{
	public:
		VCrossLoader(const std::string& filepath, uint32_t fileChannels, bool flipOnLoad);
		~VCrossLoader() = default;

		std::shared_ptr<TextureCube> LoadVCrossFromFile() const;
	private:
		std::string m_Filepath;
		uint32_t m_FileChannels;
		bool m_FlipOnLoad;
	};

	class EquirectangularLoader
	{
	public:
		EquirectangularLoader(const std::string& filepath, uint32_t fileChannels, bool flipOnLoad, const std::unique_ptr<OpenGLRenderer>& renderer);
		~EquirectangularLoader() = default;

		std::shared_ptr<TextureCube> LoadEquirectangularFromFile() const;
	private:
		const std::unique_ptr<OpenGLRenderer>& m_Renderer;
		std::string m_Filepath;
		uint32_t m_FileChannels;
		bool m_FlipOnLoad;
	};

}