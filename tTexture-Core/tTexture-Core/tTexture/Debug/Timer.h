#include "Debug/Log.h"
#include <chrono>

#ifdef TTEX_PLATFORM_WINDOWS
	#define TTEX_TIMER_FUNC_NAME __FUNCSIG__
#endif

#ifndef TTEX_RELEASE
	#define TTEX_TIME_FUNCTION tTexture::Debug::Timer timer(TTEX_TIMER_FUNC_NAME);
#else
	#define TTEX_TIME_FUNCTION	
#endif

namespace tTexture::Debug {

	struct Timer
	{
		const char* TimerName;
		std::chrono::time_point<std::chrono::steady_clock> Start, End;

		Timer(const char* timerName)
			: TimerName(timerName)
		{
			Start = std::chrono::high_resolution_clock::now();
		}

		~Timer()
		{
			End = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> Duration = End - Start;

			float ms = Duration.count() * 1000.0f;
			TTEX_CORE_TRACE("Time {0} took {1} ms", TimerName, ms);
		}

	};

}