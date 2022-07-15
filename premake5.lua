workspace "Ullmannite"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--====================================================================

project "GLFW"
    kind "StaticLib"
    language "C"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-obj/" .. outputdir .. "/%{prj.name}")

	files
	{
        "Ullmannite/ThirdPartyLibs/glfw/include/GLFW/glfw3.h",
        "Ullmannite/ThirdPartyLibs/glfw/include/GLFW/glfw3native.h",
        "Ullmannite/ThirdPartyLibs/glfw/src/glfw_config.h",
        "Ullmannite/ThirdPartyLibs/glfw/src/context.c",
        "Ullmannite/ThirdPartyLibs/glfw/src/init.c",
        "Ullmannite/ThirdPartyLibs/glfw/src/input.c",
        "Ullmannite/ThirdPartyLibs/glfw/src/monitor.c",
        "Ullmannite/ThirdPartyLibs/glfw/src/platform.c",
        "Ullmannite/ThirdPartyLibs/glfw/src/vulkan.c",
        "Ullmannite/ThirdPartyLibs/glfw/src/window.c",
        "Ullmannite/ThirdPartyLibs/glfw/src/egl_context.c",
        "Ullmannite/ThirdPartyLibs/glfw/src/null_init.c",
        "Ullmannite/ThirdPartyLibs/glfw/src/null_joystick.c",
        "Ullmannite/ThirdPartyLibs/glfw/src/null_monitor.c",
        "Ullmannite/ThirdPartyLibs/glfw/src/null_window.c"
	}

    filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		files
		{
            "Ullmannite/ThirdPartyLibs/glfw/src/win32_init.c",
            "Ullmannite/ThirdPartyLibs/glfw/src/win32_joystick.c",
            "Ullmannite/ThirdPartyLibs/glfw/src/win32_monitor.c",
            "Ullmannite/ThirdPartyLibs/glfw/src/win32_time.c",
            "Ullmannite/ThirdPartyLibs/glfw/src/win32_thread.c",
            "Ullmannite/ThirdPartyLibs/glfw/src/win32_window.c",
            "Ullmannite/ThirdPartyLibs/glfw/src/win32_module.c",
            "Ullmannite/ThirdPartyLibs/glfw/src/wgl_context.c",
            "Ullmannite/ThirdPartyLibs/glfw/src/osmesa_context.c"
		}

		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "system:linux"
		pic "On"

		systemversion "latest"
		staticruntime "On"

        files
        {
            "Ullmannite/ThirdPartyLibs/glfw/src/x11_init.c",
            "Ullmannite/ThirdPartyLibs/glfw/src/x11_monitor.c",
            "Ullmannite/ThirdPartyLibs/glfw/src/x11_window.c",
            "Ullmannite/ThirdPartyLibs/glfw/src/xkb_unicode.c",
            "Ullmannite/ThirdPartyLibs/glfw/src/posix_time.c",
            "Ullmannite/ThirdPartyLibs/glfw/src/posix_thread.c",
            "Ullmannite/ThirdPartyLibs/glfw/src/glx_context.c",
            "Ullmannite/ThirdPartyLibs/glfw/src/osmesa_context.c",
            "Ullmannite/ThirdPartyLibs/glfw/src/linux_joystick.c"
        }

        defines
		{
			"_GLFW_X11"
		}

    filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

--====================================================================

project "Ullmannite"
    location  "Ullmannite"
    kind "ConsoleApp"
    language "C++"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-obj/" .. outputdir .. "/%{prj.name}")

    pchheader "Ullpch.h"
    pchsource "%{prj.name}/src/Ullpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/ThirdPartyLibs/plog/include",
        "%{prj.name}/ThirdPartyLibs/glfw/include"
    }

    links
    {
        "GLFW",
        "opengl32.lib"
    }


    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "RELEASE"
        optimize "On"

--====================================================================

