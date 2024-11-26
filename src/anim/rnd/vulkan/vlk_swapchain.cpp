/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        vlk_swapchain.cpp
 * PROGRAMMER:  IP5
 * LAST UPDATE: 28.10.2024
 * PURPOSE:     Vulkan core swapchain file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#include "pivk.h"

/* Base project namespace */
namespace pivk
{
  /* Create swapchain of vulkan core function
   * ARGUMENTS:
   *   - new size:
   *       UINT NewFrameW, NewFrameH;
   *   - old swapchain (nullptr if this is first creating of swapchain):
   *       VkSwapchainKHR OldSwapchain;
   * RETURNS: None.
   */
  VOID vlk::CreateSwapchain( UINT NewFrameW, UINT NewFrameH, VkSwapchainKHR OldSwapchain )
  {
    FrameW = NewFrameW;
    FrameH = NewFrameH;

    VkSwapchainKHR TmpSwapchain{};

    VkSwapchainCreateInfoKHR SwapchainCreateInfo 
    {
      .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
      .flags = 0,
      .surface = Surface,
      .minImageCount = 3, // Minimal count of images, required swapchain.

      // In some cases required check, is support
      // these parameters of phys device for this surface.
      // For checking necessary take advantage
      // vkGetPhysicalDeviceSurfaceFormatsKHR(...).
      .imageFormat = VK_FORMAT_B8G8R8A8_SRGB,               // Settings color components and types.
      .imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR, // Color space used, 
                                                            // in which colors are set.
      .imageExtent = {FrameW, FrameH},
    
      .imageArrayLayers = 1,                                // Count of layers when rendering.
      .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,

      .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
      .queueFamilyIndexCount = 0,
      .pQueueFamilyIndices = nullptr,

      .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,

      .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
      .presentMode = VK_PRESENT_MODE_FIFO_KHR, 
      .clipped = VK_TRUE, 
      .oldSwapchain = OldSwapchain,
    };

    VkResult Res;
    //VkSurfaceCapabilitiesKHR Capabilities;

    std::vector<VkPresentModeKHR> PresentModes;
    UINT32 CountOfPresentModes;

    vkGetPhysicalDeviceSurfacePresentModesKHR(PhysDevice, Surface, &CountOfPresentModes, nullptr);
    PresentModes.resize(CountOfPresentModes);
    vkGetPhysicalDeviceSurfacePresentModesKHR(PhysDevice, Surface, &CountOfPresentModes, PresentModes.data());

    //VkResult res47 = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(PhysDevice, Surface, &Capabilities);


    if ((Res = vkCreateSwapchainKHR(Device, &SwapchainCreateInfo, nullptr, &TmpSwapchain)) != VK_SUCCESS)
    {
      MessageBox(hWndRef, "Error of creating vulkan swapchain", "T53VLK :: ERROR", MB_OK | MB_ICONERROR);
      std::exit(3);
    }

    UINT32 SwapchainImagesCount;

    if (vkGetSwapchainImagesKHR(Device, TmpSwapchain, &SwapchainImagesCount, nullptr) != VK_SUCCESS)
    {
      MessageBox(hWndRef, "Error of creating vulkan swapchain", "T53VLK :: ERROR", MB_OK | MB_ICONERROR);
      std::exit(3);
    }

    SwapchainImages.resize(SwapchainImagesCount);
    SwapchainImageViews.resize(SwapchainImagesCount);

    if (vkGetSwapchainImagesKHR(Device, TmpSwapchain, &SwapchainImagesCount, &SwapchainImages[0]) != VK_SUCCESS)
    {
      MessageBox(hWndRef, "Error of creating vulkan swapchain", "T53VLK :: ERROR", MB_OK | MB_ICONERROR);
      std::exit(3);
    }

    for (INT i = 0; i < SwapchainImagesCount; i++) 
    {
      VkImageViewCreateInfo ImageViewCreateInfo 
      {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = SwapchainImages[i],

        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = VK_FORMAT_B8G8R8A8_SRGB,
        .components =
        {
          .r = VK_COMPONENT_SWIZZLE_IDENTITY,
          .g = VK_COMPONENT_SWIZZLE_IDENTITY,
          .b = VK_COMPONENT_SWIZZLE_IDENTITY,
          .a = VK_COMPONENT_SWIZZLE_IDENTITY,
        },
        .subresourceRange =
        {
          .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
          .baseMipLevel = 0,
          .levelCount = 1,
          .baseArrayLayer = 0,
          .layerCount = 1,
        },
      };

      if (vkCreateImageView(Device, &ImageViewCreateInfo, nullptr, &SwapchainImageViews[i]) != VK_SUCCESS)
      {
        MessageBox(hWndRef, "Error of creating vulkan swapchain", "T53VLK :: ERROR", MB_OK | MB_ICONERROR);
        std::exit(3);
      }
    }

    VkFormat DepthFormat = FindSupportedFormat(PhysDevice,
                                               {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
                                               VK_IMAGE_TILING_OPTIMAL,
                                               VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

    VkImageCreateInfo ImageInfo {};
    ImageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    ImageInfo.imageType = VK_IMAGE_TYPE_2D;
    ImageInfo.extent.width = static_cast<UINT32>(FrameW);
    ImageInfo.extent.height = static_cast<UINT32>(FrameH);
    ImageInfo.extent.depth = 1;
    ImageInfo.mipLevels = 1;
    ImageInfo.arrayLayers = 1;
    ImageInfo.format = DepthFormat;
    ImageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    ImageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    ImageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    ImageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    ImageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    ImageInfo.flags = 0; // Optional

    // Create image through VMA
    VmaAllocationCreateInfo AllocationCreateInfo {};
    AllocationCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
    AllocationCreateInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    AllocationCreateInfo.flags = 0;
    AllocationCreateInfo.priority = 1.0f;

    if (vmaCreateImage(Allocator, &ImageInfo, &AllocationCreateInfo,
        &DepthImage, &DepthAllocation, nullptr) != VK_SUCCESS)
      throw;

    // Create depth buffer image view
    VkImageViewCreateInfo ImageViewCreateInfo 
    {
      .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
      .image = DepthImage,
      .viewType = VK_IMAGE_VIEW_TYPE_2D,
      .format = DepthFormat,
      .components =
      {
        .r = VK_COMPONENT_SWIZZLE_IDENTITY,
        .g = VK_COMPONENT_SWIZZLE_IDENTITY,
        .b = VK_COMPONENT_SWIZZLE_IDENTITY,
        .a = VK_COMPONENT_SWIZZLE_IDENTITY,
      },
      .subresourceRange =
      {
        .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
        .baseMipLevel = 0,
        .levelCount = 1,
        .baseArrayLayer = 0,
        .layerCount = 1,
      },
    };

    if (vkCreateImageView(Device, &ImageViewCreateInfo, nullptr, &DepthImageView) != VK_SUCCESS)
    {
      throw;
    }
    
    // Create temporary command buffer
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = CommandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer TmpCommandBuffer;
    vkAllocateCommandBuffers(Device, &allocInfo, &TmpCommandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(TmpCommandBuffer, &beginInfo);

    // Work with tmp cmd buf
    VkPipelineStageFlags SourceStage;
    VkPipelineStageFlags DestinationStage;

    VkImageLayout OldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    VkImageLayout NewLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    //VkFormat FormatTrans;

    VkImageMemoryBarrier barrier{};

    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = OldLayout;
    barrier.newLayout = NewLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = DepthImage;
    //barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

    if (NewLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
    {
      barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
      if (DepthFormat == VK_FORMAT_D32_SFLOAT_S8_UINT || DepthFormat == VK_FORMAT_D24_UNORM_S8_UINT)
        barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    } 
    else 
      barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.srcAccessMask = 0; // ?
    barrier.dstAccessMask = 0; // ?

    if (OldLayout == VK_IMAGE_LAYOUT_UNDEFINED && NewLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
      barrier.srcAccessMask = 0;
      barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      
      SourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
      DestinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (OldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && NewLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
      barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    
      SourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
      DestinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (OldLayout == VK_IMAGE_LAYOUT_UNDEFINED && NewLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
    {
      barrier.srcAccessMask = 0;
      barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    
      SourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
      DestinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    } 
    else 
    {
      throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(TmpCommandBuffer,
                         SourceStage, DestinationStage,
                         0,
                         0, nullptr,
                         0, nullptr,
                         1, &barrier);

    vkEndCommandBuffer(TmpCommandBuffer);

    // Delete tmp cmd buf
    VkSubmitInfo SubmitInfo{};
    SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    SubmitInfo.commandBufferCount = 1;
    SubmitInfo.pCommandBuffers = &TmpCommandBuffer;

    vkQueueSubmit(CommandQueue, 1, &SubmitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(CommandQueue);
    vkFreeCommandBuffers(Device, CommandPool, 1, &TmpCommandBuffer);

    // Finally, delete old swapchain
    if (OldSwapchain != VK_NULL_HANDLE)
      vkDestroySwapchainKHR(Device, OldSwapchain, nullptr);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &TmpCommandBuffer;

    Swapchain = TmpSwapchain;
  } /* End of 'vlk::CreateSwapchain' function */

  /* Resize vulkan swapchain function.
   * ARGUMENTS: 
   *   - new size:
   *       INT NewW, NewH;
   * RETURNS: None.
   */
  VOID vlk::ResizeSwapchain( INT NewW, INT NewH )
  {
    vkDeviceWaitIdle(Device);

    //for (INT i = 0; i < FrameBuffers.size(); i++)
    //  vkDestroyFramebuffer(Device, FrameBuffers[i], nullptr);

    for (INT i = 0; i < SwapchainImageViews.size(); i++)
      vkDestroyImageView(Device, SwapchainImageViews[i], nullptr);
    vkDestroyImageView(Device, DepthImageView, nullptr);
    vmaDestroyImage(Allocator, DepthImage, DepthAllocation);

    // Recreating swapchain
    CreateSwapchain(NewW, NewH, Swapchain);
  } /* End of 'vlk::ResizeSwapchain' function */

} /* end of 'pivk' namespace */

/* END OF 'vlk_swapchain.cpp' FILE */
