/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "DriverVulkan.h"
#include <set>

namespace Xplicit::Renderer::Vk
{
	namespace Details
	{
		VkInstanceCreateInfo create_simple_vulkan()
		{
			VkApplicationInfo appInfo{};

			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

			appInfo.pApplicationName = "XplicitVkClient";
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = "XplicitEngine";
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_0;

			VkInstanceCreateInfo createInfo{};

			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;

			uint32_t glfwExtensionCount = 0;
			const char** glfwExtensions;

			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			createInfo.enabledExtensionCount = glfwExtensionCount;
			createInfo.ppEnabledExtensionNames = glfwExtensions;

			return createInfo;
		}

		bool vulkan_check_dev_ext_support(VkPhysicalDevice device, std::vector<const char*>& deviceExtensions) 
		{
			uint32_t extensionCount;
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

			std::vector<VkExtensionProperties> availableExtensions(extensionCount);
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

			std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

			for (const auto& extension : availableExtensions) {
				requiredExtensions.erase(extension.extensionName);
			}

			return requiredExtensions.empty();
		}

		VkPhysicalDevice vulkan_find_best_device(std::vector<VkPhysicalDevice>& devs, std::vector<const char*>& deviceExtensions)
		{
			for (auto& dev : devs)
			{
				VkPhysicalDeviceProperties deviceProperties;
				VkPhysicalDeviceFeatures deviceFeatures;

				vkGetPhysicalDeviceProperties(dev, &deviceProperties);
				vkGetPhysicalDeviceFeatures(dev, &deviceFeatures);


				bool extensionsSupported = vulkan_check_dev_ext_support(dev, deviceExtensions);

				if (deviceProperties.apiVersion >= VK_API_VERSION_1_0)
					return (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader && extensionsSupported) ? dev : VK_NULL_HANDLE;
			}

			return VK_NULL_HANDLE;
		}

		VulkanFamilyIndices vulkan_find_queue_families(VkPhysicalDevice dev, VkSurfaceKHR surface)
		{
			XPLICIT_ASSERT(dev != VK_NULL_HANDLE);

			VulkanFamilyIndices indices{};

			uint32_t queueFamilyCount = 0U;
			vkGetPhysicalDeviceQueueFamilyProperties(dev, &queueFamilyCount, nullptr);

			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);

			vkGetPhysicalDeviceQueueFamilyProperties(dev, &queueFamilyCount, queueFamilies.data());

			int i = 0;
			for (const auto& queueFamily : queueFamilies) {
				VkBool32 presentSupport = false;

				vkGetPhysicalDeviceSurfaceSupportKHR(dev, i, surface, &presentSupport);

				if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT && presentSupport) {
					indices.graphics_family = i;
				}

				i++;
			}

