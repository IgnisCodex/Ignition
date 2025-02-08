project "GLAD"
	kind "StaticLib"
	language "C"
	staticruntime "off"
	warnings "off"

	targetdir ("build/bin/" .. OUTPUT_DIR .. "/%{prj.name}")
	objdir ("build/int/" .. OUTPUT_DIR .. "/%{prj.name}")

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