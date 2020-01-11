#include "pch.h"

#include "CoreApplication.h"
#include "Core/CoreLoader.h"

#include "Debug/tTextureTimer.h"

namespace tTexture {

#ifdef TTEX_APP
	uint32_t CoreApplication::s_ApplicationCount = 0;
	Debug::Log* CoreApplication::s_Logger = nullptr;
#endif

	CoreApplication::CoreApplication(Debug::Log::LogLevel logLevel)
	{
#ifdef TTEX_APP
		if (!s_Logger)
			s_Logger->Init(logLevel);
#endif
	}

	CoreApplication::~CoreApplication()
	{
#ifdef TTEX_APP
		s_ApplicationCount--;

		if (s_ApplicationCount == 0)
			delete s_Logger;
#endif
	}

	std::shared_ptr<Texture2D> CoreApplication::LoadTexture2D(const char* filepath, bool addAlpha, bool flipOnLoad)
	{
		TTEX_TIME_FUNCTION;

		CoreLoader loader(filepath, flipOnLoad);
		return loader.LoadImageFromFile(addAlpha);
	}

	std::shared_ptr<TextureCube> CoreApplication::LoadHCrossFromFile(const char* filepath)
	{
		TTEX_TIME_FUNCTION;

		CoreLoader loader(filepath, false);
		return loader.LoadHCrossFromFile();
	}

	std::shared_ptr<PrefilteredTextureCube> CoreApplication::LoadPrefilteredHCrossFromFile(const char* baseFilepath)
	{
		TTEX_TIME_FUNCTION;

		CoreLoader loader(baseFilepath, false);
		return loader.LoadPrefilteredTextureHCrossFromFile();
	}

}
