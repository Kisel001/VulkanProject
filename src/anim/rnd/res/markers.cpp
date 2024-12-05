/*************************************************************
* Copyright (C) 2024
*    Computer Graphics Support Group of 30 Phys-Math Lyceum
*************************************************************/

/* FILE NAME   : markers.cpp
 * PURPOSE     : Animation pattern on Vulkan API project.
 *               Markers class implementation module.
 * PROGRAMMER  : CGSG'Sr'2023.
 *               IP5.
 * LAST UPDATE : 26.11.2024.
 * NOTE        : Module namespace 'pivk'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
 
#include "pivk.h"

/* Markers initialization funtion.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID pivk::marker_manager::MarkersInit( VOID )
{
  MarkerPipelinePattern = RndRef.PipelinePatternFind("Markers pipeline pattern");

  MarkersShds[0] = RndRef.ShdCreate("markers/sphere", MarkerPipelinePattern);
  MarkersShds[1] = RndRef.ShdCreate("markers/cylinder", MarkerPipelinePattern);

  // Pipeline layout setup
  //std::vector<VkDescriptorSetLayout> DescriptorSetLayouts
  //{
  //  RndRef.VulkanCore.DescriptorSetLayout,
  //};
  //std::vector<VkPushConstantRange> PushConstantRanges
  //{
  //  {
  //    .stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS,
  //    .offset = 0,
  //    .size = 256, // 256
  //  }
  //};
  //VkPipelineLayoutCreateInfo PipelineLayoutCreateInfo
  //{
  //  .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
  //  .setLayoutCount = static_cast<UINT>(DescriptorSetLayouts.size()),
  //  .pSetLayouts = DescriptorSetLayouts.data(),
  //  .pushConstantRangeCount = static_cast<UINT>(PushConstantRanges.size()),
  //  .pPushConstantRanges = PushConstantRanges.data(),
  //};
  //vkCreatePipelineLayout(RndRef.VulkanCore.Device, &PipelineLayoutCreateInfo, nullptr, &PipelineLayouts[0]);
  //PipelineLayouts[1] = PipelineLayouts[0];

  MarkerVertexBuffer = RndRef.VulkanCore.BufCreate(0, 0, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
} /* End of 'pivk::marker_manager::MarkersInit' function */

/* Markers deinitialization funtion.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID pivk::marker_manager::MarkersClose( VOID )
{
  //if (VertexBuffer != VK_NULL_HANDLE)
  //  RndRef.VulkanCore.BufFree(VertexBuffer, VertexBufferAllocation);
  MarkerVertexBuffer = nullptr;
  ///VertexBufferAllocation = VK_NULL_HANDLE;
} /* End of 'vigl::marker_manager::MarkersClose' function */

  /* Draw all markers function.
  * ARGUMENTS: None.
  * RETURNS: None.
  */
