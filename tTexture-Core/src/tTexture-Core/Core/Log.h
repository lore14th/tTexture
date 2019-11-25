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
	#define TTEX_CORE_ERROR(...)	::tTexture::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define TTEX_CORE_WARN(...)		::tTexture::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define TTEX_CORE_INFO(...)		::tTexture::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define TTEX_CORE_TRACE(...)	::tTexture::Log::GetCoreLogger()->trace(__VA_ARGS__)
	#define TTEX_CORE_FATAL(...)	::tTexture::Log::GetCoreLogger()->critical(__VA_ARGS__)
	
	#define TTEX_CORE_CHECK(x, ...)	{if(x) {::tTexture::Log::GetCoreLogger()->info(__VA_ARGS__); __debugbreak();}}
	
	#define TTEX_ERROR(...)		::tTexture::Log::GetClientLogger()->error(__VA_ARGS__)
	#define TTEX_WARN(...)		::tTexture::Log::GetClientLogger()->warn(__VA_ARGS__)
	#define TTEX_INFO(...)		::tTexture::Log::GetClientLogger()->info(__VA_ARGS__)
	#define TTEX_TRACE(...)		::tTexture::Log::GetClientLogger()->trace(__VA_ARGS__)
	#define TTEX_FATAL(...)		::tTexture::Log::GetClientLogger()->critical(__VA_ARGS__)
#else
	#define TTEX_CORE_ERROR(...)
	#define TTEX_CORE_WARN(...)
	#define TTEX_CORE_INFO(...)
	#define TTEX_CORE_TRACE(...)
	#define TTEX_CORE_FATAL(...)
	
	#define TTEX_CORE_CHECK(x, ...)
	
	#define TTEX_ERROR(...)	
	#define TTEX_WARN(...)	
	#define TTEX_INFO(...)	
	#define TTEX_TRACE(...)	
	#define TTEX_FATAL(...)	
#endif

}