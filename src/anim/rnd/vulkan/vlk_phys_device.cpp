/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        vlk_phys_device.cpp
 * PROGRAMMER:  IP5
 * LAST UPDATE: 28.10.2024
 * PURPOSE:     Vulkan core physical device file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#include "pivk.h"

/* Base project namespace */
namespace pivk
{
  /* Create physical device of vulkan function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID vlk::CreatePhysDevice( VOID )
  {
    // Receiving count of phys devices of system.
    std::vector<VkPhysicalDevice> PhysDevices;
    UINT32 PhysicalDeviceCount = 0;
    if (vkEnumeratePhysicalDevices(Instance, &PhysicalDeviceCount, nullptr) != VK_SUCCESS)
    {
      MessageBox(this->hWndRef, "Error of creating vulkan physical device", "T53VLK :: ERROR", MB_OK | MB_ICONERROR);
      exit(3); // Processing error.
    }

    // Receiving physical devices of system.
    PhysDevices.resize(PhysicalDeviceCount);
    if (vkEnumeratePhysicalDevices(Instance, &PhysicalDeviceCount, PhysDevices.data()) != VK_SUCCESS)
    {
      MessageBox(this->hWndRef, "Error of creating vulkan physical device", "T53VLK :: ERROR", MB_OK | MB_ICONERROR);
      exit(3); // Processing error.
    }

    // Choice phys device by requirements.
    VkPhysicalDevice TmpPhysDevice = VK_NULL_HANDLE;
    //UINT64 BestMemory = 0;
    UINT32 BestCountOfQueue = 0;
    std::string PhysDeviceName;

    MessageLog("DEVICES:\n\n");

    for (VkPhysicalDevice &Device : PhysDevices)
    {
      std::vector<VkQueueFamilyProperties> QueueFamilies;
      UINT QueueFamilyCount;
      std::vector<VkExtensionProperties> AvailableExtensions;
      UINT ExtensionCount;

      // Physical device worth chooising based on his:
      VkPhysicalDeviceProperties CurDeviceProperties;              // Main settings
      vkGetPhysicalDeviceProperties(Device, &CurDeviceProperties); // (For example, type of device,
                                                                   // the limits of the device's capabilities),

      VkPhysicalDeviceFeatures DeviceFeatures;                    // Additional features
      vkGetPhysicalDeviceFeatures(Device, &DeviceFeatures);       // (For example, supporting of geometry è
                                                                  // tesselation shaders),

      vkGetPhysicalDeviceQueueFamilyProperties(Device, &QueueFamilyCount, nullptr);  // Support command types
                                                                                     // queue (for example, some
                                                                                     // from their intended for
                                                                                     // processing evaluation comands).
      QueueFamilies.resize(QueueFamilyCount);
      vkGetPhysicalDeviceQueueFamilyProperties(Device, &QueueFamilyCount, QueueFamilies.data());

      // Checking support extensions
      vkEnumerateDeviceExtensionProperties(Device, nullptr, &ExtensionCount, nullptr);
      AvailableExtensions.resize(ExtensionCount);
      vkEnumerateDeviceExtensionProperties(Device, nullptr, &ExtensionCount, AvailableExtensions.data());
      
      BOOL 
        IsCompatible1 = FALSE,
        IsCompatible2 = FALSE;

      MessageLog(CurDeviceProperties.deviceName);
      MessageLog("\n");

      for (auto &Extension : AvailableExtensions)
      {
        std::string S = "    " + std::string(Extension.extensionName) + '\n';
        MessageLog(S);

        if (strcmp(Extension.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
        {
          IsCompatible1 = TRUE;
          // break;
        }
      }

      // Checking queue to compatibility
      for (auto &QueueFamily : QueueFamilies)
      {
        if ((QueueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
            (QueueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) &&
            (QueueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT))
        {
          IsCompatible2 = TRUE;
          break;
        }
      }

      // Choose physical device by count of queues
      if (IsCompatible1 && IsCompatible2)
      {
        if (BestCountOfQueue < QueueFamilyCount)
        {
          TmpPhysDevice = Device;
          PhysDeviceName = CurDeviceProperties.deviceName;
          BestCountOfQueue = QueueFamilyCount;

          // Copy device properties to vulkan core
          std::memcpy(&DeviceProperties, &CurDeviceProperties, sizeof(VkPhysicalDeviceProperties));
        }
      }

#if 0 // Choose physical device by memory, not used
      if (IsCompatible)
      {
        VkPhysicalDeviceMemoryProperties PhysicalDeviceMemoryProperties;
        vkGetPhysicalDeviceMemoryProperties(Device, &PhysicalDeviceMemoryProperties);
        UINT64 Memory = 0;

        for (INT i = 0; i < PhysicalDeviceMemoryProperties.memoryHeapCount; ++i)
          Memory += PhysicalDeviceMemoryProperties.memoryHeaps[i].size;

        if (Memory > BestMemory)
        {
          TmpPhysDevice = Device;
          BestMemory = Memory;
        }
      }
#endif // 0
    }

    // Set physical device
    PhysDevice = TmpPhysDevice;

    if (PhysDevice == VK_NULL_HANDLE)
    {
      MessageBox(hWndRef, "Error of choose phys device", "T53VLK :: ERROR", MB_OK | MB_ICONERROR);
      exit(3); // Error processing.
    }

    MessageLog(std::string("Device: ") + PhysDeviceName);
  } /* End of 'vlk::CreatePhysDevice' function */

} /* end of 'pivk' namespace */

/* END OF 'vlk_physical_device.cpp' FILE */
