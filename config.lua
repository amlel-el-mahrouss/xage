XPLICIT_PROJ = "XPXNgin"

XPLICIT_SERVER_PROJ = "XPXServer"
XPLICIT_CLIENT_PROJ = "XPXPlayer"
XPLICIT_STUDIO_PROJ = "XplicitStudio"

XPLICIT_WORKSPACE = "XplicitNgine"
NPLICIT_PROJ = "XPXNgin.Physics"

XPLICIT_CONFIG = {
    ["_CXXDIALECT"] = 2022,
    ["_CONFIGS"] = { "Debug", "Release" },
    ["_MACROS"] = { "__XPLICIT__", "_CRT_SECURE_NO_WARNINGS", "_WINSOCK_DEPRECATED_NO_WARNINGS", "__XPLICIT_DLL__", "__EXPORT_XPLICIT__" },
    ["_DIALECT"] = "C++",
    ["_FILES"] = {
        ["_INC"] = {
            "XPXNgin/Source/**.h",
            "XPXNgin/Source/**.inl"
        },
        ["_SRC"] = {
            "XPXNgin/Source/**.cpp"
        },
    },
    ["_LIBDIR"] = {
      "C:/Program Files/Mono/lib/mono-2.0-sgen",
	  "dxgi",
	  "d3d11",
	  "d2d1",
	  "d3dcompiler",
	  "ws2_32",
	  "winmm",
	  "opengl32",
	  "C:/VulkanSDK/1.3.250.1/Lib/vulkan-1",
      "Library/glfw3",
      "Library/lua54",
      "Library/Irrlicht",
      "C:/Program Files/OpenSSL-Win64/lib/libssl",
      "C:/Program Files/OpenSSL-Win64/lib/libcrypto",
    },
    ["_INCDIR"] = {
        "C:/Program Files/OpenSSL-Win64/include/",
        "Vendor/",
	    "XPXLua",
	    "Vendor/glfw/include/GLFW/",
    	"Vendor/yaml/include",
    	"XPXNgin.Physics/Source/",
	    "C:/VulkanSDK/1.3.250.1/Include",
	    "Vendor/libzip/lib",
        "C:/Program Files/Mono/include/mono-2.0/",
        "C:/Program Files/Mono/include/mono-2.0/mono",
    }
};

XPLICIT_SERVER_CONFIG = {
    ["_CXXDIALECT"] = 2022,
    ["_CONFIGS"] = { "Debug", "Release" },
    ["_MACROS"] = { "__XPLICIT__", "__SERVER__", "_CRT_SECURE_NO_WARNINGS", "_WINSOCK_DEPRECATED_NO_WARNINGS" },
    ["_DIALECT"] = "C++",
    ["_FILES"] = {
        ["_INC"] = {
            "XPXServer/Source/**.h",
            "XPXServer/Source/**.inl",
        },
        ["_SRC"] = {
            "XPXServer/Source/**.cpp"
        },
    },
    ["_LIBDIR"] = {
        "C:/Program Files/Mono/lib/mono-2.0-sgen",
    	"Library/XPXNgin",
"Library/XPXNgin.Physics",
	    "ws2_32",
	    "winmm",
        "Library/Irrlicht",
        "Library/lua54",
        "C:/Program Files/OpenSSL-Win64/lib/libssl",
        "C:/Program Files/OpenSSL-Win64/lib/libcrypto",
    },
    ["_INCDIR"] = {
        "C:/Program Files/OpenSSL-Win64/include/",
        "Vendor/",
        "Vendor/stb/",
	    "Vendor/glfw/include/GLFW/",
    	"XPXNgin/Source/",
        "XPXLua/",
        "Vendor/nuklear/",
    	"Vendor/yaml/include",
    	"XPXNgin.Physics/Source/",
	    "Vendor/libzip/lib",
	    "C:/VulkanSDK/1.3.250.1/Include",
        "C:/Program Files/Mono/include/mono-2.0/",
        "C:/Program Files/Mono/include/mono-2.0/mono",
    }
};

XPLICIT_CLIENT_CONFIG = {
    ["_CXXDIALECT"] = 2022,
    ["_CONFIGS"] = { "Debug", "Release" },
    ["_MACROS"] = { "__XPLICIT__", "_CRT_SECURE_NO_WARNINGS", "_WINSOCK_DEPRECATED_NO_WARNINGS", "__XPLICIT_CLIENT__" },
    ["_DIALECT"] = "C++",
    ["_FILES"] = {
        ["_INC"] = {
            "XPXPlayer/Source/**.h",
            "XPXPlayer/Source/**.inl",
        },
        ["_SRC"] = {
            "XPXPlayer/Source/**.cpp",
            "XPXPlayer/Source/**.c"
        },
    },
    ["_LIBDIR"] = {
    	"Library/XPXNgin",
"Library/XPXNgin.Physics",
        "d3d11",
        "dxgi",
        "Library/glfw3",
        "d2d1",
        "opengl32",
	    "ws2_32",
	    "winmm",
        "Library/lua54",
        "Library/Irrlicht",
        "C:/Program Files/OpenSSL-Win64/lib/libssl",
        "C:/Program Files/OpenSSL-Win64/lib/libcrypto",
    },
    ["_INCDIR"] = {
        "C:/Program Files/OpenSSL-Win64/include/",
        "Vendor/",
        "XPXNgin.Sound/",
    	"XPXNgin/Source/",
    	"Vendor/irr/include",
        "XPXLua/",
        "Vendor/stb/",
	    "Vendor/luajit/src",
    	"Vendor/yaml/include",
        "Vendor/nuklear/",
	    "Vendor/glfw/include/GLFW/",
    	"XPXNgin.Physics/Source/",
	    "Vendor/libzip/lib",
	    "C:/VulkanSDK/1.3.250.1/Include",
        "C:/Program Files/Mono/include/mono-2.0/",
        "C:/Program Files/Mono/include/mono-2.0/mono",
    }
};

NPLICIT_CONFIG = {
    ["_CXXDIALECT"] = 2022,
    ["_CONFIGS"] = { "Debug", "Release" },
    ["_MACROS"] = { "__NPLICIT__", "_CRT_SECURE_NO_WARNINGS", "_WINSOCK_DEPRECATED_NO_WARNINGS", "__NPLICIT_DLL__", "__EXPORT_NPLICIT__" },
    ["_DIALECT"] = "C++",
    ["_FILES"] = {
        ["_INC"] = {
            "XPXNgin.Physics/Source/**.h",
            "XPXNgin.Physics/Source/**.inl",
        },
        ["_SRC"] = {
            "XPXNgin.Physics/Source/**.cpp",
            "XPXNgin.Physics/Source/**.c"
        },
    },
    ["_LIBDIR"] = {
    	"Library/XPXNgin",
        "d3d11",
      "Library/lua54",
        "dxgi",
    },
    ["_INCDIR"] = {
        "Vendor/",
    	"XPXNgin/Source/",
        "XPXLua/",
        "Vendor/stb/",
	    "Vendor/libzip/lib",
        "Vendor/yaml/include",
	    "Vendor/glfw/include/GLFW/",
        "Vendor/nuklear/",
    }
};

-- END OF CONFIG
