--	solution ---------------------------------------------------------------------
workspace "tTexture"
	architecture "x64"
	targetdir "build"
	startproject "tTexture"

	configurations { 
		"Debug", 
		"Release",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--	Include directories relative to root folder (solution directory) -----------
IncludeDir = {}
IncludeDir["GLFW"]		= "tTexture-Core/vendor/GLFW/include"
IncludeDir["Glad"]		= "tTexture-Core/vendor/GLAD/include"
IncludeDir["ImGui"]		= "tTexture-Core/vendor/imgui"
IncludeDir["glm"]		= "tTexture-Core/vendor/glm"
IncludeDir["stb_image"]	= "tTexture-Core/vendor"
IncludeDir["spdlog"]	= "tTexture-Core/vendor/spdlog/include"
-------------------------------------------------------------------------------

group "Dependencies"
	include "tTexture-Core/vendor/GLFW"
	include "tTexture-Core/vendor/GLAD"
	include "tTexture-Core/vendor/imgui"
group ""

project "tTexture-Core"
	location "tTexture-Core"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files { 
		"%{prj.name}/src/tTexture-Core/**.h", 
		"%{prj.name}/src/tTexture-Core/**.c", 
		"%{prj.name}/src/tTexture-Core**.hpp", 
		"%{prj.name}/src/tTexture-Core/**.cpp",
	}

	includedirs {
		"%{prj.name}/src/tTexture-Core",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.stb_image}", --this includes the entire vendor folder
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
	}

	links { 
		"GLFW",
		"Glad",
		"ImGui",
	}

	filter "system:windows"
	systemversion "latest"
	
		defines { 
			"TTEX_PLATFORM_WINDOWS",
			"_CRT_SECURE_NO_WARNINGS",
			"GLFW_INCLUDE_NONE",
		}
		
		links {
			"opengl32.lib"
		}

	filter "configurations:Debug"
		defines "TTEX_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "TTEX_RELEASE"
		runtime "Release"
		optimize "on"

project "tTexture"
	location "tTexture"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	links {
		"tTexture-Core",
	}

	files {
		"tTexture/src/**.h",
		"tTexture/src/**.cpp",
	}
    
	includedirs {
		"%{prj.name}/src",
		"tTexture-Core/src",
		"tTexture-Core/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.spdlog}",
	}
	
	filter "system:windows"
		systemversion "latest"

		defines {
			"TEXT_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "TTEX_DEBUG" 
		runtime "Debug"
		symbols "on"	-- debug version --

	filter "configurations:Release"
		defines "TTEX_RELEASE"
		runtime "Release"
		optimize "on"	-- release version --
