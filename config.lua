architecture ("x86_64")

XPLICIT_PROJ = "Xplicit"

XPLICIT_SERVER_PROJ = "XPXServer"
XPLICIT_CLIENT_PROJ = "XPXPlayer"

XPLICIT_WORKSPACE = "XplicitNgine"
NPLICIT_PROJ = "XPXNgin.Physics"

TAGRET="ARM64"

XPLICIT_CONFIG = {
    ["_CXXDIALECT"] = 2022,
    ["_CONFIGS"] = { "Debug", "Release" },
    ["_MACROS"] = { "__XPX__", "_CRT_SECURE_NO_WARNINGS", "_WINSOCK_DEPRECATED_NO_WARNINGS", "__XPLICIT_DLL__", "__EXPORT_XPLICIT__" },
    ["_DIALECT"] = "Objective-C++",
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
      "/opt/homebrew/Cellar/mono/6.12.0.182/lib/monosgen-2.0",
	  "/Users/amlal/VulkanSDK/1.3.250.1/macOS/vulkan.1",
      "Library/glfw3",
      "/opt/homebrew/Cellar/lua/5.4.6/lib/lua",
      "/opt/homebrew/Cellar/irrlicht/1.8.5_1/lib/Irrlicht",
      "/opt/homebrew/opt/openssl@1.1/lib/ssl",
      "/opt/homebrew/opt/openssl@1.1/lib/crypto",
    },
    ["_INCDIR"] = {
        "/opt/homebrew/opt/openssl@1.1/include",
        "Vendor/",
        "XPXLua/",
	    "Vendor/glfw/include/GLFW/",
    	"Vendor/yaml/include",
    	"XPXNgin.Physics/Source/",
	    "/Users/amlal/VulkanSDK/1.3.250.1/Include",
	    "Vendor/libzip/lib",
        "/opt/homebrew/Cellar/mono/6.12.0.182/include/mono-2.0/",
        "/opt/homebrew/Cellar/mono/6.12.0.182/include/mono-2.0/mono",
    }
};

XPLICIT_SERVER_CONFIG = {
    ["_CXXDIALECT"] = 2022,
    ["_CONFIGS"] = { "Debug", "Release" },
    ["_MACROS"] = { "__XPX__", "__SERVER__", "_CRT_SECURE_NO_WARNINGS", "_WINSOCK_DEPRECATED_NO_WARNINGS" },
    ["_DIALECT"] = "Objective-C++",
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
        "/opt/homebrew/Cellar/mono/6.12.0.182/lib/monosgen-2.0",
    	"Library/Xplicit",
        "/opt/homebrew/Cellar/irrlicht/1.8.5_1/lib/Irrlicht",
        "/opt/homebrew/Cellar/lua/5.4.6/lib/lua",
      	"/opt/homebrew/opt/openssl@1.1/lib/ssl",
      	"/opt/homebrew/opt/openssl@1.1/lib/crypto",
    },
    ["_INCDIR"] = {
        "/opt/homebrew/opt/openssl@1.1/include",
        "Vendor/",
        "XPXLua/",
        "Vendor/stb/",
	    "Vendor/glfw/include/GLFW/",
    	"XPXNgin/Source/",
        "Vendor/nuklear/",
    	"Vendor/yaml/include",
    	"XPXNgin.Physics/Source/",
	    "Vendor/libzip/lib",
	    "/Users/amlal/VulkanSDK/1.3.250.1/Include",
        "/opt/homebrew/Cellar/mono/6.12.0.182/include/mono-2.0/",
        "/opt/homebrew/Cellar/mono/6.12.0.182/include/mono-2.0/mono",
    }
};

XPLICIT_CLIENT_CONFIG = {
    ["_CXXDIALECT"] = 2022,
    ["_CONFIGS"] = { "Debug", "Release" },
    ["_MACROS"] = { "__XPX__", "_CRT_SECURE_NO_WARNINGS", "_WINSOCK_DEPRECATED_NO_WARNINGS", "__XPLICIT_CLIENT__" },
    ["_DIALECT"] = "Objective-C++",
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
    	"Library/Xplicit",
        "/opt/homebrew/Cellar/lua/5.4.6/lib/lua",
        "/opt/homebrew/Cellar/irrlicht/1.8.5_1/lib/Irrlicht",
      "/opt/homebrew/opt/openssl@1.1/lib/ssl",
      "/opt/homebrew/opt/openssl@1.1/lib/crypto",
    },
    ["_INCDIR"] = {
        "/opt/homebrew/opt/openssl@1.1/include",
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
	    "/Users/amlal/VulkanSDK/1.3.250.1/Include",
        "/opt/homebrew/Cellar/mono/6.12.0.182/include/mono-2.0/",
        "/opt/homebrew/Cellar/mono/6.12.0.182/include/mono-2.0/mono",
    }
};

NPLICIT_CONFIG = {
    ["_CXXDIALECT"] = 2022,
    ["_CONFIGS"] = { "Debug", "Release" },
    ["_MACROS"] = { "__NPLICIT__", "_CRT_SECURE_NO_WARNINGS", "_WINSOCK_DEPRECATED_NO_WARNINGS", "__NPLICIT_DLL__", "__EXPORT_NPLICIT__" },
    ["_DIALECT"] = "Objective-C++",
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
    	"Library/Xplicit",
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