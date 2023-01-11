workspace "hashtbl"
    configurations { "Debug", "Release", "Sanitizer" }

    includedirs {}
    buildoptions { 
        "-ggdb3",
        "-fPIC",
        "-Wall",
        "-Werror",
        "-Wno-strict-aliasing",
        "-Wno-unused-function",
        "-Wno-unused-variable",
    }
    links {}
    libdirs {}
    language "C"
    kind "ConsoleApp"
    targetprefix ""
    targetdir "."

    project "last10lines"

        files { "**.h", "last10lines_tests.c", "munit.c", "last10lines.c"}
        
    filter "configurations:Sanitizer"
    defines { "DEBUG" }
    symbols "On"
    linkoptions {
        "-fsanitize=address",
    }
    buildoptions { 
        "-fsanitize=address",
    }

    filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"

    filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"
