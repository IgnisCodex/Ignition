project "GLAD"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir ("%{wks.location}/build/bin/" .. OUTPUT_DIR .. "/%{prj.name}")
	objdir ("%{wks.location}/build/int/" .. OUTPUT_DIR .. "/%{prj.name}")

	files {
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c",
	}

    includedirs {
        "include"
    }

    filter "system:windows"
		systemversion "latest"

	
	filter "configurations:Debug"
        symbols "on"
        runtime "Debug"

    filter "configurations:Release"  
        optimize "on"
        runtime "Release"

    filter "configurations:Dist"  
        optimize "on"
        runtime "Release"