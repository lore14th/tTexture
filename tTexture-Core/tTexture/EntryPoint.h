#pragma once

#include "Core/Log.h"
#include "Renderer/OpenGLRenderer.h"

namespace tTexture {

	static int Initialize()
	{
		bool returnStatus = true;

		// TODO: add initialization flags
		// TODO: return 0 if there are errors during initialization
		// TODO: [Safety Check] Initialize should be called once, but make sure that init methods are not called by accident multiple types

		bool logStatus = Log::Init(); // safe
		
		return returnStatus && logStatus;
	}

	static void Shutdown()
	{
	}
}