-- This file compiles tTexture and the GUI editor

-- If you just want to compile tTexture-Core, or if you want to link the source code 
-- with your application, the CoreLibrary branch

workspace "tTexture"
	architecture "x64"
	targetdir "build"
	startproject "tTexture-Editor"
	location ""

	configurations { 
		"Debug", 
		"Profile",
		"Release",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "tTexture-Editor"
