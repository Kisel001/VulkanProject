/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        vlk_image.cpp
 * PROGRAMMER:  IP5
 * LAST UPDATE: 12.11.2024
 * PURPOSE:     Vulkan image realization file.
 * NOTE:        None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#include "pivk.h"

/* Vulkan image creation function.
 * ARGUMENTS:
 *   - vulkan image handle output reference:
 *       VkImage &Image;
 *   - vulkan image view handle output reference:
 *       VkImageView &ImageView;
 *   - VMA AMD image memory handle output reference:
 *       VmaAllocation &Memory;
 *   - image size:
 *       INT W, H;
 *   - image color flag (otherwise Image is depth buffer):
 *       BOOL IsColor,
 *   - cube map flag (create 6 layers and 'PixelData' contain 6 joined same size images):
 *       BOOL IsCube,
 *   - vulkan image Format (see VK_FORMAT_***):
 *       VkFormat Format;
 *   - vulkan image usage flags bitwise combination (VK_IMAGE_USAGE_***):
 *       VkImageUsageFlags UsageFlags;
 *   - vulkan image layout (see VK_IMAGE_LAYOUT_***):
 *       VkImageLayout Layout;
 *   - image pixel data:
 *       const VOID *PixelsData;
 *   - image pixel data size (in bytes):
 *       UINT PixelsDataSize;
 *   - number of mipmaps to be create:
 *       UINT MipCount;
 * RETURNS:
 *   (BOOL) success creation result.
 */
