-- If you want to compile tTexture-Core, generate the tTexture-Core Build solution using
-- this file and the build script inside the scripts folder. 
-- Compiled .lib file is inside tTexture-Core/bin/{outputdir}/tTexture-Core

-- If you want to link the source code with your application, use the premake5.lua file 
-- inside the tTexutre-Core folder and include it as shown at line 27.

workspace "tTexture-Core Build"
	architecture "x64"
	targetdir "build"
	startproject "tTexture-Core"
	location ""

	configurations { 
		"Debug", 
		"Profile",
		"Release",
	}

-- In order to compile tTexture-Core with your application code, 
-- you need to specify an outputdir. If you don't have one, you can
-- uncomment the default value inside the .lua file in the tTexture-Core folder 
-- targetdir ("bin/" .. outputdir .. "/%{prj.name}") or change this line to whatever you need.
-- Note: this stores the .lib files in the bin folder and organizes them depending on 
-- the outputdir value

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "tTexture-Core"
