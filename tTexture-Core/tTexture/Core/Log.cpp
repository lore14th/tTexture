#include "pch.h"
#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace tTexture {

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init(LogLevel logLevel)
	{
		// [timestamp] loggerName: message
		spdlog::set_pattern("%^[%T] %n: %v%$");

		// setting up the core logger
		s_CoreLogger = spdlog::stdout_color_mt("tTexture-Core");
		s_CoreLogger->set_level(GetLogLevel(logLevel));

		// setting up the application logger
		s_ClientLogger = spdlog::stdout_color_mt("tTexture-App");
		s_ClientLogger->set_level(GetLogLevel(logLevel));

		TTEX_CORE_TRACE("Log:Initialized");
	}

	spdlog::level::level_enum Log::GetLogLevel(LogLevel level)
	{
		switch (level)
		{
			case LogLevel::Fatal: return spdlog::level::critical; break;
			case LogLevel::Error: return spdlog::level::err;   	  break;
			case LogLevel::Warn: return spdlog::level::warn;	  break;
			case LogLevel::Info: return spdlog::level::info;	  break;
			case LogLevel::Trace: return spdlog::level::trace;	  break;
		}
		std::cout << "Error: Invalid LogLevel. Exit.\n";
		exit(-1);
	}

}