			return indices;
		}

		VkBool32 vulkan_debug_callback(
			VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData)
		{
			XPLICIT_CRITICAL("Validation Layer [VK]", pCallbackData->pMessage);
			return 0;
		}

		VkResult vulkan_create_debug_utils_messenger(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback) {
			auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
			if (func != nullptr) {
				return func(instance, pCreateInfo, pAllocator, pCallback);
			}
			else {
				return VK_ERROR_EXTENSION_NOT_PRESENT;
			}
		}

		bool vulkan_init_validation_layers(VkInstance instance)
		{
			VkDebugUtilsMessengerCreateInfoEXT createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			createInfo.pfnUserCallback = (PFN_vkDebugUtilsMessengerCallbackEXT)vulkan_debug_callback;
			createInfo.pUserData = nullptr;

			return vulkan_create_debug_utils_messenger(instance, &createInfo, nullptr, nullptr) == VK_SUCCESS;
		}

		VulkanSwapChainSupportDetails vulkan_query_chain_support(VkPhysicalDevice device)
		{
			VulkanSwapChainSupportDetails details;
			return details;
		}

		VkSurfaceFormatKHR vulkan_choose_chain_format(const std::vector<VkSurfaceFormatKHR>& availableFormats) 
		{
			for (const auto& availableFormat : availableFormats) 
			{
				if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				{
					return availableFormat;
				}
			}

			return {};
		}

		VkPresentModeKHR vulkan_choose_chain_present_mode(const std::vector<VkPresentModeKHR>& availablePresentModes)
		{
			for (const auto& availablePresentMode : availablePresentModes)
			{
				if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
				{
					return availablePresentMode;
				}
			}

			return VK_PRESENT_MODE_FIFO_KHR;
		}

		VkExtent2D vulkan_choose_chain_extent(HWND window, const VkSurfaceCapabilitiesKHR& capabilities)
		{
			if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) 
			{
				return capabilities.currentExtent;
			}
			else 
			{

				RECT rect{};

				GetWindowRect(window, &rect);

				int width = rect.right - rect.left;
				int height = rect.bottom - rect.top;

				VkExtent2D actualExtent = {
					static_cast<uint32_t>(width),
					static_cast<uint32_t>(height)
				};

				actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
				actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

				return actualExtent;
			}
		}
	}

	/*
		Man you're going to read lots of code here
		:)
	*/

	DriverSystemVulkan::DriverSystemVulkan(HWND window)
		: m_Info(Details::create_simple_vulkan())
	{
		for (size_t i = 0; i < m_Info.enabledExtensionCount; ++i)
		{
			m_Ext.push_back(m_Info.ppEnabledExtensionNames[i]);
		}

		m_Ext.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
		m_Ext.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		m_Info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

		m_Info.enabledExtensionCount = (uint32_t)m_Ext.size();
		m_Info.ppEnabledExtensionNames = m_Ext.data();
		m_Info.enabledLayerCount = 0U;

		if (vkCreateInstance(&m_Info, nullptr, &m_Instance) != VK_SUCCESS)
			throw EngineError("DriverSystemVulkan: This driver wasn't successfully initialized.");

		uint32_t cnt = 0U;

#ifdef XPLICIT_DEBUG
		XPLICIT_ASSERT(Details::vulkan_init_validation_layers(m_Instance));
#endif

		vkEnumeratePhysicalDevices(m_Instance, &cnt, nullptr);
		
		if (cnt < 1)
			throw EngineError("DriverSystemVulkan: No Physical devices were given to the engine.");

		m_PhysVec = std::vector<VkPhysicalDevice>(cnt);
		
		vkEnumeratePhysicalDevices(m_Instance, &cnt, m_PhysVec.data());
		
		m_PhysCurrent = Details::vulkan_find_best_device(m_PhysVec, m_Ext);

		if (m_PhysCurrent == VK_NULL_HANDLE)
			throw EngineError("DriverSystemVulkan: No Such Device.");

		m_GraphicsFamily = Details::vulkan_find_queue_families(m_PhysCurrent, m_Surface);

		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = m_GraphicsFamily.graphics_family;
		queueCreateInfo.queueCount = 1;

		float queuePriority = 1.0f;

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { m_GraphicsFamily.graphics_family, m_GraphicsFamily.graphics_family };

		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		queueCreateInfo.pQueuePriorities = &queuePriority;

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(m_Ext.size());
		createInfo.ppEnabledExtensionNames = m_Ext.data();

		if (vkCreateDevice(m_PhysCurrent, &createInfo, nullptr, &m_Device) != VK_SUCCESS) {
			throw EngineError("VulkanDriver: failed to create logical device! This is due to a invalid feature set for this Card.");
		}

		vkGetDeviceQueue(m_Device, m_GraphicsFamily.graphics_family, 0, &m_Queue);

		m_WindowCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		m_WindowCreateInfo.hwnd = window;
		m_WindowCreateInfo.hinstance = GetModuleHandle(nullptr);

		if (vkCreateWin32SurfaceKHR(m_Instance, &m_WindowCreateInfo, nullptr, &m_Surface) != VK_SUCCESS)
			throw EngineError("failed to create window surface!");

		bool extensionsSupported = m_Ext.size() != 0;
		bool swapChainAdequate = false;

		if (extensionsSupported) {
			auto details = Details::vulkan_query_chain_support(m_PhysCurrent);
			swapChainAdequate = !details.formats.empty() && !details.presentModes.empty();

			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_PhysCurrent, m_Surface, &details.capabilities);

			uint32_t formatCount = 0UL;
			vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysCurrent, m_Surface, &formatCount, nullptr);

			if (formatCount != 0)
			{
				details.formats.resize(formatCount);
				vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysCurrent, m_Surface, &formatCount, details.formats.data());
			}

			uint32_t presentModeCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(m_PhysCurrent, m_Surface, &presentModeCount, nullptr);

			if (presentModeCount != 0) {
				details.presentModes.resize(presentModeCount);
				vkGetPhysicalDeviceSurfacePresentModesKHR(m_PhysCurrent, m_Surface, &presentModeCount, details.presentModes.data());
			}
		}

		this->vkInitSpawnChainCls(window);
	}

	DriverSystemVulkan::~DriverSystemVulkan()
	{
		if (m_Instance)
			vkDestroyInstance(m_Instance, nullptr);

		if (m_Device)
			vkDestroyDevice(m_Device, nullptr);

		if (m_Surface)
			vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);

		if (m_SwapChain)
			vkDestroySwapchainKHR(m_Device, m_SwapChain, nullptr);
	}

	void DriverSystemVulkan::vkInitSpawnChainCls(HWND window)
	{
		m_SwapChainSupport = Details::vulkan_query_chain_support(m_PhysCurrent);

		VkSurfaceFormatKHR surfaceFormat = Details::vulkan_choose_chain_format(m_SwapChainSupport.formats);
		VkPresentModeKHR presentMode = Details::vulkan_choose_chain_present_mode(m_SwapChainSupport.presentModes);
		VkExtent2D extent = Details::vulkan_choose_chain_extent(window, m_SwapChainSupport.capabilities);

		VkSwapchainCreateInfoKHR createInfo{};

		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_Surface;

		uint32_t imageCount = m_SwapChainSupport.capabilities.minImageCount + 1;

		if (m_SwapChainSupport.capabilities.maxImageCount > 0 && imageCount > m_SwapChainSupport.capabilities.maxImageCount) {
			imageCount = m_SwapChainSupport.capabilities.maxImageCount;
		}

		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		Details::VulkanFamilyIndices indices = Details::vulkan_find_queue_families(m_PhysCurrent, m_Surface);
		uint32_t queueFamilyIndices[] = { indices.graphics_family, indices.graphics_family };

		if (indices.graphics_family != indices.graphics_family) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0; // Optional
			createInfo.pQueueFamilyIndices = nullptr; // Optional
		}

		createInfo.preTransform = m_SwapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;

		createInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(m_Device, &createInfo, nullptr, &m_SwapChain) != VK_SUCCESS)
			throw EngineError("vkCreateSwapChainKHR: Something bad happened! Swapchain couldn't be created!");
	
	
		vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &imageCount, nullptr);
		m_VkImages.resize(imageCount);
		vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &imageCount, m_VkImages.data());
	}

	const char* DriverSystemVulkan::name() noexcept { return ("DriverSystemVulkan"); }
	RENDER_SYSTEM DriverSystemVulkan::api() noexcept { return RENDER_SYSTEM::VULKAN; }
}