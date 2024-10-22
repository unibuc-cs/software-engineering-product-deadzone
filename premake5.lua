workspace "DeadZone"
    architecture "x64"
    startproject "DeadZone"

    configurations
    {
        "Debug",
        "Release"
    }

    -- Ouput directories for bin and intermediate files
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    -- Include directories relative to root folder (solution directory)
    IncludeDir = {}
    IncludeDir["glad"] = "vendor/glad/include"
    IncludeDir["glfw"] = "vendor/glfw/include"
    IncludeDir["glm"] = "vendor/glm"
    IncludeDir["stb"] = "vendor/stb"
    IncludeDir["freetype"] = "vendor/freetype/include"
    IncludeDir["json"] = "vendor/json/include"
    IncludeDir["fmod"] = "vendor/fmod/include"
    IncludeDir["enet"] = "vendor/enet/include"



    project "glfw"
        location "bin-int/project-files"
        kind "StaticLib"
        language "C"
        
        targetdir("bin/" .. outputdir .. "/%{prj.name}")
        objdir("bin-int/" .. outputdir .. "/%{prj.name}")

        files
        {
            "vendor/glfw/include/GLFW/glfw3.h",
            "vendor/glfw/include/GLFW/glfw3native.h",
            "vendor/glfw/src/**.c",
            "vendor/glfw/src/**.h"
        }

        filter "system:windows"
            systemversion "latest"
            staticruntime "On"

            defines 
            { 
                "_GLFW_WIN32",
                "_CRT_SECURE_NO_WARNINGS"
            }

        filter "configurations:Debug"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            runtime "Release"
            optimize "on"



    project "glad"
        location "bin-int/project-files"
        kind "StaticLib"
        language "C"
        
        targetdir("bin/" .. outputdir .. "/%{prj.name}")
        objdir("bin-int/" .. outputdir .. "/%{prj.name}")

        files
        {
            "vendor/glad/src/glad.c"
        }

        includedirs
        {
            "%{IncludeDir.glad}"
        }

        filter "system:windows"
            systemversion "latest"
            staticruntime "On"

            defines 
            { 
                "_GLAD_WIN32",
                "_CRT_SECURE_NO_WARNINGS"
            }

        filter "configurations:Debug"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            runtime "Release"
            optimize "on"



    project "freetype"
        location "bin-int/project-files"
        kind "StaticLib"
        language "C"

        targetdir("bin/" .. outputdir .. "/%{prj.name}")
        objdir("bin-int/" .. outputdir .. "/%{prj.name}")
        
        files
        {
            "vendor/freetype/src/autofit/autofit.c",
            "vendor/freetype/src/bdf/bdf.c",
            "vendor/freetype/src/cff/cff.c",
            "vendor/freetype/src/dlg/dlgwrap.c",
            "vendor/freetype/src/base/ftbase.c",
            "vendor/freetype/src/cache/ftcache.c",
            "vendor/freetype/src/gzip/ftgzip.c",
            "vendor/freetype/src/base/ftinit.c",
            "vendor/freetype/src/lzw/ftlzw.c",
            "vendor/freetype/src/pcf/pcf.c",
            "vendor/freetype/src/pfr/pfr.c",
            "vendor/freetype/src/psaux/psaux.c",
            "vendor/freetype/src/pshinter/pshinter.c",
            "vendor/freetype/src/psnames/psmodule.c",
            "vendor/freetype/src/raster/raster.c",
            "vendor/freetype/src/sdf/sdf.c",
            "vendor/freetype/src/sfnt/sfnt.c",
            "vendor/freetype/src/smooth/smooth.c",
            "vendor/freetype/src/svg/svg.c",
            "vendor/freetype/src/truetype/truetype.c",
            "vendor/freetype/src/type1/type1.c",
            "vendor/freetype/src/cid/type1cid.c",
            "vendor/freetype/src/type42/type42.c",
            "vendor/freetype/src/winfonts/winfnt.c",

            "vendor/freetype/src/base/ftbbox.c",
            "vendor/freetype/src/base/ftbdf.c",
            "vendor/freetype/src/base/ftbitmap.c",
            "vendor/freetype/src/base/ftcid.c",
            "vendor/freetype/src/base/ftfstype.c",
            "vendor/freetype/src/base/ftgasp.c",
            "vendor/freetype/src/base/ftglyph.c",
            "vendor/freetype/src/base/ftgxval.c",
            "vendor/freetype/src/base/ftmm.c",
            "vendor/freetype/src/base/ftotval.c",
            "vendor/freetype/src/base/ftpatent.c",
            "vendor/freetype/src/base/ftpfr.c",
            "vendor/freetype/src/base/ftstroke.c",
            "vendor/freetype/src/base/ftsynth.c",
            "vendor/freetype/src/base/fttype1.c",
            "vendor/freetype/src/base/ftwinfnt.c"
        }

        includedirs
        {
            "%{IncludeDir.freetype}"
        }

        defines
        {
            "FT2_BUILD_LIBRARY"
        }

        filter "system:windows"
            systemversion "latest"
            staticruntime "On"

            defines 
            { 
                "_CRT_SECURE_NO_WARNINGS"
            }

            files 
            {
                "vendor/freetype/builds/windows/ftsystem.c",
                "vendor/freetype/builds/windows/ftdebug.c"
            }

        filter "configurations:Debug"
            runtime "Debug"
            symbols "on"

            defines 
            {
                "FT_DEBUG_LEVEL_ERROR",
                "FT_DEBUG_LEVEL_TRACE"
            }

        filter "configurations:Release"
            runtime "Release"
            optimize "on"

            defines
            {
                "NDEBUG"
            }



    project "enet"
        location "bin-int/project-files"
        kind "StaticLib"
        language "C"

        targetdir("bin/" .. outputdir .. "/%{prj.name}")
        objdir("bin-int/" .. outputdir .. "/%{prj.name}")

        files
        {
            "vendor/enet/**.c"
        }

        includedirs
        {
            "%{IncludeDir.enet}"
        }

        filter "system:windows"
            links
            {
                "ws2_32",
                "winmm"
            }
            
        filter "configurations:Debug"
            targetsuffix "64d"
            symbols "On"
            
            defines
            {
                "DEBUG"
            }
        
        filter "configurations:Release"
            targetsuffix "64"
            optimize "On"
            
            defines
            {
                "NDEBUG"
            }



    project "DeadZone"
        location "DeadZone"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"

        targetdir("bin/" .. outputdir .. "/%{prj.name}")
        objdir("bin-int/" .. outputdir .. "/%{prj.name}")

        files
        {
            "%{prj.name}/**.h",
            "%{prj.name}/**.cpp",
            "%{prj.name}/**.hpp",
            "%{prj.name}/**.vert",
            "%{prj.name}/**.frag",
            "%{prj.name}/**.map",
            "%{prj.name}/**.ini",
            "%{prj.name}/**.json"
        }

        includedirs
        {
            "%{IncludeDir.glad}",
            "%{IncludeDir.glfw}",
            "%{IncludeDir.glm}",
            "%{IncludeDir.stb}",
            "%{IncludeDir.freetype}",
            "%{IncludeDir.enet}",
            "%{IncludeDir.json}",
            "%{IncludeDir.fmod}"
        }

        libdirs
        {
            "vendor/fmod/lib/x64"
        }

        links
        {
            "glfw",
            "glad",
            "freetype",
            "enet",
            "fmod_vc.lib",
            "fmodL_vc.lib"
        }

        postbuildcommands 
        {
            '{COPY} "%{path.getabsolute("vendor/fmod/lib/x64/fmod.dll")}" "%{cfg.targetdir}"',
            '{COPY} "%{path.getabsolute("vendor/fmod/lib/x64/fmodL.dll")}" "%{cfg.targetdir}"'
        }

        filter "system:windows"
            staticruntime "On"
            systemversion "latest"

        filter "configurations:Debug"
            symbols "On"

            defines
            {
                "DEBUG"
            }

        filter "configurations:Release"
            optimize "On"

            defines
            {
                "NDEBUG"
            }



    project "Unit-Tests"
        location "Unit-Tests"
        kind "SharedLib"
        language "C++"
        cppdialect "C++17"

        targetdir("bin/" .. outputdir .. "/%{prj.name}")
        objdir("bin-int/" .. outputdir .. "/%{prj.name}")

        files
        {
            "%{prj.name}/**.cpp",
            "DeadZone/source/**.h",
            "DeadZone/source/**.cpp",
            "DeadZone/source/**.hpp"
        }

        includedirs
        {
            "$(VCInstallDir)Auxiliary/VS/UnitTest/include",
            "%{IncludeDir.glad}",
            "%{IncludeDir.glfw}",
            "%{IncludeDir.glm}",
            "%{IncludeDir.stb}",
            "%{IncludeDir.freetype}",
            "%{IncludeDir.enet}",
            "%{IncludeDir.json}",
            "%{IncludeDir.fmod}"
        }

        libdirs 
        {
            "$(VCInstallDir)Auxiliary/VS/UnitTest/lib",
            "vendor/fmod/lib/x64"
        }

        links
        {
            "glfw",
            "glad",
            "freetype",
            "enet",
            "fmod_vc.lib",
            "fmodL_vc.lib"
        }

        postbuildcommands 
        {
            '{COPY} "%{path.getabsolute("vendor/fmod/lib/x64/fmod.dll")}" "%{cfg.targetdir}"',
            '{COPY} "%{path.getabsolute("vendor/fmod/lib/x64/fmodL.dll")}" "%{cfg.targetdir}"'
        }

        filter "system:windows"
            staticruntime "On"
            systemversion "latest"

        filter "configurations:Debug"
            symbols "On"

            defines
            {
                "_CRT_SECURE_NO_WARNINGS",
                "DEBUG"
            }

        filter "configurations:Release"
            optimize "On"

            defines
            {
                "NDEBUG"
            }

