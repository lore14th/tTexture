IncludeDir = {}
IncludeDir["GLFW"]		= "tTexture-Editor/vendor/glfw/include"
IncludeDir["Glad"]		= "tTexture-Editor/vendor/GLAD/include"
IncludeDir["ImGui"]		= "tTexture-Editor/vendor/imgui"
IncludeDir["glm"]		= "tTexture-Editor/vendor/glm"
IncludeDir["spdlog"]	= "../tTexture-Core/tTexture-Core/vendor/spdlog/include"

IncludeDir["tTexture_Core"]	= "../tTexture-Core/tTexture-Core"
IncludeDir["tTexture_Core_vendor"] = "../tTexture-Core/tTexture-Core/vendor"

-- Remove these?
--IncludeDir["stb"]		= "tTexture-Core/tTexture-Core/tTexture/vendor"
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
	pchsource "%{prj.name}/tTexture/pch.cpp"

	links {
		"GLFW",
		"Glad",
		"ImGui",

		"tTexture-Core",
	}

	defines {
		"GLFW_INCLUDE_NONE",
	}

	files {
		"%{prj.name}/tTexture/**.h",
		"%{prj.name}/tTexture/**.c",

		"%{prj.name}/tTexture/**.hpp",
		"%{prj.name}/tTexture/**.cpp",
	}

	includedirs {
		"%{prj.name}/tTexture",
		"%{prj.name}/vendor", -- stbi_image_write includes the entire vendor folder
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.spdlog}",

		"%{IncludeDir.tTexture_Core}",
		"%{IncludeDir.tTexture_Core}/tTexture",
		"%{IncludeDir.tTexture_Core_vendor}",
	}
	
	filter "system:windows"
		systemversion "latest"

		defines {
			"TTEX_PLATFORM_WINDOWS"
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
