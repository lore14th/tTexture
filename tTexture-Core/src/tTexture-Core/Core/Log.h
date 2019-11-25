#pragma once

#include "Core/Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace tTexture {

	class Log
	{
	public:
		enum class LogLevel { Fatal, Error, Warn, Info, Trace };

	public:
		static void Init(LogLevel level = LogLevel::Trace);

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

		// converts Tinfoil API LogLevels into spdlog::level
		static spdlog::level::level_enum GetLogLevel(LogLevel level);
	};

#ifdef TTEX_DEBUG
	#define CORE_ERROR(...)		::tTexture::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define CORE_WARN(...)		::tTexture::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define CORE_INFO(...)		::tTexture::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define CORE_TRACE(...)		::tTexture::Log::GetCoreLogger()->trace(__VA_ARGS__)
	#define CORE_FATAL(...)		::tTexture::Log::GetCoreLogger()->critical(__VA_ARGS__)
	
	#define CORE_CHECK(x, ...)	{if(x) {::tTexture::Log::GetCoreLogger()->info(__VA_ARGS__); __debugbreak();}}
	
	#define APP_ERROR(...)		::tTexture::Log::GetClientLogger()->error(__VA_ARGS__)
	#define APP_WARN(...)		::tTexture::Log::GetClientLogger()->warn(__VA_ARGS__)
	#define APP_INFO(...)		::tTexture::Log::GetClientLogger()->info(__VA_ARGS__)
	#define APP_TRACE(...)		::tTexture::Log::GetClientLogger()->trace(__VA_ARGS__)
	#define APP_FATAL(...)		::tTexture::Log::GetClientLogger()->critical(__VA_ARGS__)
#else
	#define CORE_ERROR(...)
	#define CORE_WARN(...)
	#define CORE_INFO(...)
	#define CORE_TRACE(...)
	#define CORE_FATAL(...)
	
	#define CORE_CHECK(x, ...)
	
	#define ERROR(...)	
	#define WARN(...)	
	#define INFO(...)	
	#define TRACE(...)	
	#define FATAL(...)	
#endif

}