BOOL pivk::vlk::ImageCreate( VkImage &Image, VkImageView &ImageView, VmaAllocation &Allocation,
                             INT W, INT H, BOOL IsColor, BOOL IsCube,
                             VkFormat Format, VkImageUsageFlags UsageFlags, VkImageLayout Layout,
                             const VOID *PixelsData, UINT PixelsDataSize, UINT MipCount )
{
  if (MipCount < 1)
    MipCount = 1;

  // Create vulkan image
  VkImageCreateInfo ImageCreateInfo
  {
    .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
    .imageType = VK_IMAGE_TYPE_2D,
    .format = Format,
    .extent {.width = (UINT)W, .height = (UINT)H, .depth = 1 },
    .mipLevels = MipCount,
    .arrayLayers = IsCube ? 6U : 1U,
    .samples = VK_SAMPLE_COUNT_1_BIT,
    .tiling = VK_IMAGE_TILING_OPTIMAL,
    .usage = UsageFlags,
    .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    .initialLayout = Layout,
  };
  //vkCreateImage(Device, &ImageCreateInfo, nullptr, &Image);

  // Create image through VMA
  VmaAllocationCreateInfo AllocationCreateInfo {};
  AllocationCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
  AllocationCreateInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
  AllocationCreateInfo.flags = 0;
  AllocationCreateInfo.priority = 1.0f;
  if (vmaCreateImage(Allocator, &ImageCreateInfo, &AllocationCreateInfo,
      &Image, &Allocation, nullptr) != VK_SUCCESS)
    throw;

  // Create image view
  VkImageViewCreateInfo ImageViewCreateInfo
  {
    .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    .image = Image,
    .viewType = VK_IMAGE_VIEW_TYPE_2D,
    .format = Format,
    .components = 
    {
      .r = VK_COMPONENT_SWIZZLE_IDENTITY,
      .g = VK_COMPONENT_SWIZZLE_IDENTITY,
      .b = VK_COMPONENT_SWIZZLE_IDENTITY,
      .a = VK_COMPONENT_SWIZZLE_IDENTITY
    },
    .subresourceRange
    {
      .aspectMask = VkImageAspectFlags(IsColor ? VK_IMAGE_ASPECT_COLOR_BIT : VK_IMAGE_ASPECT_DEPTH_BIT), // ??? shadow map
      .baseMipLevel = 0,
      .levelCount = MipCount,
      .baseArrayLayer = 0,
      .layerCount = IsCube ? 6U : 1U,
    },
  };

  vkCreateImageView(Device, &ImageViewCreateInfo, nullptr, &ImageView);

  if (PixelsData != nullptr)
  {
    // Create transfer stage image buffer
    // Allocate vulkan buffer memory and bind to buffer
    VkBufferCreateInfo BufferCreateInfo
    {
      .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
      .size = VkDeviceSize(PixelsDataSize),
      .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };
    VmaAllocationCreateInfo AllocationCreateInfo1 {};

    AllocationCreateInfo1.usage = VMA_MEMORY_USAGE_AUTO;
    AllocationCreateInfo1.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    AllocationCreateInfo1.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
    AllocationCreateInfo1.priority = 1.0f;

    VkBuffer Buffer;
    VmaAllocation Allocation;

    vmaCreateBuffer(Allocator, &BufferCreateInfo, &AllocationCreateInfo1, &Buffer, &Allocation, nullptr);
    
    // Map buffer memory and copy Image to image
    VOID *MapPointer;
    
    vmaMapMemory(Allocator, Allocation, &MapPointer);
    std::memcpy(MapPointer, PixelsData, PixelsDataSize);
    vmaUnmapMemory(Allocator, Allocation);

    // Allocate vulkan buffer memory and bind to buffer
    // VkMemoryRequirements MemoryRequirements;
    // vkGetBufferMemoryRequirements(Device, Buffer, &MemoryRequirements);
    // VkMemoryPropertyFlags Flags = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    // UINT memory_type_index = -1;
    // for (UINT i = 0; i < PhysDeviceMemoryProperties.memoryTypeCount; i++)
    // {
    //   const auto &Type = PhysDeviceMemoryProperties.memoryTypes[i];
    //   if ((MemoryRequirements.memoryTypeBits & (1 << i)) && (Type.propertyFlags & Flags) == Flags)
    //   {
    //     memory_type_index = i;
    //     break;
    //   }
    // }
    // VkMemoryAllocateInfo MemoryAllocateInfo
    // {
    //   .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    //   .allocationSize = MemoryRequirements.size,
    //   .memoryTypeIndex = memory_type_index,
    // };
    // VkDeviceMemory BufferMemory;
    // vkAllocateMemory(Device, &MemoryAllocateInfo, nullptr, &BufferMemory);
    // vkBindBufferMemory(Device, Buffer, BufferMemory, 0);

    // Map buffer memory and copy Image to
    //VOID *MapPointer;
    //vkMapMemory(Device, BufferMemory, 0, MemoryRequirements.size, 0, &MapPointer);
    //std::memcpy(MapPointer, PixelsData, PixelsDataSize);
    //
    //vkUnmapMemory(Device, BufferMemory);

    //StageBufferUpdate(0, PixelsDataSize, PixelsData);

    // Copy through command buffer
    VkCommandBufferBeginInfo CommandBufferBeginInfo
    {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    };
    vkBeginCommandBuffer(CommandBuffer, &CommandBufferBeginInfo);

    ImageSetLayout(Image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .baseMipLevel = 0, .levelCount = 1, .baseArrayLayer = 0, .layerCount = 1},
      VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_HOST_BIT);

    // Create transfer barrier
    VkImageMemoryBarrier ImageMemoryBarrier1
    {
      .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      .srcAccessMask = VK_ACCESS_HOST_WRITE_BIT,
      .dstAccessMask = 0,
      .oldLayout = Layout,
      .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .image = Image,
      .subresourceRange
      {
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel = 0,
        .levelCount = MipCount,
        .baseArrayLayer = 0,
        .layerCount = 1,
      },
    };
    
    vkCmdPipelineBarrier(CommandBuffer,
                         VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_HOST_BIT, 0, 0, nullptr, 0, nullptr, 1, &ImageMemoryBarrier1);

    // Copy buffer to image
    VkBufferImageCopy Region
    {
      .bufferOffset = 0,
      .bufferRowLength = (UINT)W,
      .bufferImageHeight = (UINT)H,
      .imageSubresource
      {
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .mipLevel = 0,
        .baseArrayLayer = 0,
        .layerCount = 1,
      },
      .imageOffset {.x = 0, .y = 0, .z = 0},
      .imageExtent {.width = (UINT)W, .height = (UINT)H, .depth = 1},
    };
    vkCmdCopyBufferToImage(CommandBuffer, Buffer, Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &Region);

    ImageSetLayout(Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .baseMipLevel = 0, .levelCount = 1, .baseArrayLayer = 0, .layerCount = 1},
      VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_HOST_BIT);

    // Create transfer barrier
    VkImageMemoryBarrier ImageMemoryBarrier2
    {
      .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      .srcAccessMask = VK_ACCESS_HOST_WRITE_BIT,
      .dstAccessMask = 0,
      .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .image = Image,
      .subresourceRange
      {
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel = 0,
        .levelCount = MipCount,
        .baseArrayLayer = 0,
        .layerCount = 1,
      },
    };
    vkCmdPipelineBarrier(CommandBuffer,
                         VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_HOST_BIT, 0, //VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                         0, nullptr, 0, nullptr, 1, &ImageMemoryBarrier2);

    vkEndCommandBuffer(CommandBuffer);
    VkPipelineStageFlags PipelineStageFlags[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    VkSubmitInfo SubmitInfo
    {
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .waitSemaphoreCount = 0,
      .pWaitDstStageMask = PipelineStageFlags,
      .commandBufferCount = 1,
      .pCommandBuffers = &CommandBuffer,
    };
    //vkWaitForFences(Device, 1, &Fence, VK_TRUE, UINT64_MAX);
    //vkResetFences(Device, 1, &Fence);
    vkQueueSubmit(CommandQueue, 1, &SubmitInfo, Fence);
    vkQueueWaitIdle(CommandQueue);
    vkWaitForFences(Device, 1, &Fence, VK_TRUE, UINT64_MAX);

    //ImageGenerateMipmaps(Image, W, H, MipCount, Format, IsCube);

    vmaDestroyBuffer(Allocator, Buffer, Allocation);
  }
  return TRUE;
} /* End of 'pivk::vlk::ImageCreate' function */

