#pragma once

#ifdef TTEX_APP
	#include "Debug/Log.h"
#endif

namespace tTexture {

	// TODO: add proper documentation since this should be the client API

	class CoreApplication
	{
	public:
#ifdef TTEX_APP
		CoreApplication(Debug::Log::LogLevel logLevel = Debug::Log::LogLevel::Trace);
		virtual ~CoreApplication();

#else
		CoreApplication() = default;
		~CoreApplication() = default;
#endif

		std::shared_ptr<Texture2D> LoadTexture2D(const char* filepath, uint32_t imageChannels, bool flipOnLoad = false);
		std::shared_ptr<TextureCube> LoadHCrossFromFile(const char* filepath, uint32_t imageChannels, bool flipOnLoad = false);
		std::shared_ptr<PrefilteredTextureCube> LoadPrefilteredHCrossFromFile(const char* filepath, uint32_t imageChannels, uint32_t mipCount, bool flipOnLoad = false);
	private:
#ifdef TTEX_APP
		static uint32_t s_ApplicationCount;
		static Debug::Log* s_Logger;
#endif
	};

}