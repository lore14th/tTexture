#pragma once

#ifdef TTEX_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <deque>

#include "Core/tTextureCommon.h"
#include "Core/Buffer.h"

#include "Debug/tTextureLog.h"
#include "Debug/tTextureTimer.h"
