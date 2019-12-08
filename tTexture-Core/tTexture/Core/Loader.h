#pragma once

#include "Core/Core.h"

namespace tTexture {

	class Application;

	class Loader
	{
	public:
		Loader(const std::string& filepath, uint32_t desiredChannels = 3, bool flipOnLoad = false);

		void SetApplicationCallback(Application* app) { m_ApplicationRef = app; }

		std::shared_ptr<Texture2D> LoadImageFromFile();
		// returns a 4 channels image spitted up into cube faces.
		// you can use the Loader::Face enum to access the data inside result.Images
		std::shared_ptr<TextureCube> LoadCubeMapFromFile(CubeFormat format);

		static void FreeImageBuffer(byte* buffer);
	private:
		std::string m_Filepath;
		uint32_t m_DesiredChannels;
		bool m_FlipOnLoad;

		Application* m_ApplicationRef;
	};

}