/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        vlk_surface.cpp
 * PROGRAMMER:  IP5
 * LAST UPDATE: 28.10.2024
 * PURPOSE:     Vulkan core surface file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#include "pivk.h"

/* Base project namespace */
namespace pivk
{
  /* Create frame buffers function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID vlk::CreateFrameBuffers( VOID )
  {
    FrameBuffers.resize(SwapchainImageViews.size());
    for (INT i = 0; i < SwapchainImageViews.size(); i++)
    {
      std::array<VkImageView, 2> Attachments =
      {
        SwapchainImageViews[i],
        DepthImageView
      };

      VkFramebufferCreateInfo FrameBufferCreateInfo
      {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .renderPass = RenderPass,
        .attachmentCount = static_cast<UINT32>(Attachments.size()),
        .pAttachments = Attachments.data(),
        .width = FrameW,
        .height = FrameH,
        .layers = 1,                                                   // Count of render buffer layers.
      };
    
      if (vkCreateFramebuffer(Device, &FrameBufferCreateInfo, nullptr, &FrameBuffers[i]) != VK_SUCCESS)
      {
        exit(3);
      }
    }
  } /* End of 'vlk::CreateFrameBuffers' function */



} /* end of 'pivk' namespace */

/* END OF 'vlk_surface.cpp' FILE */