VOID pivk::marker_manager::DrawAllMarkers( VOID )
{
  matr
    w = matr::Identity(),
    wvp = w * RndRef.Camera.VP,
    invw = w.Inverse();

  for (INT i = 0; i < 2; i++)
  {
    //if (MarkersShds[i] == VK_NULL_HANDLE)
    //{
    //  MarkersShds[i] = 
    //}
    //if (Shds[i]->IsReload)
    //{
    //  Shds[i]->IsReload = FALSE;
    //  if (Pipelines[i] != VK_NULL_HANDLE)
    //    vkDestroyPipeline(RndRef.Vlk.Device, Pipelines[i], nullptr);
    //  Pipelines[i] = VK_NULL_HANDLE;
    //}
  
    if (MarkersPipelines[i] == VK_NULL_HANDLE)
      MarkersPipelines[i] = MarkersShds[i]->Pipeline;
  }

  // Draw markers:
  //static PFN_vkCmdSetPolygonModeEXT vkCmdSetPolygonModeEXT = reinterpret_cast<PFN_vkCmdSetPolygonModeEXT>(vkGetDeviceProcAddr(RndRef.VulkanCore.Device, "vkCmdSetPolygonModeEXT"));
  // reinterpret_cast<PFN_vkCmdSetPolygonModeEXT>(vkGetInstanceProcAddr(RndRef.Vlk.Instance, "vkCmdSetPolygonModeEXT"));

  //if (vkCmdSetPolygonModeEXT != nullptr)
  //  if ((GetAsyncKeyState('W') & 0x8000) != 0 && (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0)
  //    vkCmdSetPolygonModeEXT(RndRef.VulkanCore.CommandBuffer, VK_POLYGON_MODE_LINE);
  //  else
  //    vkCmdSetPolygonModeEXT(RndRef.VulkanCore.CommandBuffer, VK_POLYGON_MODE_FILL);

  try
  {
    if (MarkersPipelines[0] != VK_NULL_HANDLE)
    {
      //vkCmdBindDescriptorSets(RndRef.VulkanCore.CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, PipelineLayouts[0],
      //                        0, 1, &RndRef.VulkanCore.DescriptorSet, 0, nullptr);
      vkCmdBindPipeline(RndRef.VulkanCore.CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, MarkersPipelines[0]);
      RndRef.VulkanCore.BindDescriptor(0);

      render::DRAW_BUF bp 
      {
        w, wvp, invw, // Matrixes
        {}, {},       // Other data
      };
    
      for (auto &sph : Spheres)
      {
        bp.MarkersData[0] = std::get<0>(sph);
        bp.MarkersData[1] = std::get<1>(sph);
        VkBuffer VertexBuffers[] = {MarkerVertexBuffer->Buffer};
        VkDeviceSize Offsets[] = {0};
        vkCmdPushConstants(RndRef.VulkanCore.CommandBuffer, RndRef.VulkanCore.PipelineLayout, VK_SHADER_STAGE_ALL_GRAPHICS, 0, sizeof(bp), &bp);
        vkCmdBindVertexBuffers(RndRef.VulkanCore.CommandBuffer, 0, 1, VertexBuffers, Offsets);
        vkCmdDraw(RndRef.VulkanCore.CommandBuffer, 1, 1, 0, 0);
      }
    }
  }
  catch ( std::exception &e )
  {
    OutputDebugString("Exception\n");
    OutputDebugString(e.what());
  }
  catch ( ... )
  {
    OutputDebugString("Exception\n");
  }
  try
  {
    if (MarkersPipelines[1] != VK_NULL_HANDLE)
    {
      //vkCmdBindDescriptorSets(RndRef.VulkanCore.CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, PipelineLayouts[1],
      //                        0, 1, &RndRef.VulkanCore.FrameDescriptorSet, 0, nullptr);
      vkCmdBindPipeline(RndRef.VulkanCore.CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, MarkersPipelines[1]);
      RndRef.VulkanCore.BindDescriptor(0);

      render::DRAW_BUF bp 
      {
        w, wvp, invw, // Matrixes
        {}, {},       // Other data
      };
    
      for (auto &cyl : Cylinders)
      {
        bp.MarkersData[0] = std::get<0>(cyl);
        bp.MarkersData[1] = std::get<1>(cyl);
        bp.MarkersData[2] = std::get<2>(cyl);
        VkBuffer VertexBuffers[] = {MarkerVertexBuffer->Buffer};
        VkDeviceSize Offsets[] = {0};
        vkCmdPushConstants(RndRef.VulkanCore.CommandBuffer, RndRef.VulkanCore.PipelineLayout, VK_SHADER_STAGE_ALL_GRAPHICS, 0, sizeof(bp), &bp);
        vkCmdBindVertexBuffers(RndRef.VulkanCore.CommandBuffer, 0, 1, VertexBuffers, Offsets);
        vkCmdDraw(RndRef.VulkanCore.CommandBuffer, 1, 1, 0, 0);
      }
    }
  }
  catch ( std::exception &e )
  {
    OutputDebugString("Exception\n");
    OutputDebugString(e.what());
  }
  catch ( ... )
  {
    OutputDebugString("Exception\n");
  }
  Spheres.clear();
  Cylinders.clear();
} /* End of 'pivk::marker_manager::DrawAllMarkers' function */

/* END OF 'markers.cpp' FILE */
