workspace "Ignition"
    architecture "x64"
    configurations {
        "Debug", "Release", "Dist"
    }
    startproject "Sandbox"

OUTPUT_DIR = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

INCLUDE_DIR = {}
INCLUDE_DIR["GLFW"] = "Ignition/vendor/GLFW/include"
include "Ignition/vendor/GLFW"

project "Ignition"
    location "Ignition"
    kind "SharedLib"
    language "C++"

    targetdir ("build/bin/" .. OUTPUT_DIR .. "/%{prj.name}")
    objdir ("build/int/" .. OUTPUT_DIR .. "/%{prj.name}")

    pchheader "IGPCH.hpp"
    pchsource "%{prj.name}/src/IGPCH.cpp"

    files {
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{INCLUDE_DIR.GLFW}"
    }

    links {
        "GLFW",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "IG_PLATFORM_WINDOWS",
            "IG_BUILD_DLL"
        }

        postbuildcommands {
            {
                "{COPY} %{cfg.buildtarget.relpath} ../build/bin/" .. OUTPUT_DIR .. "/Sandbox"
            }
        }

        buildoptions {
            "/utf-8"
        }

    filter "configurations:Debug"
        defines { "IG_CONFIG_DEBUG" }  
        symbols "On"
        buildoptions "/MDd"

    filter "configurations:Release"  
        defines { "IG_CONFIG_RELEASE" }    
        optimize "On"
        buildoptions "/MD"

    filter "configurations:Dist"  
        defines { "IG_CONFIG_DIST" }    
        optimize "On"
        buildoptions "/MD"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("build/bin/" .. OUTPUT_DIR .. "/%{prj.name}")
    objdir ("build/int/" .. OUTPUT_DIR .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "Ignition/src",
        "Ignition/vendor/spdlog/include"
    }

    links {
        "Ignition"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "IG_PLATFORM_WINDOWS"
        }

        buildoptions {
            "/utf-8"
        }

    filter "configurations:Debug"
        defines { "IG_CONFIG_DEBUG" }  
        symbols "On"
        buildoptions "/MDd"

    filter "configurations:Release"  
        defines { "IG_CONFIG_RELEASE" }    
        optimize "On"
        buildoptions "/MD"

    filter "configurations:Dist"  
        defines { "IG_CONFIG_DIST" }    
        optimize "On"
        buildoptions "/MD"

