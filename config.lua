XPLICIT_PROJ = "Xplicit"

XPLICIT_SERVER_PROJ = "XplicitServer"
XPLICIT_CLIENT_PROJ = "XplicitPlayer"
XPLICIT_STUDIO_PROJ = "XplicitStudio"

XPLICIT_WORKSPACE = "XplicitNgine"
NPLICIT_PROJ = "Nplicit"

XPLICIT_CONFIG = {
    ["_CXXDIALECT"] = 2022,
    ["_CONFIGS"] = { "Debug", "Release" },
    ["_MACROS"] = { "__XPLICIT__", "_CRT_SECURE_NO_WARNINGS", "_WINSOCK_DEPRECATED_NO_WARNINGS", "__XPLICIT_DLL__", "__EXPORT_XPLICIT__" },
    ["_DIALECT"] = "C++",
    ["_FILES"] = {
        ["_INC"] = {
            "XplicitNgin/Source/**.h",
            "XplicitNgin/Source/**.inl"
        },
        ["_SRC"] = {
            "XplicitNgin/Source/**.cpp"
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
	  "C:/VulkanSDK/1.3.250.0/Lib/vulkan-1",
      "Library/glfw3",
      "Library/lua54",
      "Library/Irrlicht",
      "C:/Program Files/OpenSSL-Win64/lib/libssl",
      "C:/Program Files/OpenSSL-Win64/lib/libcrypto",
    },
    ["_INCDIR"] = {
        "C:/Program Files/OpenSSL-Win64/include/",
        "Vendor/",
	    "Vendor/luajit/src",
	    "Vendor/glfw/include/GLFW/",
    	"Vendor/yaml/include",
    	"Nplicit/Source/",
	    "C:/VulkanSDK/1.3.250.0/Include",
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
            "XplicitServer/Source/**.h",
            "XplicitServer/Source/**.inl",
        },
        ["_SRC"] = {
            "XplicitServer/Source/**.cpp"
        },
    },
    ["_LIBDIR"] = {
        "C:/Program Files/Mono/lib/mono-2.0-sgen",
    	"Library/Xplicit",
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
    	"XplicitNgin/Source/",
        "Vendor/nuklear/",
    	"Vendor/yaml/include",
    	"Nplicit/Source/",
	    "Vendor/libzip/lib",
	    "C:/VulkanSDK/1.3.250.0/Include",
        "C:/Program Files/Mono/include/mono-2.0/",
        "C:/Program Files/Mono/include/mono-2.0/mono",
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
    	"Library/Xplicit",
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
        "SoundNgin/",
    	"XplicitNgin/Source/",
    	"Vendor/irr/include",
        "Vendor/CLua/",
        "Vendor/stb/",
	    "Vendor/luajit/src",
    	"Vendor/yaml/include",
        "Vendor/nuklear/",
	    "Vendor/glfw/include/GLFW/",
    	"Nplicit/Source/",
	    "Vendor/libzip/lib",
	    "C:/VulkanSDK/1.3.250.0/Include",
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
            "Nplicit/Source/**.h",
            "Nplicit/Source/**.inl",
        },
        ["_SRC"] = {
            "Nplicit/Source/**.cpp",
            "XplicitPlayer/Source/**.c"
        },
    },
    ["_LIBDIR"] = {
    	"Library/Xplicit",
        "d3d11",
        "dxgi",
    },
    ["_INCDIR"] = {
        "Vendor/",
    	"XplicitNgin/Source/",
        "Vendor/CLua/",
        "Vendor/stb/",
	    "Vendor/libzip/lib",
        "Vendor/yaml/include",
	    "Vendor/glfw/include/GLFW/",
        "Vendor/nuklear/",
    }
};

-- END OF CONFIG

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
    	"Library/Xplicit",
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
        "SoundNgin/",
    	"XplicitNgin/Source/",
    	"Vendor/irr/include",
        "Vendor/CLua/",
        "Vendor/stb/",
	    "Vendor/luajit/src",
    	"Vendor/yaml/include",
        "Vendor/nuklear/",
	    "Vendor/glfw/include/GLFW/",
    	"Nplicit/Source/",
	    "Vendor/libzip/lib",
	    "C:/VulkanSDK/1.3.250.0/Include",
        "C:/Program Files/Mono/include/mono-2.0/",
        "C:/Program Files/Mono/include/mono-2.0/mono",
    }
};