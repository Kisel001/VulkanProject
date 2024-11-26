/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        vlk_comandpool.cpp
 * PROGRAMMER:  IP5
 * LAST UPDATE: 29.10.2024
 * PURPOSE:     Vulkan comand pool file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#include "pivk.h"

/* Base project namespace */
namespace pivk
{
  /* Create command queue function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID vlk::CreateCommandQueue( VOID )
  {
    // Get handles for every queue families.
    vkGetDeviceQueue(Device,         // Logic device.
                     0,              // Index of queue family.
                     0,              // Queue index.
                     &CommandQueue); // Pointer to variable VkQueue.
  } /* End of 'CreateCommandQueue' function */

  /* Get queue family indices function.
   * ARGUMENTS: None.
   * RETURNS:
   *   (vlk::queue_family_indices) suitable queue family indices.
   */
  vlk::queue_family_indices vlk::GetQueueFamilyIndices( VOID )
  {
    UINT32 QueueFamilyCount = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(this->PhysDevice, &QueueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> QueueFamilyProperties {QueueFamilyCount};
    vkGetPhysicalDeviceQueueFamilyProperties(this->PhysDevice, &QueueFamilyCount, QueueFamilyProperties.data());

    queue_family_indices Indices;

    for (UINT32 i = 0; i < QueueFamilyCount; i++)
    {
      // Checking of support rendering by queue family
      if ((QueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
        Indices.GraphicsFamily = i;
      
      // Checking of support screen output by queue family
      VkBool32 PresentSupport = VK_FALSE;
      vkGetPhysicalDeviceSurfaceSupportKHR(this->PhysDevice, i, Surface, &PresentSupport);
      if (PresentSupport == VK_TRUE)
        Indices.PresentFamily = i;
    }

    return Indices;
  } /* End of 'vlk::GetQueueFamilyIndices' function */

  /* Create command pool function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID vlk::CreateCommandPool( VOID )
  {
    queue_family_indices Indices = GetQueueFamilyIndices();
    VkCommandPoolCreateInfo Info
    {
      .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,      // Type of structure.
      .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, // Flags of comand pool.
      .queueFamilyIndex = Indices.GraphicsFamily.value()        // Index of using queue family.
    };
    
    vkCreateCommandPool(Device,        // Logical device.
                        &Info,         // Create information.
                        nullptr,       // Memory allocator (optional).
                        &CommandPool); // Pointer to variable VkCommandPool.
  } /* End of 'vlk::CreateCommandPool' function */

  /* Create command buffer function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID vlk::CreateCommandBuffer( VOID )
  {
    VkCommandBufferAllocateInfo Info
    {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, // Structure type.
      .commandPool = CommandPool,                              // Using command pool.
      .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,                // Command buffer size.
      .commandBufferCount = 1                                  // Count of command buffers.
    };
    
    if (vkAllocateCommandBuffers(Device,                        // Logic device.
                                 &Info,                         // Creating information.
                                 &CommandBuffer) != VK_SUCCESS) // Pointer to array with variables VkCommandBuffer
                                                                // (now we have one buffer => one variable).
    {
      exit(3);
    }

    VkFenceCreateInfo FenceCreateInfo
    {
      .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
      .flags = 0,
    };

    if (vkCreateFence(Device, &FenceCreateInfo, nullptr, &Fence))
    {
      exit(3);
    }
  } /* End of 'vlk::CreateCommandBuffer' function */

} /* end of 'pivk' namespace */

/* END OF 'vlk_comandpool.cpp' FILE */
