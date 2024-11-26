/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        vlk_renderpass.cpp
 * PROGRAMMER:  IP5
 * LAST UPDATE: 28.10.2024
 * PURPOSE:     Vulkan core render pass file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#include "pivk.h"

/* Base project namespace */
namespace pivk
{
  /* Create vulkan render pass function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID vlk::CreateRenderPass( VOID )
  {
    /* Swapchain image format */
    VkFormat SwapchainImageFormat =
    {
      VK_FORMAT_B8G8R8A8_SRGB
    };

    /* Structure about G-Buffer. */
    VkAttachmentDescription AttachmentDescription
    {
      .flags = 0,                                         // Flags.
      .format = SwapchainImageFormat,                     // Format of imaging attachment.
      .samples = VK_SAMPLE_COUNT_1_BIT,                   // Multisampling.
      .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,              // What happens to the data when the subpass starts.
      .storeOp = VK_ATTACHMENT_STORE_OP_STORE,            // What happens when trying to anything writing.
      .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,   // The same, but for Stencil Buffer.
      .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE, // 
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,         // What layout should the image have at the beginning of rendering.
      .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,     // What layout the image will have in the end.
    };
    
    /* Structure, referenced to definite G-Buffer and showing his condition during subpass */
    VkAttachmentReference ColorAttachmentReference
    {
      .attachment = 0,                                    // Index of attachment in the general list.
      .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, // Layout of image attachment during passage.
    };

    /* Structor about Depth-Buffer */
    VkAttachmentDescription DepthBufferDescription
    {
      .flags = 0,
      .format = FindSupportedFormat(PhysDevice, 
                                    {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
                                    VK_IMAGE_TILING_OPTIMAL,
                                    VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT),
      .samples = VK_SAMPLE_COUNT_1_BIT,
      .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
      .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
      .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    };

    /* Structure - reference G-Buffer */
    VkAttachmentReference DepthReference =
    {
      .attachment = 1,
      .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    };
    
    /* Structure, describe one subpass */
    VkSubpassDescription SubpassDescription
    {
      .flags = 0,                                           // Flags.
      .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS, // What type pipeline uses this subpass.
      .inputAttachmentCount = 0,                            // Count of input attachments.
      .pInputAttachments = nullptr,                         // Their VkAttachmentReference[].
      .colorAttachmentCount = 1,                            // Count of output attachments.
      .pColorAttachments = &ColorAttachmentReference,       // Their VkAttachmentReference[].
      .pResolveAttachments = nullptr,                       // Attachments for multisampling.
      .pDepthStencilAttachment = &DepthReference,           // Reference Depthbuffer, who participates in the passage.
      .preserveAttachmentCount = 0,                         // Attachments, which in this subpass
                                                            // do not particiable directly, but data which should be
                                                            // saving for using in further subpasses.
      .pPreserveAttachments = nullptr,
    };

    /* Structure, describing passage between subpasses */
    VkSubpassDependency SubpassDependency
    {
      .srcSubpass = VK_SUBPASS_EXTERNAL,                              // Index of subpass, from which we are moving.
                                                                      // VK_SUBPASS_EXTERNAL means, that srcSubpass,
                                                                      // not, so we only starting self passage.
      .dstSubpass = 0,                                                // Index of subpass, in which we moving.
      .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | // Condition all Pipeline in srcSubpass, in
                      VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,     // which will be transition has been made in dstSubpass.
      .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | // Condition, from which opportunities will be obtained
                      VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,     // Access from dstAccessMask.
      .srcAccessMask = 0,                                             // Access to G-Buffer in srcSubpass.
      .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |         // Access to G-Buffer in dstSubpass.
                       VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
      .dependencyFlags = 0,                                           // Flags
    };

    // Array with attachments
    std::array<VkAttachmentDescription, 2> attachments = {AttachmentDescription, DepthBufferDescription};
    
    /* Structure, contains all information for creating renderpass */
    VkRenderPassCreateInfo CreateInfo
    {
      .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO, // Type of structure.
      .pNext = nullptr,                                   // Pointer to structure-extension, if she exist.
      .flags = 0,                                         // Flags.
      .attachmentCount = UINT32(attachments.size()),      // Count of color attachments in all renderpass.
      .pAttachments = attachments.data(),                 // Their description.
      .subpassCount = 1,                                  // Count of sub-passages of rendering.
      .pSubpasses = &SubpassDescription,                  // Description of sub-passages.
      .dependencyCount = 1,                               // Count of transitions between subpasses.
      .pDependencies = &SubpassDependency,                // Their descriptions.
    };
   
    
    if (vkCreateRenderPass(Device, &CreateInfo, nullptr, &RenderPass) != VK_SUCCESS)
    {
      MessageBox(this->hWndRef, "Error of creating vulkan render pass", "T53VLK :: ERROR", MB_OK | MB_ICONERROR);
      std::exit(3); // Processing error.
    }

    // Initialize viewport
    Viewport.x = 0;
    Viewport.y = FrameH;
    Viewport.width = FrameW;
    Viewport.height = -(INT)FrameH;
    Viewport.minDepth = 0.0f;
    Viewport.maxDepth = 1.0f;

    // Initialize scissor
    Scissor.extent.width = FrameW;
    Scissor.extent.height = FrameH;
    Scissor.offset.x = 0;
    Scissor.offset.y = 0;
  } /* End of 'vlk::CreateRenderPass' function */

