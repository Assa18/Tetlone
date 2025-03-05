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

    targetdir ("bin/" .. outputdir)
    objdir ("obj/" .. outputdir)

    files
    { 
        "Game/src/**.h", 
        "Game/src/**.cpp",
        "Game/src/glad.c",
        "Game/dependencies/include/stb_image/stb_image.h",
        "Game/dependencies/include/stb_image/stb_image.cpp"
    }

    links
    {
        "glfw3.lib",
        "opengl32.lib"
    }

    includedirs
    {
        "Game/src",
        "Game/dependencies/include"
    }

    libdirs
    {
        "Game/dependencies/lib"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
