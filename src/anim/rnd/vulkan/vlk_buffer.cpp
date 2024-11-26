/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        vlk_buffer.cpp
 * PROGRAMMER:  IP5
 * LAST UPDATE: 29.10.2024
 * PURPOSE:     Vulkan buffer file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#include "pivk.h"

/* Base project namespace */
namespace pivk
{
  /* Get memory type index function.
   * ARGUMENTS:
   *   - Memory properties:
   *       VkPhysicalDeviceMemoryProperties &MemoryProperties;
   *   - Mask of suitable indeces:
   *       UINT TypeIndexMask;
   *   - Memory flags:
   *       VkMemoryPropertyFlags Flags;
   * RETURNS:
   *   (INT) index.
   */
  static INT GetMemoryTypeIndex( const VkPhysicalDeviceMemoryProperties &MemoryProperties, UINT TypeIndexMask, VkMemoryPropertyFlags Flags )
  {
    for (UINT i = 0; i < MemoryProperties.memoryTypeCount; i++)
    {
      const auto &Type = MemoryProperties.memoryTypes[i];
    
      if ((TypeIndexMask & (1 << i)) &&           // Checking, that index suitable.
          (Type.propertyFlags & Flags) == Flags)  // Checking, that the necessary flags are present.
        return i; // Successful finded.
    }

    return -1;
  } /* End of 'GetMemoryTypeIndex' function */

  /* Create buffer function
   * ARGUMENTS:
   *   - size:
   *       UINT_PTR NewSize;
   *   - type of using:
   *       VkBufferUsageFlags UsageFlags;
   * RETURNS:
   *    (vlk_buf &) reference to this buf.
   */
  vlk_buf & vlk_buf::Create( UINT_PTR NewSize, UINT_PTR NewCount, VkBufferUsageFlags UsageFlags )
  {
    BufferUsageFlags = UsageFlags;
    CountOfElements = NewCount;
    Size = NewSize;

    if (Size > 0)
    {
      VkBufferCreateInfo Info
      {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO, // Structure type.
        .size = Size,                                  // Size of buffer in bytes.
        .usage = UsageFlags,                           // Bit mask of using buffer.
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,      // Sharing type when accessed 
                                                       // by several families of queues
      };

      VmaAllocationCreateInfo AllocationCreateInfo{};
      AllocationCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
      AllocationCreateInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
      AllocationCreateInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
      AllocationCreateInfo.priority = 1.0f;

      if (vmaCreateBuffer(VlkCore->Allocator, &Info, &AllocationCreateInfo, &Buffer, &Allocation, nullptr) != VK_SUCCESS)
      {
        throw;
      }

      ////VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
      //
      //if (vkCreateBuffer(VlkCore->Device,        // Logical device.
      //                   &Info,                  // Information about creating.
      //                   nullptr,                // Memory allocator (optional).
      //                   &Buffer) != VK_SUCCESS) // Pointer to variable 'VkBuffer'.
      //{
      //  throw;
      //}

      //VkMemoryRequirements MemoryRequirements;
      //
      //vkGetBufferMemoryRequirements(VlkCore->Device,      // Logical device.
      //                              Buffer,               // Buffer.
      //                              &MemoryRequirements); // Memory requirememts
      //
      //// Memory parameters (are optained only once).
      //VkPhysicalDeviceMemoryProperties MemoryProperties;
      //vkGetPhysicalDeviceMemoryProperties(VlkCore->PhysDevice, &MemoryProperties);

      // TODO: write parser of memory flag
      // (?, some may be used for compute shaders, for example)
      //INT index = GetMemoryTypeIndex(MemoryProperties,
      //                               MemoryRequirements.memoryTypeBits,
      //                               VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | 
      //                               VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | 
      //                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT); // <-- Temporary
      //
      //if (index == -1)
      //{
      //  throw;
      //}
      //
      //VkMemoryAllocateInfo AllocateInfo
      //{
      //  .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, // Structure type.
      //  .allocationSize = MemoryRequirements.size,       // Size of allocating memory.
      //  .memoryTypeIndex = (UINT32)index                 // Memory type index.
      //};
      //
      //VmaAllocationCreateInfo BufAllocationCreateInfo {};
      //BufAllocationCreateInfo.usage = VMA_MEMORY_USAGE_UNKNOWN;
      //BufAllocationCreateInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
      //BufAllocationCreateInfo.flags = 0;
      //BufAllocationCreateInfo.priority = 1.0f;

      //vkAllocateMemory()
      //if (vkAllocateMemory(VlkCore->Device,              // Logical device.
      //                     &AllocateInfo,                // Information about creating memory.
      //                     nullptr,                      // Memory allocator (optional).
      //                     &DeviceMemory) != VK_SUCCESS) // Pointer to 'VkDeviceMemory' variable.
      //{
      //  throw;
      //}

      //if (vkBindBufferMemory(VlkCore->Device,  // Logical device.
      //                       Buffer,           // Buffer.
      //                       DeviceMemory,     // Suitable memory.
      //                       0) != VK_SUCCESS) // Offset from start memory.
      //{
      //  throw;
      //}
    }

    return *this;
  } /* End of 'vlk_buf::Create' function */

