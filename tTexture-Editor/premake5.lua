IncludeDir = {}
IncludeDir["GLFW"]		= "tTexture-Editor/vendor/glfw/include"
IncludeDir["Glad"]		= "tTexture-Editor/vendor/GLAD/include"
IncludeDir["ImGui"]		= "tTexture-Editor/vendor/imgui"
IncludeDir["glm"]		= "tTexture-Editor/vendor/glm"

-- Remove these?
--IncludeDir["stb"]		= "tTexture-Core/tTexture-Core/tTexture/vendor"
--IncludeDir["spdlog"]	= "tTexture-Core/tTexture-Core/tTexture/vendor/spdlog/include"

IncludeDir["tTexture-Core"]	= "tTexture-Core/tTexture-Core/tTexture"
-------------------------------------------------------------------------------

group "Dependencies"
	include "tTexture-Editor/vendor/GLFW"
	include "tTexture-Editor/vendor/GLAD"
	include "tTexture-Editor/vendor/imgui"

	include "tTexture-Core"
group ""

project "tTexture-Editor"
	location "tTexture-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "tTexture-Editor/tTexture/pch.cpp"

	links {
		"tTexture-Core",
	}

	files {
		"tTexture-Editor/tTexture/**.h",
		"tTexture-Editor/tTexture/**.c",

		"tTexture-Editor/tTexture/**.hpp",
		"tTexture-Editor/tTexture/**.cpp",
	}
    
	includedirs {
		"%{prj.name}/tTexture",
		"../tTexture-Core/tTexture-Core",
		
		"%{prj.name}/vendor", -- stbi_image_write includes the entire vendor folder
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
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

	filter "configurations:Profile"
		defines "TTEX_PROFILE"
		runtime "Release"
		symbols "on"

	filter "configurations:Release"
		defines "TTEX_RELEASE"
		runtime "Release"
		optimize "on"	-- release version --