  /* Begin render pass function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID vlk::RenderPassBegin( VOID )
  {
    if (vkResetFences(Device, 1, &Fence) != VK_SUCCESS)
    {
      throw;
    }
    if (vkAcquireNextImageKHR(Device, Swapchain, UINT64_MAX, VK_NULL_HANDLE, Fence, &CurImgIdx) != VK_SUCCESS)
    {
      throw;
    }
    if (vkWaitForFences(Device, 1, &Fence, TRUE, UINT64_MAX))
    {
      throw;
    }
    if (vkResetCommandBuffer(CommandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT) != VK_SUCCESS)
    {
      throw;
    }

    VkCommandBufferBeginInfo CommandBufferBeginInfo
    {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .flags = 0,
    };

    if (vkBeginCommandBuffer(CommandBuffer, &CommandBufferBeginInfo) != VK_SUCCESS)
    {
      throw;
    }

    std::array<VkClearValue, 2> ClearValues
    {
      VkClearValue{.color = {0.3f, 0.47f, 0.8f, 1.0f}},
      VkClearValue{.depthStencil = {1.0f, 0}},
    };

    VkRenderPassBeginInfo BeginInfo
    {
      .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
      .renderPass = RenderPass,
      .framebuffer = FrameBuffers[CurImgIdx],
      .renderArea =
      {
        .offset = {0, 0},
        .extent = {FrameW, FrameH},
      },
      .clearValueCount = UINT32(ClearValues.size()),
      .pClearValues = ClearValues.data()
    };

    vkCmdBeginRenderPass(CommandBuffer, &BeginInfo, VK_SUBPASS_CONTENTS_INLINE);
  } /* End of 'RenderPassBegin' function */

  /* Begin render pass function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID vlk::RenderPassEnd( VOID )
  {
    vkCmdEndRenderPass(CommandBuffer);
    vkEndCommandBuffer(CommandBuffer);

    if (vkResetFences(Device, 1, &Fence) != VK_SUCCESS)
    {
      throw;
    }

    VkSubmitInfo SubmitInfo
    {
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .waitSemaphoreCount = 0,
      .pWaitSemaphores = nullptr,
      .pWaitDstStageMask = nullptr,
      .commandBufferCount = 1,
      .pCommandBuffers = &CommandBuffer,
      .signalSemaphoreCount = 0,
      .pSignalSemaphores = nullptr,
    };

    if (vkQueueSubmit(CommandQueue, 1, &SubmitInfo, Fence) != VK_SUCCESS)
    {
      throw;
    }

    vkWaitForFences(Device, 1, &Fence, TRUE, UINT64_MAX);

    VkPresentInfoKHR PresentInfo
    {
      .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
      .waitSemaphoreCount = 0,
      .pWaitSemaphores = nullptr,
      .swapchainCount = 1,
      .pSwapchains = &Swapchain,
      .pImageIndices = &CurImgIdx,
    };

    vkQueuePresentKHR(CommandQueue, &PresentInfo);
  } /* End of 'RenderPassEnd' function */

} /* end of 'pivk' namespace */

/* END OF 'vlk_renderpass.cpp' FILE */
