OUTPUT_DIR = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "dependencies.lua"

workspace "Ignition"
	architecture "x86_64"
	startproject "Ignition"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

group "Dependencies"
	-- include "vendor/premake"
	include "Ignition/vendor/GLFW"
	include "Ignition/vendor/Glad"
	include "Ignition/vendor/imgui"
	-- include "Ignition/vendor/yaml-cpp"
group ""

group "Core"
	include "Ignition"
group ""

group "Tools"
	include "Flint"
group ""

group "Misc"
	include "Sandbox"
group ""