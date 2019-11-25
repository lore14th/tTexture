#pragma once

#include "Core/Log.h"

namespace tTexture {

	static int Initialize()
	{
		// TODO: add initialization flags
		// TODO: return 0 if there are errors during initialization

		Log::Init();
		
		return 1;
	}
}