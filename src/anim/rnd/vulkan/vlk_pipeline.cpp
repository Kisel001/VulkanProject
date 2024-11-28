/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        vlk_pipeline.cpp
 * PROGRAMMER:  IP5
 * LAST UPDATE: 29.10.2024
 * PURPOSE:     Vulkan pipeline file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#include "pivk.h"

/* Base project namespace */
namespace pivk
{
  /* Create vulkan pipeline layout function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID vlk::CreatePipelineLayout( VOID )
  {
    CountOfDescriptorSets = 2;

    /* Create push constants range for pipeline layout */
    VkPushConstantRange Range {};
    Range.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;
    Range.offset = 0;
    Range.size = sizeof(render::DRAW_BUF);

    std::array<VkDescriptorSetLayout, 2> DescriptorSetLayouts = {DescriptorSetLayout, TexDescriptorSetLayout};

    // Setup pipeline layout
    VkPipelineLayoutCreateInfo PipelineLayoutInfo =
    {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .setLayoutCount = 2,                        // Optional
      .pSetLayouts = DescriptorSetLayouts.data(), // Optional
      .pushConstantRangeCount = 1,                // Optional
      .pPushConstantRanges = &Range,              // Optional
    };

    // Create pipeline layout
    if (vkCreatePipelineLayout(Device, &PipelineLayoutInfo, nullptr, &PipelineLayout) != VK_SUCCESS)
    {
      throw;
      exit(3);
    }
  } /* End of 'vlk::CreatePipelineLayout' function */

}; /* end of 'pivk' namespace */

/* END OF 'vlk_pipeline.cpp' FILE */
