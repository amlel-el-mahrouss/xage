/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright PlayXPlicit, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "Bites.h"
#include "DriverSystem.h"

#ifdef XPLICIT_WINDOWS

#	define VK_USE_PLATFORM_WIN32_KHR
#	define GLFW_INCLUDE_VULKAN

#	include <glfw3.h>

#	define GLFW_EXPOSE_NATIVE_WIN32

#	include <glfw3native.h>

#endif

#include <vulkan/vulkan.hpp>

namespace Xplicit::Renderer::Vk
{
	namespace Details
	{
		struct VulkanFamilyIndices
		{
			uint32_t graphics_family;
		};

		struct VulkanSwapChainSupportDetails 
		{
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};

		VkInstanceCreateInfo create_simple_vulkan();
	}

	class XPLICIT_API DriverSystemVulkan final : public DriverSystem
	{
	public:
		DriverSystemVulkan(HWND window);
		~DriverSystemVulkan() override;

	public:
		XPLICIT_DRIVER_SYSTEM_OVERRIDE();

	private:
		void vkInitSpawnChainCls(HWND window);

	public:
		XPLICIT_COPY_DELETE(DriverSystemVulkan);

	private:
		std::vector<VkPhysicalDevice> m_PhysVec;
		std::vector<VkImage> m_VkImages;
		std::vector<const char*> m_Ext;

	private:
		Details::VulkanSwapChainSupportDetails m_SwapChainSupport;
		Details::VulkanFamilyIndices m_GraphicsFamily;

	private:
		VkWin32SurfaceCreateInfoKHR m_WindowCreateInfo;
		VkPhysicalDevice m_PhysCurrent;
		VkSwapchainKHR m_SwapChain;
		VkInstanceCreateInfo m_Info;
		VkInstance m_Instance;
		VkSurfaceKHR m_Surface;
		VkDevice m_Device;
		VkQueue m_Queue;

	};
}