/* Vulkan image free function.
 * ARGUMENTS:
 *   - vulkan image handle output reference:
 *       VkImage &Image;
 *   - vulkan image view handle output reference:
 *       VkImageView &ImageView;
 *   - AMD VMA image memory handle output reference:
 *       VmaAllocation &Memory;
 * RETURNS: None.
 */
VOID pivk::vlk::ImageFree( VkImage &Image, VkImageView &ImageView, VmaAllocation &Allocation )
{
  if (ImageView != VK_NULL_HANDLE)
    vkDestroyImageView(Device, ImageView, nullptr);
  if (Image != VK_NULL_HANDLE)
    vmaDestroyImage(Allocator, Image, Allocation);
    //vkDestroyImage(Device, Image, nullptr);
  //if (Memory != VK_NULL_HANDLE)
  //  vkFreeMemory(Device, Memory, nullptr);

  ImageView = VK_NULL_HANDLE;
  Image = VK_NULL_HANDLE;
  Allocation = VK_NULL_HANDLE;
} /* End of 'pivk::vlk::ImageFree' function */

/* Vulkan image set layout function.
 * Create an image memory 'ImageMemoryBarrier' for changing the layout of
 * an Image and put it into an active command buffer
 * ARGUMENTS:
 *   - vulkan image handle:
 *       VkImage Image;
 *   - vulkan image old and new layouts (see VK_IMAGE_LAYOUT_***):
 *       VkImageLayout OldImageLayout, NewImageLayout;
 *   - vulkan image subrsourcse description:
 *       VkImageSubresourceRange SubresourceRange;
 *   - vulkan pipeline source and destination stage flags (see VK_PIPELINE_STAGE_***):
 *       VkPipelineStageFlags SrcStageMask, DstStageMask;
 * RETURNS: None.
 */
