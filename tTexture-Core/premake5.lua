project "tTexture-Core"
	location "tTexture-Core"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	-- This can be defined by the premake file of the entire solution
	--outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "tTexture-Core/tTexture/pch.cpp"

	files { 
		"%{prj.name}/tTexture/**.h", 
		"%{prj.name}/tTexture/**.c", 
		"%{prj.name}/tTexture/**.hpp", 
		"%{prj.name}/tTexture/**.cpp",
	}

	includedirs {
		"%{prj.name}/tTexture",

		"%{prj.name}/vendor", 					--stb_image (includes the entire vendor folder)
		"%{prj.name}/vendor/spdlog/include",	-- spdlog
	}

	excludes {
		"%{prj.name}/tTexture/TODO.txt"
	}

	defines {
		-- to load images using a single thread. Do not define if you want to use multi-threading
		--"TTEX_LOADER_SINGLE_THREAD",

		-- this macro is used by the tTexture Editor Application,and enables Log, Timer and Assertions. 
		-- Note: Could be unnecessary and could interfear with your code base.
		"TTEX_APP",

	}

	filter "system:windows"
	systemversion "latest"
	
		defines { 
			"TTEX_PLATFORM_WINDOWS",
			"_CRT_SECURE_NO_WARNINGS",
		}
		
	filter "configurations:Debug"
		defines "TTEX_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Profile"
		defines "TTEX_PROFILE"
		runtime "Release"
		symbols "on"

	filter "configurations:Release"
		defines "TTEX_RELEASE"
		runtime "Release"
		optimize "on"
