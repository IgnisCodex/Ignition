project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/build/bin/" .. OUTPUT_DIR .. "/%{prj.name}")
	objdir ("%{wks.location}/build/int/" .. OUTPUT_DIR .. "/%{prj.name}")

	files
	{
		"src/**.hpp",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Ignition/vendor/spdlog/include",
		"%{wks.location}/Ignition/src",
		"%{wks.location}/Ignition/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"Ignition"
	}

    buildoptions {
		"/utf-8"
	}

	filter "system:windows"
		systemversion "latest"

        defines {
            "IG_PLATFORM_WINDOWS",
        }

	filter "configurations:Debug"
		defines "IG_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "IG_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "IG_DIST"
		runtime "Release"
		optimize "on"