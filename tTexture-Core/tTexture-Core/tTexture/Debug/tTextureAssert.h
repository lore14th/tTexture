#pragma once

#include "Debug/tTextureLog.h"

#ifdef TTEX_PLATFORM_WINDOWS
	#define TTEX_debugbreak __debugbreak();
#endif

#ifdef TTEX_DEBUG
	#define TTEX_ENABLE_ASSERTS
#endif 

#ifdef TTEX_ENABLE_ASSERTS
	#define TTEX_ASSERT(x, ...)\
				{ if(!(x)) { TTEX_FATAL("Assertion Failed: {0}", __VA_ARGS__); TTEX_debugbreak; } }
	#define TTEX_CORE_ASSERT(x, ...)\
				{ if(!(x)) { TTEX_CORE_FATAL("Assertion Failed: {0}", __VA_ARGS__); TTEX_debugbreak; } }
#else
	#define TTEX_ASSERT(x, ...)
	#define TTEX_CORE_ASSERT(x, ...)
#endif