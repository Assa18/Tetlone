workspace "Tetlone"
    configurations
    { 
        "Debug", 
        "Release" 
    }
    architecture "x64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Game"
    location "Game"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir ("Game/bin/" .. outputdir)
    objdir ("Game/obj/" .. outputdir)

    files
    { 
        "Game/src/**.h", 
        "Game/src/**.cpp"
    }

    links
    {
        "opengl32.lib"
    }

    includedirs
    {
        "Game/src"
    }


    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
