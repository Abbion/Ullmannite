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

project "GLAD"
    kind "StaticLib"
    language "C"
    staticruntime "Off"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-obj/" .. outputdir .. "/%{prj.name}")

	files
	{
        "Ullmannite/ThirdPartyLibs/glad/include/glad/glad.h",
        "Ullmannite/ThirdPartyLibs/glad/include/KHR/khrplatform.h",
        "Ullmannite/ThirdPartyLibs/glad/src/glad.c"
	}

    includedirs
    {
        "Ullmannite/ThirdPartyLibs/glad/include"
    }

    filter "system:windows"
		systemversion "latest"

	filter "system:linux"
		systemversion "latest"

    filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

--====================================================================

project "IMGUI"
    kind "StaticLib"
    language "C"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-obj/" .. outputdir .. "/%{prj.name}")

	files
	{
        "Ullmannite/ThirdPartyLibs/imgui/imconfig.h",
		"Ullmannite/ThirdPartyLibs/imgui/imgui.h",
		"Ullmannite/ThirdPartyLibs/imgui/imgui.cpp",
		"Ullmannite/ThirdPartyLibs/imgui/imgui_draw.cpp",
		"Ullmannite/ThirdPartyLibs/imgui/imgui_internal.h",
		"Ullmannite/ThirdPartyLibs/imgui/imgui_widgets.cpp",
		"Ullmannite/ThirdPartyLibs/imgui/imstb_rectpack.h",
		"Ullmannite/ThirdPartyLibs/imgui/imstb_textedit.h",
		"Ullmannite/ThirdPartyLibs/imgui/imstb_truetype.h",
		"Ullmannite/ThirdPartyLibs/imgui/imgui_demo.cpp",
        "Ullmannite/ThirdPartyLibs/imgui/imgui_tables.cpp",

        --Backend loader
        "Ullmannite/ThirdPartyLibs/imgui/imgui_impl_opengl3_loader.h",

        --OpenGL
        "Ullmannite/ThirdPartyLibs/imgui/imgui_impl_opengl3.h",
        "Ullmannite/ThirdPartyLibs/imgui/imgui_impl_opengl3.cpp",

        --GLFW
        "Ullmannite/ThirdPartyLibs/imgui/imgui_impl_glfw.h",
        "Ullmannite/ThirdPartyLibs/imgui/imgui_impl_glfw.cpp"
	}

    includedirs
    {
        "Ullmannite/ThirdPartyLibs/glfw/include",
    }

    links
    {
        "GLFW"
    }


    filter "system:windows"
        cppdialect "C++17"
		systemversion "latest"
        staticruntime "On"

	filter "system:linux"
        pic "On"
        systemversion "latest"
        cppdialect "C++17"
        staticruntime "On"

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
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/ThirdPartyLibs/glm/**.hpp",
        "%{prj.name}/ThirdPartyLibs/glm/**.inl"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/ThirdPartyLibs/plog/include",
        "%{prj.name}/ThirdPartyLibs/glfw/include",
        "%{prj.name}/ThirdPartyLibs/glad/include",
        "%{prj.name}/ThirdPartyLibs/imgui",
        "%{prj.name}/ThirdPartyLibs/glm"
    }

    links
    {
        "GLFW",
        "GLAD",
        "IMGUI",
        "opengl32.lib"
    }

    defines
    {
        "GLFW_INCLUDE_NONE"
    }


    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "PLATFORM_WINDOWS"
        }

    filter "system:linux"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "PLATFORM_LINUX"
        }

    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"
        buildoptions "/MDd"

    filter "configurations:Release"
        defines "RELEASE"
        optimize "On"
        buildoptions "/MD"

--====================================================================

