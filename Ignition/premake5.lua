project "Ignition"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/build/bin/" .. OUTPUT_DIR .. "/%{prj.name}")
	objdir ("%{wks.location}/build/int/" .. OUTPUT_DIR .. "/%{prj.name}")

	pchheader "IGPCH.hpp"
	pchsource "src/IGPCH.cpp"

	files
	{
		"src/**.hpp",
		"src/**.cpp",
		"vendor/stb/include/stb/**.h",
		"vendor/stb/include/stb/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"src",
		"vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		-- "%{IncludeDir.yaml_cpp}",
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		-- "yaml-cpp",
		"opengl32.lib",
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