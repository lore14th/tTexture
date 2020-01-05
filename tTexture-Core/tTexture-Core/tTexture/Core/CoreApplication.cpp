#include "pch.h"

#include "CoreApplication.h"
#include "Core/CoreLoader.h"

#include "Debug/Timer.h"

namespace tTexture {

#ifdef TTEX_APP
	uint32_t CoreApplication::s_ApplicationCount = 0;
	Debug::Log* CoreApplication::s_Logger = nullptr;

	CoreApplication::CoreApplication(Debug::Log::LogLevel logLevel)
		: CoreApplication()
	{
		

		if (!s_Logger)
			s_Logger->Init(logLevel);
	}

	CoreApplication::~CoreApplication()
	{
		s_ApplicationCount--;

		if (s_ApplicationCount == 0)
			delete s_Logger;
	}
#endif

	std::shared_ptr<Texture2D> CoreApplication::LoadTexture2D(const char* filepath, uint32_t imageChannels, bool flipOnLoad)
	{
		TTEX_TIME_FUNCTION;

		CoreLoader loader(filepath, imageChannels, flipOnLoad);
		return loader.LoadImageFromFile();
	}

	std::shared_ptr<TextureCube> CoreApplication::LoadHCrossFromFile(const char* filepath, uint32_t imageChannels, bool flipOnLoad)
	{
		TTEX_TIME_FUNCTION;

		CoreLoader loader(filepath, imageChannels, flipOnLoad);
		return loader.LoadHCrossFromFile();
	}

	std::shared_ptr<PrefilteredTextureCube> CoreApplication::LoadPrefilteredHCrossFromFile(const char* filepath, uint32_t imageChannels, uint32_t mipCount, bool flipOnLoad)
	{
		TTEX_TIME_FUNCTION;

		CoreLoader loader(filepath, imageChannels, flipOnLoad);
		return loader.LoadPrefilteredTextureHCrossFromFile(mipCount);
	}

}
