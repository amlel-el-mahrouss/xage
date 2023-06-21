XPLICIT_PROJ = "Xplicit"

XPLICIT_SERVER_PROJ = "XplicitServer"
XPLICIT_CLIENT_PROJ = "XplicitPlayer"

XPLICIT_SCRIPT_PROJ = "XplicitScript"
XPLICIT_WORKSPACE = "XplicitNgine"
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
          "C:/Program Files/Mono/lib/mono-2.0-sgen",
	  "dxgi",
	  "d3d11",
	  "d2d1",
	  "Library/enet_static64",
	  "Library/enet64",
	  "d3dcompiler",
	  "ws2_32",
	  "winmm",
	  "opengl32",
	  "C:/VulkanSDK/1.3.250.0/Lib/vulkan-1",
          "Library/glfw3",
	  "Library/irrlicht",
    },
    ["_INCDIR"] = {
        "Vendor/",
	"Vendor/luajit/src",
	"Vendor/glfw/include/GLFW/",
    	"Vendor/yaml/include",
    	"Nplicit/Source/",
	"C:/VulkanSDK/1.3.250.0/Include",
	"Vendor/irr/include",
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
	"Library/enet_static64",
	"Library/enet64",
        "C:/Program Files/Mono/lib/mono-2.0-sgen",
    	"bin/Debug/Xplicit",
        "Library/Irrlicht",
	  "ws2_32",
	  "winmm",
    },
    ["_INCDIR"] = {
        "Vendor/",
        "Vendor/stb/",
	"Vendor/luajit/src",
	"Vendor/glfw/include/GLFW/",
    	"Xplicit/Source/",
        "Vendor/nuklear/",
    	"Vendor/yaml/include",
    	"Nplicit/Source/",
	"Vendor/irr/include",
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
	"Library/enet_static64",
	"Library/enet64",
    	"bin/Debug/Xplicit",
        "d3d11",
        "dxgi",
        "Library/glfw3",
        "d2d1",
        "opengl32",
	  "ws2_32",
	  "winmm",
      "Library/Irrlicht"
    },
    ["_INCDIR"] = {
        "Vendor/",
    	"Xplicit/Source/",
    	"Xplicit/Vendor/include/",
    	"Vendor/irr/include",
        "Vendor/lua/",
        "Vendor/stb/",
	"Vendor/luajit/src",
    	"Vendor/yaml/include",
        "Vendor/nuklear/",
	"Vendor/glfw/include/GLFW/",
    	"Nplicit/Source/",
	"Vendor/libzip/lib",
	"C:/VulkanSDK/1.3.250.0/Include",
	"Vendor/irr/include",
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
	"C:/VulkanSDK/1.3.250.0/Lib/vulkan-1",
    	"Library/Xplicit",
        "d3d11",
        "dxgi",
    },
    ["_INCDIR"] = {
        "Vendor/",
    	"Xplicit/Source/",
    	"Xplicit/Vendor/include/",
    	"Vendor/irr/includeinclude",
        "Vendor/lua/",
        "Vendor/stb/",
	    "Vendor/libzip/lib",
        "Vendor/yaml/include",
	    "Vendor/glfw/include/GLFW/",
        "Vendor/nuklear/",
	    "Vendor/irr/include",
        "C:/Program Files/Mono/include/mono-2.0/",
        "C:/Program Files/Mono/include/mono-2.0/mono",
    }
};

-- END OF CONFIG