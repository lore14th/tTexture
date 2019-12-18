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
		"tTexture-Core/tTexture/**.h", 
		"tTexture-Core/tTexture/**.c", 
		"tTexture-Core/tTexture**.hpp", 
		"tTexture-Core/tTexture/**.cpp",
	}

	includedirs {
		"tTexture-Core/tTexture",

		"tTexture-Core/vendor", 				--stb_image (includes the entire vendor folder)
		"tTexture-Core/vendor/spdlog/include",	-- spdlog
	}

	excludes {
		"%{prj.name}/tTexture/TODO.txt"
	}

	defines {
		--"TTEX_LOADER_SINGLE_THREAD" -- to load images using a single thread. Do not define if you want to use multi-threading
	}

	filter "system:windows"
	systemversion "latest"
	
		defines { 
			"TTEX_PLATFORM_WINDOWS",
			"_CRT_SECURE_NO_WARNINGS",
			"GLFW_INCLUDE_NONE",
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