VOID pivk::vlk::ImageSetLayout( VkImage Image,
                                VkImageLayout OldImageLayout, VkImageLayout NewImageLayout,
                                VkImageSubresourceRange SubresourceRange,
                                VkPipelineStageFlags SrcStageMask, VkPipelineStageFlags DstStageMask )
{
  // Create an Image ImageMemoryBarrier object
  VkImageMemoryBarrier ImageMemoryBarrier
  {
    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    .oldLayout = OldImageLayout,
    .newLayout = NewImageLayout,
    .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED, // SelectedGraphicsQueueFamilyIndex,
    .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED, // SelectedTransferQueueFamilyIndex,
    .image = Image,
    .subresourceRange = SubresourceRange,
  };

  VkPipelineStageFlags stage = 0;

  if (NewImageLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
  {
    ImageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    ImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  }

  else if (NewImageLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
  {
    ImageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    ImageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  }

  else if (NewImageLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
  {
    ImageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT; // | (Format == VK_FORMAT_D32_SFLOAT_S8_UINT || Format == VK_FORMAT_D24_UNORM_S8_UINT ? VK_IMAGE_ASPECT_STENCIL_BIT : 0);
    ImageMemoryBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    stage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  }

//#if 0
  // Source layouts (old)
  // Source access mask controls actions that have to be finished on the old layout
  // before it will be transitioned to the new layout
  switch (OldImageLayout)
  {
  case VK_IMAGE_LAYOUT_UNDEFINED:
    // Image layout is undefined (or does not matter)
    // Only valid as initial layout
    // No flags required, listed only for completeness
    ImageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_READ_BIT; // 0;
    break;

  case VK_IMAGE_LAYOUT_PREINITIALIZED:
    // Image is preinitialized
    // Only valid as initial layout for linear images, preserves memory contents
    // Make sure host writes have been finished
    ImageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
    break;

  case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
    // Image is a color attachment
    // Make sure any writes to the color buffer have been finished
    ImageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
    break;

  case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
    // Image is a depth/stencil attachment
    // Make sure any writes to the depth/stencil buffer have been finished
    ImageMemoryBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
    break;

  case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
    // Image is a transfer source
    // Make sure any reads from the Image have been finished
    ImageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    break;

  case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
    // Image is a transfer destination
    // Make sure any writes to the Image have been finished
    ImageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    break;

  case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
    // Image is read by a shader
    // Make sure any shader reads from the Image have been finished
    ImageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
    break;
  default:
    // Other source layouts aren't handled (yet)
    break;
  }

  // Target layouts (new)
  // Destination access mask controls the dependency for the new Image layout
  switch (NewImageLayout)
  {
  case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
    // Image will be used as a transfer destination
    // Make sure any writes to the Image have been finished
    ImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    break;

  case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
    // Image will be used as a transfer source
    // Make sure any reads from the Image have been finished
    ImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    break;

  case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
    // Image will be used as a color attachment
    // Make sure any writes to the color buffer have been finished
    ImageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    break;

  case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
    // Image layout will be used as a depth/stencil attachment
    // Make sure any writes to depth/stencil buffer have been finished
    ImageMemoryBarrier.dstAccessMask = ImageMemoryBarrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    break;

  case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
    // Image will be read in a shader (sampler, input attachment)
    // Make sure any writes to the Image have been finished
    if (ImageMemoryBarrier.srcAccessMask == 0)
      ImageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
    ImageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    break;
  default:
    // Other source layouts aren't handled (yet)
    break;
  }
//#endif /* 0 */

  // Put ImageMemoryBarrier inside setup command buffer
  vkCmdPipelineBarrier(CommandBuffer, SrcStageMask, DstStageMask, 0, 0, nullptr, 0, nullptr, 1, &ImageMemoryBarrier);
} /* End of 'pivk::vlk::ImageSetLayout' function */

/* Vulkan image set layout (on first mip level and layer) function.
 * ARGUMENTS:
 *   - vulkan image handle:
 *       VkImage Image;
 *   - vulkan image old and new layouts (see VK_IMAGE_LAYOUT_***):
 *       VkImageLayout OldImageLayout, NewImageLayout;
 *   - vulkan image subrsourcse description:
 *       VkImageSubresourceRange SubresourceRange;
 *   - vulkan pipeline source and destination stage flags (see VK_PIPELINE_STAGE_***):
 *       VkPipelineStageFlags SrcStageMask, DstStageMask;
 * RETURNS: None.
 */
VOID pivk::vlk::ImageSetLayout( VkImage Image,
                                VkImageAspectFlags AspectMask,
                                VkImageLayout OldImageLayout, VkImageLayout NewImageLayout,
                                VkPipelineStageFlags SrcStageMask, VkPipelineStageFlags DstStageMask )
{
  VkImageSubresourceRange SubresourceRange = {};
  SubresourceRange.aspectMask = AspectMask;
  SubresourceRange.baseMipLevel = 0;
  SubresourceRange.levelCount = 1;
  SubresourceRange.layerCount = 1;
  ImageSetLayout(Image, OldImageLayout, NewImageLayout, SubresourceRange, SrcStageMask, DstStageMask);
} /* End of 'pivk::vlk::ImageSetLayout' function */

/* Vulkan Image set layout (on first mip level and layer) function.
 * ARGUMENTS:
 *   - vulkan image handle:
 *       VkImage Image;
 *   - image size:
 *       INT W, H;
 *   - vulkan Image Format (see VK_FORMAT_***):
 *       VkFormat Format;
 *   - cube map flag (create 6 layers and 'PixelData' contain 6 joined same size images):
 *       BOOL IsCube;
 * RETURNS: None.
 */
VOID pivk::vlk::ImageGenerateMipmaps( VkImage Image, INT W, INT H, UINT Levels, VkFormat Format, BOOL IsCube )
{
  if (Levels == 1)
    return;
  VkFormatProperties formatProperties;
  vkGetPhysicalDeviceFormatProperties(PhysDevice, Format, &formatProperties);

  // Copy through command buffer
  VkCommandBufferBeginInfo CommandBufferBeginInfo
  {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
  };
  vkBeginCommandBuffer(CommandBuffer, &CommandBufferBeginInfo);

  VkImageMemoryBarrier ImageMemoryBarrier
  {
    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .image = Image,
    .subresourceRange
    {
      .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      .baseMipLevel = 0,
      .levelCount = 1,
      .baseArrayLayer = 0,
      .layerCount = IsCube ? 6U : 1U,
  },
  };

  INT MipWidth = W, MipHeight = H;
  for (UINT MipLevel = 1; MipLevel < Levels; MipLevel++)
  {
    ImageMemoryBarrier.subresourceRange.baseMipLevel = MipLevel - 1;
    ImageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    ImageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    ImageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    ImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    vkCmdPipelineBarrier(CommandBuffer,
                         VK_PIPELINE_STAGE_TRANSFER_BIT/*VK_PIPELINE_STAGE_HOST_BIT*/, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, //VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                         0, nullptr, 0, nullptr, 1, &ImageMemoryBarrier);

    ImageMemoryBarrier.subresourceRange.baseMipLevel = MipLevel;
    ImageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;//VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    ImageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    ImageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    ImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    vkCmdPipelineBarrier(CommandBuffer,
                         VK_PIPELINE_STAGE_TRANSFER_BIT/*VK_PIPELINE_STAGE_HOST_BIT*/, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, //VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                         0, nullptr, 0, nullptr, 1, &ImageMemoryBarrier);

    VkImageBlit blit
    {
      .srcSubresource
      {
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .mipLevel = MipLevel - 1,
        .baseArrayLayer = 0,
        .layerCount = IsCube ? 6U : 1U,
    },
    .srcOffsets
      {
        {.x = 0, .y = 0, .z = 0},
      {.x = MipWidth, .y = MipHeight, .z = 1},
    },
    .dstSubresource
      {
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .mipLevel = MipLevel,
        .baseArrayLayer = 0,
        .layerCount = IsCube ? 6U : 1U,
    },
    .dstOffsets
      {
        {.x = 0, .y = 0, .z = 0},
      {.x = MipWidth > 1 ? MipWidth / 2 : 1, .y = MipHeight > 1 ? MipHeight / 2 : 1, .z = 1},
    },
    };
    vkCmdBlitImage(CommandBuffer, Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                   Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

    ImageMemoryBarrier.subresourceRange.baseMipLevel = MipLevel - 1;
    ImageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    ImageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    ImageMemoryBarrier.srcAccessMask = 0*VK_ACCESS_TRANSFER_READ_BIT;
    ImageMemoryBarrier.dstAccessMask = 0*VK_ACCESS_TRANSFER_WRITE_BIT;
    vkCmdPipelineBarrier(CommandBuffer,
                         VK_PIPELINE_STAGE_TRANSFER_BIT/*VK_PIPELINE_STAGE_HOST_BIT*/, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, //VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                         0, nullptr, 0, nullptr, 1, &ImageMemoryBarrier);

    ImageMemoryBarrier.subresourceRange.baseMipLevel = MipLevel;
    ImageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;//VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    ImageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    ImageMemoryBarrier.srcAccessMask = 0*VK_ACCESS_TRANSFER_WRITE_BIT;
    ImageMemoryBarrier.dstAccessMask = 0*VK_ACCESS_TRANSFER_READ_BIT;
    vkCmdPipelineBarrier(CommandBuffer,
                         VK_PIPELINE_STAGE_TRANSFER_BIT/*VK_PIPELINE_STAGE_HOST_BIT*/, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, //VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                         0, nullptr, 0, nullptr, 1, &ImageMemoryBarrier);

    if (MipWidth > 1)
      MipWidth /= 2;
    if (MipHeight > 1)
      MipHeight /= 2;
  }
  vkEndCommandBuffer(CommandBuffer);
  VkPipelineStageFlags PipelineStageFlags[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  VkSubmitInfo SubmitInfo
  {
    .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
    .waitSemaphoreCount = 0,
    .pWaitDstStageMask = PipelineStageFlags,
    .commandBufferCount = 1,
    .pCommandBuffers = &CommandBuffer,
  };
  vkWaitForFences(Device, 1, &Fence, VK_TRUE, UINT64_MAX);
  vkResetFences(Device, 1, &Fence);
  vkQueueSubmit(CommandQueue, 1, &SubmitInfo, Fence);
  vkQueueWaitIdle(CommandQueue);
  vkWaitForFences(Device, 1, &Fence, VK_TRUE, UINT64_MAX);
} /* End of 'pivk::vlk::ImageGenerateMipmaps' function */

/* END OF 'vlk_image.cpp' FILE */
