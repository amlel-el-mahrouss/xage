XPLICIT_PROJ = "Xplicit"
XPLICIT_SERVER_PROJ = "XplicitServer"
XPLICIT_CLIENT_PROJ = "XplicitPlayer"
XPLICIT_STUDIO_PROJ = "XplicitStudio"
XPLICIT_WORKSPACE = "XplicitNgin"
NPLICIT_PROJ = "Nplicit"

XPLICIT_CONFIG = {
    ["_CXXDIALECT"] = 2022,
    ["_CONFIGS"] = { "Debug", "Release" },
    ["_MACROS"] = { "__XPLICIT__", "_CRT_SECURE_NO_WARNINGS", "_WINSOCK_DEPRECATED_NO_WARNINGS", "__XPLICIT_DLL__", "__EXPORT_XPLICIT__" },
    ["_DIALECT"] = "C++",
    ["_FILES"] = {
        ["_INC"] = {
            "Xplicit/Source/**.h",
            "Xplicit/Source/**.inl"
        },
        ["_SRC"] = {
            "Xplicit/Source/**.cpp"
        },
    },
    ["_LIBDIR"] = {
    	  "Vendor/lib/Irrlicht",
        "C:/Program Files/Mono/lib/mono-2.0-sgen",
	  "dxgi",
	  "d3d11",
	  "d2d1",
	  "d3dcompiler",
 	  "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64/d3dx10",
    },
    ["_INCDIR"] = {
        "Vendor/",
	  "Vendor/glfw/include/GLFW/",
    	  "Xplicit/Vendor/include/",
    	  "Vendor/irr/include",
    	  "Nplicit/Source/",
	  "Vendor/libzip/lib",
	  "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include",
        "C:/Program Files/Mono/include/mono-2.0",
    }
};

XPLICIT_SERVER_CONFIG = {
    ["_CXXDIALECT"] = 2022,
    ["_CONFIGS"] = { "Debug", "Release" },
    ["_MACROS"] = { "__XPLICIT__", "__SERVER__", "_CRT_SECURE_NO_WARNINGS", "_WINSOCK_DEPRECATED_NO_WARNINGS" },
    ["_DIALECT"] = "C++",
    ["_FILES"] = {
        ["_INC"] = {
            "XplicitServer/Source/**.h",
            "XplicitServer/Source/**.inl",
        },
        ["_SRC"] = {
            "XplicitServer/Source/**.cpp"
        },
    },
    ["_LIBDIR"] = {
        "C:/Program Files/Mono/lib/mono-2.0-sgen",
    	  "Vendor/lib/Irrlicht",
    	  "bin/Debug/Xplicit",
    },
    ["_INCDIR"] = {
        "Vendor/",
        "Vendor/stb/",
	  "Vendor/glfw/include/GLFW/",
    	  "Xplicit/Source/",
        "Vendor/nuklear/",
    	  "Vendor/irr/include",
    	  "Nplicit/Source/",
	  "Vendor/libzip/lib",
	  "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include",
        "C:/Program Files/Mono/include/mono-2.0",
    }
};

XPLICIT_CLIENT_CONFIG = {
    ["_CXXDIALECT"] = 2022,
    ["_CONFIGS"] = { "Debug", "Release" },
    ["_MACROS"] = { "__XPLICIT__", "_CRT_SECURE_NO_WARNINGS", "_WINSOCK_DEPRECATED_NO_WARNINGS" },
    ["_DIALECT"] = "C++",
    ["_FILES"] = {
        ["_INC"] = {
            "XplicitPlayer/Source/**.h",
            "XplicitPlayer/Source/**.inl",
        },
        ["_SRC"] = {
            "XplicitPlayer/Source/**.cpp",
            "XplicitPlayer/Source/**.c"
        },
    },
    ["_LIBDIR"] = {
    	"Vendor/lib/Irrlicht",
    	"bin/Debug/Xplicit",
	"d3d11",
	"dxgi",
	"d2d1",
 	"C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64/d3dx10",
    },
    ["_INCDIR"] = {
        "Vendor/",
    	  "Xplicit/Source/",
    	  "Xplicit/Vendor/include/",
    	  "Vendor/irr/include",
        "Vendor/lua/",
        "Vendor/stb/",
        "Vendor/nuklear/",
	  "Vendor/glfw/include/GLFW/",
    	  "Nplicit/Source/",
	  "Vendor/libzip/lib",
	  "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include",
        "C:/Program Files/Mono/include/mono-2.0",
    }
};

XPLICIT_STUDIO_CONFIG = {
    ["_CXXDIALECT"] = 2022,
    ["_CONFIGS"] = { "Debug", "Release" },
    ["_MACROS"] = { "__XPLICIT__", "_CRT_SECURE_NO_WARNINGS", "_WINSOCK_DEPRECATED_NO_WARNINGS" },
    ["_DIALECT"] = "C++",
    ["_FILES"] = {
        ["_INC"] = {
            "XplicitStudio/Source/**.h",
            "XplicitStudio/Source/**.inl",
        },
        ["_SRC"] = {
            "XplicitStudio/Source/**.cpp",
            "XplicitStudio/Source/**.c"
        },
    },
    ["_LIBDIR"] = {
    	"bin/Debug/Xplicit",
	"d3d11",
	"dxgi",
	"d2d1",
 	"C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64/d3dx10",
    },
    ["_INCDIR"] = {
        "Vendor/",
    	  "Xplicit/Source/",
    	  "Xplicit/Vendor/include/",
    	  "Vendor/irr/include",
        "Vendor/lua/",
        "Vendor/stb/",
        "Vendor/nuklear/",
	  "Vendor/glfw/include/GLFW/",
	  "Vendor/libzip/lib",
    	  "Nplicit/Source/",
	  "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include",
        "C:/Program Files/Mono/include/mono-2.0",
    }
};

NPLICIT_CONFIG = {
    ["_CXXDIALECT"] = 2022,
    ["_CONFIGS"] = { "Debug", "Release" },
    ["_MACROS"] = { "__NPLICIT__", "_CRT_SECURE_NO_WARNINGS", "_WINSOCK_DEPRECATED_NO_WARNINGS", "__NPLICIT_DLL__", "__EXPORT_NPLICIT__" },
    ["_DIALECT"] = "C++",
    ["_FILES"] = {
        ["_INC"] = {
            "Nplicit/Source/**.h",
            "Nplicit/Source/**.inl",
        },
        ["_SRC"] = {
            "Nplicit/Source/**.cpp",
            "XplicitPlayer/Source/**.c"
        },
    },
    ["_LIBDIR"] = {
    	"Vendor/lib/Irrlicht",
    	"bin/Debug/Xplicit",
	"d3d11",
	"dxgi",
    },
    ["_INCDIR"] = {
        "Vendor/",
    	  "Xplicit/Source/",
    	  "Xplicit/Vendor/include/",
    	  "Vendor/irr/include",
        "Vendor/lua/",
        "Vendor/stb/",
	  "Vendor/libzip/lib",
	  "Vendor/glfw/include/GLFW/",
        "Vendor/nuklear/",
	  "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include",
        "C:/Program Files/Mono/include/mono-2.0",
    }
};

-- END OF CONFIG