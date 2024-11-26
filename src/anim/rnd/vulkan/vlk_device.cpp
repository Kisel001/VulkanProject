/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        vlk_device.cpp
 * PROGRAMMER:  IP5
 * LAST UPDATE: 28.10.2024
 * PURPOSE:     Vulkan core device file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#include "pivk.h"

/* Base project namespace */
namespace pivk
{
  /* Create device of vulkan function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID vlk::CreateDevice( VOID )
  {
    UINT32 QueueFamilyCount;

    vkGetPhysicalDeviceQueueFamilyProperties(PhysDevice, &QueueFamilyCount, nullptr);

    //vkGetPhysicalDeviceProperties()

    //float a = 1.0;
    std::vector<FLOAT> QueuePriorities;
    QueuePriorities.resize(QueueFamilyCount);

    for (auto& i : QueuePriorities)
      i = 1.f;

    VkDeviceQueueCreateInfo QueueInfo
    {
      .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .queueFamilyIndex = 0,
      //.queueCount = 1,
      .queueCount = QueueFamilyCount,
      .pQueuePriorities = QueuePriorities.data(),
    };

    // Set extenstions of logical device.
    std::vector<const CHAR *> ExtensionNames
    {
      //VK_KHR_SURFACE_EXTENSION_NAME,
      VK_KHR_SWAPCHAIN_EXTENSION_NAME,
      VK_EXT_EXTENDED_DYNAMIC_STATE_3_EXTENSION_NAME,
    };

    VkPhysicalDeviceExtendedDynamicState3FeaturesEXT PhysicalDeviceExtendedDynamicState3Features
    {
      .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_3_FEATURES_EXT,
      .extendedDynamicState3PolygonMode = VK_TRUE,
    };

    VkPhysicalDeviceFeatures DeviceFeatures;
    vkGetPhysicalDeviceFeatures(PhysDevice, &DeviceFeatures);
    DeviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo DeviceInfo
    {
      .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
      .pNext = &PhysicalDeviceExtendedDynamicState3Features,
      .flags = 0,
      .queueCreateInfoCount = 1,
      .pQueueCreateInfos = &QueueInfo,
      .enabledLayerCount = 0,
      .ppEnabledLayerNames = nullptr,
      .enabledExtensionCount = UINT32(ExtensionNames.size()),
      .ppEnabledExtensionNames = ExtensionNames.data(),
      .pEnabledFeatures = &DeviceFeatures
    };

    VkResult Res;

    if ((Res = vkCreateDevice(PhysDevice,  // Physical device.
                              &DeviceInfo, // Information of creating.
                              nullptr,     // Memory allocator (optional).
                              &Device)) != VK_SUCCESS)
    {
      MessageBox(hWndRef, "Error of creating logical device", "T53VLK :: ERROR", MB_OK | MB_ICONERROR);
      exit(3);
    }

    // Create AMD VMA allocator
    AllocatorCreateInfo = VmaAllocatorCreateInfo {};
    AllocatorCreateInfo.vulkanApiVersion = DeviceProperties.apiVersion;
    AllocatorCreateInfo.instance = Instance;
    AllocatorCreateInfo.physicalDevice = PhysDevice;
    AllocatorCreateInfo.device = Device;

    if ((vmaCreateAllocator(&AllocatorCreateInfo, &Allocator)) != VK_SUCCESS)
    {
      MessageBox(hWndRef, "Error of creating AMD Vulkan Memory Allocator", "T53VLK :: ERROR", MB_OK | MB_ICONERROR);
      exit(3);
    }
  } /* End of 'vlk::CreateDevice' function */

} /* end of 'pivk' namespace */

/* END OF 'vlk_device.cpp' FILE */
