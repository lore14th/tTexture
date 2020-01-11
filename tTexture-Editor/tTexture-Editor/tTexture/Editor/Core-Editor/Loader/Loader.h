#pragma once

#include <tTexture/tTexture-Core.h>

namespace tTexture {

	class OpenGLRenderer;
	
	class VCrossLoader
	{
	public:
		VCrossLoader(const std::string& filepath);
		~VCrossLoader() = default;

		std::shared_ptr<TextureCube> LoadVCrossFromFile() const;
	private:
		std::string m_Filepath;
	};

	class EquirectangularLoader
	{
	public:
		EquirectangularLoader(const std::string& filepath, const std::unique_ptr<OpenGLRenderer>& renderer);
		~EquirectangularLoader() = default;

		std::shared_ptr<TextureCube> LoadEquirectangularFromFile() const;
	private:
		const std::unique_ptr<OpenGLRenderer>& m_Renderer;
		std::string m_Filepath;
	};

}