XPLICIT_PROJ = "Xplicit"
XPLICIT_SERVER_PROJ = "XplicitServer"
XPLICIT_CLIENT_PROJ = "XplicitPlayer"
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
    },
    ["_INCDIR"] = {
        "Vendor/",
    	  "Xplicit/Vendor/include/",
    	  "Vendor/irr/include",
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
    	  "Xplicit/Source/",
        "Vendor/nuklear/",
    	  "Vendor/irr/include",
    	  "Nplicit/Source/",
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
    },
    ["_INCDIR"] = {
        "Vendor/",
    	"Xplicit/Source/",
    	"Xplicit/Vendor/include/",
    	"Vendor/irr/include",
        "Vendor/lua/",
        "Vendor/stb/",
        "Vendor/nuklear/",
    	  "Nplicit/Source/",
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
        "Vendor/nuklear/",
        "C:/Program Files/Mono/include/mono-2.0",
    }
};

-- END OF CONFIG