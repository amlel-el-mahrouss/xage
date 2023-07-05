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
	  "d3dcompiler",
	  "ws2_32",
	  "winmm",
	  "opengl32",
	  "C:/VulkanSDK/1.3.250.0/Lib/vulkan-1",
      "Library/glfw3",
	  "Library/lua",
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
	    "$(XPLICIT_OGRE_D3D11_INCLUDE)",
	    "$(XPLICIT_OGRE_MAIN_INCLUDE)",
	    "$(XPLICIT_OGRE_VK_INCLUDE)",
	    "$(XPLICIT_OGRE_RTSS_INCLUDE)",
	    "$(XPLICIT_OGRE_BITES_INCLUDE)",
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
	    "Library/lua",
	    "ws2_32",
	    "winmm",
        "C:/Program Files/OpenSSL-Win64/lib/libssl",
        "C:/Program Files/OpenSSL-Win64/lib/libcrypto",
    },
    ["_INCDIR"] = {
        "C:/Program Files/OpenSSL-Win64/include/",
        "Vendor/",
        "Vendor/stb/",
	    "Vendor/luajit/src",
	    "Vendor/glfw/include/GLFW/",
    	"Xplicit/Source/",
        "Vendor/nuklear/",
    	"Vendor/yaml/include",
    	"Nplicit/Source/",
	    "$(XPLICIT_OGRE_D3D11_INCLUDE)",
	    "$(XPLICIT_OGRE_MAIN_INCLUDE)",
	    "$(XPLICIT_OGRE_VK_INCLUDE)",
	    "$(XPLICIT_OGRE_RTSS_INCLUDE)",
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
	    "Library/lua",
        "dxgi",
        "Library/glfw3",
        "d2d1",
        "opengl32",
	    "ws2_32",
	    "winmm",
        "Library/RenderSystem_Direct3D11",
        "Library/Plugin_ParticleFX",
        "Library/Plugin_OctreeSceneManager",
        "C:/Program Files/OpenSSL-Win64/lib/libssl",
        "C:/Program Files/OpenSSL-Win64/lib/libcrypto",
    },
    ["_INCDIR"] = {
        "C:/Program Files/OpenSSL-Win64/include/",
        "Vendor/",
        "SoundNgin/",
    	"Xplicit/Source/",
    	"Xplicit/Vendor/include/",
    	"Vendor/irr/include",
        "Vendor/lua/",
        "Vendor/stb/",
	    "Vendor/luajit/src",
    	"Vendor/yaml/include",
	    "$(XPLICIT_OGRE_D3D11_INCLUDE)",
	    "$(XPLICIT_OGRE_MAIN_INCLUDE)",
	    "$(XPLICIT_OGRE_VK_INCLUDE)",
	    "$(XPLICIT_OGRE_RTSS_INCLUDE)",
	    "$(XPLICIT_OGRE_BITES_INCLUDE)",
        "$(XPLICIT_OGRE_PFX_INCLUDE)",
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
    	"Xplicit/Source/",
    	"Xplicit/Vendor/include/",
        "Vendor/lua/",
        "Vendor/stb/",
	    "Vendor/libzip/lib",
        "Vendor/yaml/include",
	    "Vendor/glfw/include/GLFW/",
        "Vendor/nuklear/",
	    "$(XPLICIT_OGRE_MAIN_INCLUDE)",
    }
};

-- END OF CONFIG

print('Please define Ogre3D exports! e.g: C:/Users/amlal/Desktop/include')
print('Please define Ogre3D exports! e.g: C:/Users/amlal/Desktop/include')
print('Please define Ogre3D exports! e.g: C:/Users/amlal/Desktop/include')