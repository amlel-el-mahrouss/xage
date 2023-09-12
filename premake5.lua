if os.execute('ver') == nil then
	include('config.lua')
	filter { "system:macosx", "action:gmake2" }
		linkoptions { "-framework OpenAL -framework Foundation -framework OpenGL -framework AppKit -framework IOKit -fobjc-arc" }
else
	include('config-windows.lua')
end

 filter "configurations:Debug"
      defines { "_DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "_NDEBUG" }
      optimize "On"

workspace(XPLICIT_WORKSPACE)

if (XPLICIT_CONFIG["_CXXDIALECT"] >= 2022) then
    cppdialect "c++20"
else
    cppdialect "c++17"
end

configurations { XPLICIT_CONFIG["_CONFIGS"] }
project(XPLICIT_PROJ)
defines { XPLICIT_CONFIG["_MACROS"] }
lang = XPLICIT_CONFIG["_DIALECT"]
files { XPLICIT_CONFIG["_FILES"]["_INC"], XPLICIT_CONFIG["_FILES"]["_SRC"] }
includedirs { XPLICIT_CONFIG["_INCDIR"] } 
links{ XPLICIT_CONFIG["_LIBDIR"] } 
architecture = TAGRET

kind ("SharedLib")

configurations { XPLICIT_SERVER_CONFIG["_CONFIGS"] }
project(XPLICIT_SERVER_PROJ)
defines { XPLICIT_SERVER_CONFIG["_MACROS"] }
compileas = XPLICIT_SERVER_CONFIG["_DIALECT"]
files { XPLICIT_SERVER_CONFIG["_FILES"]["_INC"], XPLICIT_SERVER_CONFIG["_FILES"]["_SRC"] }
includedirs { XPLICIT_SERVER_CONFIG["_INCDIR"] } 
links{ XPLICIT_SERVER_CONFIG["_LIBDIR"] } 
architecture = TAGRET

kind ("ConsoleApp")

configurations { XPLICIT_CLIENT_CONFIG["_CONFIGS"] }
project(XPLICIT_CLIENT_PROJ)
defines { XPLICIT_CLIENT_CONFIG["_MACROS"] }
compileas = XPLICIT_CLIENT_CONFIG["_DIALECT"]
files { XPLICIT_CLIENT_CONFIG["_FILES"]["_INC"], XPLICIT_CLIENT_CONFIG["_FILES"]["_SRC"] }
includedirs { XPLICIT_CLIENT_CONFIG["_INCDIR"] } 
links{ XPLICIT_CLIENT_CONFIG["_LIBDIR"] } 
architecture = TAGRET

kind ("WindowedApp")

configurations { NPLICIT_CONFIG["_CONFIGS"] }
project(NPLICIT_PROJ)
defines { NPLICIT_CONFIG["_MACROS"] }
compileas = NPLICIT_CONFIG["_DIALECT"]
files { NPLICIT_CONFIG["_FILES"]["_INC"], NPLICIT_CONFIG["_FILES"]["_SRC"] }
includedirs { NPLICIT_CONFIG["_INCDIR"] } 
links{ NPLICIT_CONFIG["_LIBDIR"] } 
architecture = TAGRET

kind ("SharedLib")