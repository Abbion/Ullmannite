workspaceName = "Ullmannite"

workspace (workspaceName)
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
libdir = workspaceName.."/ThirdPartyLibs"

--====================================================================

project "GLFW"
    kind "StaticLib"
    language "C"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-obj/" .. outputdir .. "/%{prj.name}")

    glfw_libdir = libdir.."/glfw"

	files
	{
        glfw_libdir.."/include/GLFW/glfw3.h",
        glfw_libdir.."/include/GLFW/glfw3native.h",
        glfw_libdir.."/src/glfw_config.h",
        glfw_libdir.."/src/internal.h",
        glfw_libdir.."/src/mappings.h",
        glfw_libdir.."/src/platform.h",
        glfw_libdir.."/src/context.c",
        glfw_libdir.."/src/init.c",
        glfw_libdir.."/src/input.c",
        glfw_libdir.."/src/monitor.c",
        glfw_libdir.."/src/platform.c",
        glfw_libdir.."/src/vulkan.c",
        glfw_libdir.."/src/window.c",
        glfw_libdir.."/src/egl_context.c",
        glfw_libdir.."/src/null_init.c",
        glfw_libdir.."/src/null_joystick.c",
        glfw_libdir.."/src/null_monitor.c",
        glfw_libdir.."/src/null_window.c",
        glfw_libdir.."/src/osmesa_context.c",
	}

    filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		files
		{
            glfw_libdir.."/src/win32_init.c",
            glfw_libdir.."/src/win32_joystick.c",
            glfw_libdir.."/src/win32_monitor.c",
            glfw_libdir.."/src/win32_time.c",
            glfw_libdir.."/src/win32_thread.c",
            glfw_libdir.."/src/win32_window.c",
            glfw_libdir.."/src/win32_module.c",
            glfw_libdir.."/src/wgl_context.c"
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
            glfw_libdir.."/src/x11_platform.h",
            glfw_libdir.."/src/x11_init.c",
            glfw_libdir.."/src/x11_monitor.c",
            glfw_libdir.."/src/x11_window.c",
            glfw_libdir.."/src/xkb_unicode.c",
            glfw_libdir.."/src/posix_module.h",
            glfw_libdir.."/src/posix_module.c",
            glfw_libdir.."/src/posix_poll.h",
            glfw_libdir.."/src/posix_poll.c",
            glfw_libdir.."/src/posix_time.c",
            glfw_libdir.."/src/posix_thread.c",
            glfw_libdir.."/src/glx_context.c",
            glfw_libdir.."/src/linux_joystick.h",
            glfw_libdir.."/src/linux_joystick.c"
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

    glad_libdir = libdir.."/glad"

	files
	{
        glad_libdir.."/include/glad/glad.h",
        glad_libdir.."/include/KHR/khrplatform.h",
        glad_libdir.."/src/glad.c"
	}

    includedirs
    {
        glad_libdir.."/include"
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

    imgui_libdir = libdir.."/imgui"

	files
	{
        imgui_libdir.."/imconfig.h",
		imgui_libdir.."/imgui.h",
		imgui_libdir.."/imgui.cpp",
		imgui_libdir.."/imgui_draw.cpp",
		imgui_libdir.."/imgui_internal.h",
		imgui_libdir.."/imgui_widgets.cpp",
		imgui_libdir.."/imstb_rectpack.h",
		imgui_libdir.."/imstb_textedit.h",
		imgui_libdir.."/imstb_truetype.h",
		imgui_libdir.."/imgui_demo.cpp",
        imgui_libdir.."/imgui_tables.cpp",

        --Backend loader
        imgui_libdir.."/imgui_impl_opengl3_loader.h",

        --GLUT
        imgui_libdir.."/imgui_impl_opengl3.h",
        imgui_libdir.."/imgui_impl_opengl3.cpp",

        --GLFW
        imgui_libdir.."/imgui_impl_glfw.h",
        imgui_libdir.."/imgui_impl_glfw.cpp"
	}

    includedirs
    {
        libdir.."/glfw/include",
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
project "FreeType"
    kind "StaticLib"
    language "C"
    
    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-obj/" .. outputdir .. "/%{prj.name}")

    defines
    {
        "FT2_BUILD_LIBRARY",
        "_CRT_SECURE_NO_WARNINGS",
        "_CRT_NONSTDC_NO_WARNINGS",
        "DLL_EXPORT",
    }

    freetype_libdir = libdir.."/freeType"

    includedirs
    {
        freetype_libdir.."/include",
    }

    files
    {
        freetype_libdir.."/src/autofit/autofit.c",
        freetype_libdir.."/src/base/ftbase.c",
        freetype_libdir.."/src/base/ftbbox.c",
        freetype_libdir.."/src/base/ftbdf.c",
        freetype_libdir.."/src/base/ftbitmap.c",
        freetype_libdir.."/src/base/ftcid.c",
        freetype_libdir.."/src/base/ftfstype.c",
        freetype_libdir.."/src/base/ftgasp.c",
        freetype_libdir.."/src/base/ftglyph.c",
        freetype_libdir.."/src/base/ftgxval.c",
        freetype_libdir.."/src/base/ftinit.c",
        freetype_libdir.."/src/base/ftmm.c",
        freetype_libdir.."/src/base/ftotval.c",
        freetype_libdir.."/src/base/ftpatent.c",
        freetype_libdir.."/src/base/ftpfr.c",
        freetype_libdir.."/src/base/ftstroke.c",
        freetype_libdir.."/src/base/ftsynth.c",
        freetype_libdir.."/src/base/fttype1.c",
        freetype_libdir.."/src/base/ftwinfnt.c",
        freetype_libdir.."/src/bdf/bdf.c",
        freetype_libdir.."/src/bzip2/ftbzip2.c",
        freetype_libdir.."/src/cache/ftcache.c",
        freetype_libdir.."/src/cff/cff.c",
        freetype_libdir.."/src/cid/type1cid.c",
        freetype_libdir.."/src/gzip/ftgzip.c",
        freetype_libdir.."/src/lzw/ftlzw.c",
        freetype_libdir.."/src/pcf/pcf.c",
        freetype_libdir.."/src/pfr/pfr.c",
        freetype_libdir.."/src/psaux/psaux.c",
        freetype_libdir.."/src/pshinter/pshinter.c",
        freetype_libdir.."/src/psnames/psnames.c",
        freetype_libdir.."/src/raster/raster.c",
        freetype_libdir.."/src/sdf/sdf.c",
        freetype_libdir.."/src/sfnt/sfnt.c",
        freetype_libdir.."/src/smooth/smooth.c",
        freetype_libdir.."/src/svg/svg.c",
        freetype_libdir.."/src/truetype/truetype.c",
        freetype_libdir.."/src/type1/type1.c",
        freetype_libdir.."/src/type42/type42.c",
        freetype_libdir.."/src/winfonts/winfnt.c",
        freetype_libdir.."/src/winfonts/winfnt.c",
    }

    filter "system:windows"
        systemversion "latest"
        staticruntime "On"
        defines { "_CRT_SECURE_NO_WARNINGS" }
        files
        {
            freetype_libdir.."/builds/windows/ftdebug.c",
            freetype_libdir.."/builds/windows/ftsystem.c",
        }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

--====================================================================
project "GoogleTest"
    kind "StaticLib"
    language "C++"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-obj/" .. outputdir .. "/%{prj.name}")

    gtest_libdir = libdir.."/googleTest/googletest"

    includedirs
    {
        gtest_libdir.."/include",
        gtest_libdir.."",
    }

    files
    {
        gtest_libdir.."/src/*.cc",
    }

    filter "system:windows"
        cppdialect "C++17"
		systemversion "latest"

	filter "system:linux"
        pic "On"
        systemversion "latest"
        cppdialect "C++17"

    filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
        defines { "DEBUG" }

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
        defines { "NDEBUG" }

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
        libdir.."/glm/**.hpp",
        libdir.."/glm/**.inl"
    }

    includedirs
    {
        "%{prj.name}/src",
        libdir.."/plog/include",
        libdir.."/glfw/include",
        libdir.."/glad/include",
        libdir.."/freeType/include",
        libdir.."/imgui",
        libdir.."/glm",
        libdir.."/StbImage/",
        libdir.."/googleTest/googletest/include",
    }

    links
    {
        "GLFW",
        "GLAD",
        "IMGUI",
        "FreeType",
        "GoogleTest",
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

        links
        {
            "opengl32.lib"
        }

    filter "system:linux"
    	flags { "NoPCH" }
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "PLATFORM_LINUX"
        }
        
        links
        {
            "X11"
	}

    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "RELEASE"
        optimize "On"


    filter {"system:windows", "configurations:Debug"}
        buildoptions "/MDd"

    filter {"system:windows", "configurations:Release"}
        buildoptions "/MD"

--====================================================================