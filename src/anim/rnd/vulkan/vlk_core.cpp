/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        vlk_core.cpp
 * PROGRAMMER:  IP5
 * LAST UPDATE: 28.10.2024
 * PURPOSE:     Vulkan core file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#include "pivk.h"

/* Base project namespace */
namespace pivk
{
  /* Init vulkan core function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID vlk::Init( VOID )
  {
    CreateInstance();
    CreatePhysDevice();
    CreateDevice();
    CreateSurface();
    CreateCommandQueue();
    CreateCommandPool();
    CreateCommandBuffer();
    CreateSwapchain(47, 47);
    CreateRenderPass();
    CreateFrameBuffers();
    CreateDescriptors();
    CreatePipelineLayout();
    vlk_descriptor_manager::CreateDescriptors();
  } /* End of 'vlk::Init' function */

  /* Vulkan core close function
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID vlk::Close( VOID )
  {
    vlk_descriptor_manager::FreeDescriptors();
    vlk_buf_manager::BufsFree();
    vkDestroyDescriptorPool(Device, DescriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(Device, TexDescriptorSetLayout, nullptr);
    vkDestroyDescriptorSetLayout(Device, DescriptorSetLayout, nullptr);
    vkDestroyPipelineLayout(Device, PipelineLayout, nullptr);
    vkDestroyFence(Device, Fence, nullptr);
    vkDestroyCommandPool(Device, CommandPool, nullptr);
    for (VkFramebuffer FrameBuffer : FrameBuffers)
      vkDestroyFramebuffer(Device, FrameBuffer, nullptr);
    vkDestroyImageView(Device, DepthImageView, nullptr);
    vmaDestroyImage(Allocator, DepthImage, DepthAllocation);
    vkDestroyRenderPass(Device, RenderPass, nullptr); 
    for (INT i = 0; i < SwapchainImageViews.size(); ++i)
      vkDestroyImageView(Device, SwapchainImageViews[i], nullptr);
    vkDestroySwapchainKHR(Device, Swapchain, nullptr);
    vkDestroySurfaceKHR(Instance, Surface, nullptr);

    // Destroy VMA allocator
    vmaDestroyAllocator(Allocator);

    vkDestroyDevice(Device, nullptr);
    vkDestroyInstance(Instance, nullptr);
  } /* End of 'vlk::Close' function */

  /* Vulkan core resize function.
   * ARGUMENTS:
   *   - frame size:
   *       UINT NewW, NewH;
   * RETURNS: None.
   */
  VOID vlk::Resize( UINT NewW, UINT NewH )
  {
    ResizeSwapchain(NewW, NewH);

    // Resize framebuffers
    for (auto &i : FrameBuffers)
      vkDestroyFramebuffer(Device, i, nullptr);
    CreateFrameBuffers();


    //Viewport.x = 0;
    Viewport.y = NewH;
    Viewport.width = NewW;
    Viewport.height = -(INT)NewH;

    // Resize viewport and scissor
    //Viewport.y = FrameH;
    //Viewport.width = FrameW;
    ////Viewport.height = FrameH;
    //Viewport.height = -INT(FrameH);
    Scissor.extent.width = FrameW;
    Scissor.extent.height = FrameH;
  } /* End of 'vlk::Resize' function */

  /* Vulkan render start function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID vlk::RenderStart( VOID )
  {
    RenderPassBegin();
  } /* End of 'vlk::RenderStart' function */

  /* Vulkan render end function.
   * ARGUMETNS: None.
   * RETURNS: None.
   */
  VOID vlk::RenderEnd( VOID )
  {
    RenderPassEnd();
  } /* End of 'vlk::RenderEnd' function  */

  /* Get pointer to vulkan core by render pointer function
   * ARGUMETNS:
   *   - pointer to render:
   *       render *Rnd;
   * RETURNS:
   *   (vlk *) pointer to vulkan core.
   */
  vlk * GetVlk( render *Rnd )
  {
    return &Rnd->VulkanCore;
  } /* End of 'GetVlk' function */

} /* end of 'pivk' namespace */

/* END OF 'vlk_core.cpp' FILE */