  /* Write data to vulkan buffer function.
   * ARGUMENTS:
   *   - memory data buffer:
   *       const VOID *Data;
   * RETURNS: None.
   */
  VOID vlk_buf::Write( const VOID *Data )
  {
    VOID *MapPointer;

    if (vmaMapMemory(VlkCore->Allocator, Allocation, &MapPointer) != VK_SUCCESS)
    {
      throw;
    }

    //if (vkMapMemory(VlkCore->Device,            // Logical device.
    //                DeviceMemory,               // Memory.
    //                0,                          // Align from start memory.
    //                Size,                       // Size of getting memory.
    //                0,                          // Flags (not used, reserved for future).
    //                &MapPointer) != VK_SUCCESS) // Getting pointer.
    //{
    //  throw;
    //}

    //VkMappedMemoryRange MemoryRange;

    //vkMappedMemoryRand

    // Write buffer to memory.
    std::memcpy(MapPointer, Data, Size);

    // Not used, we use VK_MEMORY_PROPERTY_HOST_COHERENT_BIT flag
    //if (TRUE) // Update data on GPU, if memory don't have corresponding flag. (???)
    //  vkFlushMappedMemoryRanges(VlkCore->Device, );

    // Finally, free pointer to memory.
    // Pointer definety bounded to memory, so his not need transmit to function.
    
    vmaUnmapMemory(VlkCore->Allocator, Allocation);
    //vkUnmapMemory(VlkCore->Device, // Logical device.
    //              DeviceMemory);   // Memory.
  } /* Endof 'vlk_buf::Write' function */

  /* Delete buffer function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID vlk_buf::Free( VOID )
  {
    if (Buffer != nullptr)
    {
      vmaDestroyBuffer(VlkCore->Allocator, Buffer, Allocation);
      Buffer = nullptr;
      //vkFreeMemory(VlkCore->Device, DeviceMemory, nullptr);                                                                   
      //vkDestroyBuffer(VlkCore->Device, Buffer, nullptr);
    }
  } /* End of 'vlk_buf::Free' function */

  /* Resize buffer function.
   * ARGUMENTS:
   *   - new size:
   *       UINT_PTR NewSize;
   * RETURNS: None.
   */
  VOID vlk_buf::Resize( UINT_PTR NewSize, UINT_PTR NewCount )
  {
    if (NewSize <= Size)
      return;

    Free();
    Create(NewSize, NewCount, BufferUsageFlags);
  } /* End of 'Resize' function */

} /* End of 'pivk' namespace */

/* END OF 'vlk_buffer.cpp' FILE */
