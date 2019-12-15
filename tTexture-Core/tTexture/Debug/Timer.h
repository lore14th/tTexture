#include "Core/Log.h"
#include <chrono>

#ifndef TTEX_RELEASE
	#define TTEX_TIME_FUNCTION tTexture::Debug::Timer timer(__FUNCSIG__);
#else
	#define TTEX_TIME_FUNCTION	
#endif

namespace tTexture::Debug {

	struct Timer
	{
		const char* TimerName;
		std::chrono::time_point<std::chrono::steady_clock> Start, End;
		std::chrono::duration<float> Duration;

		Timer(const char* timerName)
			: TimerName(timerName)
		{
			Start = std::chrono::high_resolution_clock::now();
		}

		~Timer()
		{
			End = std::chrono::high_resolution_clock::now();
			Duration = End - Start;

			float ms = Duration.count() * 1000.0f;
			TTEX_CORE_TRACE("Time {0} took {1} ms", TimerName, ms);
		}

	};

}