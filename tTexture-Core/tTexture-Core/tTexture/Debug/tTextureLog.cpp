#include "pch.h"
#include "tTextureLog.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace tTexture::Debug {

	bool Log::s_Initialized = false;

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init(LogLevel logLevel)
	{
		if (!s_Initialized && logLevel != LogLevel::None)
		{
			// [timestamp] loggerName: message
			spdlog::set_pattern("%^[%T] %n: %v%$");

			// setting up the core logger
			s_CoreLogger = spdlog::stdout_color_mt("tTexture-Core");
			s_CoreLogger->set_level(GetLogLevel(logLevel));

			// setting up the application logger
			s_ClientLogger = spdlog::stdout_color_mt("tTexture-App");
			s_ClientLogger->set_level(GetLogLevel(logLevel));

			s_Initialized = true;
			TTEX_CORE_TRACE("Log:Initialized");
		}
	}


	spdlog::level::level_enum Log::GetLogLevel(LogLevel level)
	{
#ifdef TTEX_APP
		switch (level)
		{
			case LogLevel::None: return spdlog::level::off; 
			case LogLevel::Fatal: return spdlog::level::critical; 
			case LogLevel::Error: return spdlog::level::err;   	  
			case LogLevel::Warn: return spdlog::level::warn;	  
			case LogLevel::Info: return spdlog::level::info;	  
			case LogLevel::Trace: return spdlog::level::trace;	  
		}
		std::cout << "Error: Invalid LogLevel. Exit.\n";
		exit(-1);
#else
		return spdlog::level::off;
#endif

	}

}