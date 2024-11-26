/*************************************************************
* Copyright (C) 2024
*    Computer Graphics Support Group of 30 Phys-Math Lyceum
*************************************************************/

/* FILE NAME   : markers.cpp
 * PURPOSE     : Animation pattern on Vulkan API project.
 *               Markers class implementation module.
 * PROGRAMMER  : CGSG'Jr'2024.
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 26.11.2024.
 * NOTE        : Module namespace 'vigl'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
 
#include "pivk.h"

/* Markers initialization funtion.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID pivk::marker_manager::Init( VOID )
{
  Shds[0] = RndRef.ShdCreate("markers/sphere");
  Shds[1] = RndRef.ShdCreate("markers/cylinder");

  // Pipeline layout setup
  std::vector<VkDescriptorSetLayout> DescriptorSetLayouts
  {
    RndRef.VulkanCore.DescriptorSetLayout,
  };
  std::vector<VkPushConstantRange> PushConstantRanges
  {
    {
      .stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS,
      .offset = 0,
      .size = 256, // 256
    }
  };
  VkPipelineLayoutCreateInfo PipelineLayoutCreateInfo
  {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    .setLayoutCount = static_cast<UINT>(DescriptorSetLayouts.size()),
    .pSetLayouts = DescriptorSetLayouts.data(),
    .pushConstantRangeCount = static_cast<UINT>(PushConstantRanges.size()),
    .pPushConstantRanges = PushConstantRanges.data(),
  };
  vkCreatePipelineLayout(RndRef.VulkanCore.Device, &PipelineLayoutCreateInfo, nullptr, &PipelineLayouts[0]);
  PipelineLayouts[1] = PipelineLayouts[0];

  VertexBuffer = RndRef.VulkanCore.BufCreate(0, 0, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
} /* End of 'pivk::marker_manager::Init' function */

/* Markers deinitialization funtion.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID pivk::marker_manager::Close( VOID )
{
  //if (VertexBuffer != VK_NULL_HANDLE)
  //  RndRef.VulkanCore.BufFree(VertexBuffer, VertexBufferAllocation);
  VertexBuffer = nullptr;
  ///VertexBufferAllocation = VK_NULL_HANDLE;
} /* End of 'vigl::marker_manager::Close' function */

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
    //if (Shds[i]->IsReload)
    //{
    //  Shds[i]->IsReload = FALSE;
    //  if (Pipelines[i] != VK_NULL_HANDLE)
    //    vkDestroyPipeline(RndRef.Vlk.Device, Pipelines[i], nullptr);
    //  Pipelines[i] = VK_NULL_HANDLE;
    //}

    if (Pipelines[i] == VK_NULL_HANDLE)
    {
      /////////////////////
      // Shaders setup
      /////////////////////

      // Vertex stride parameter
      std::vector<VkVertexInputBindingDescription> VertexInputBindingDescriptions
      {
        {
          .binding = 0,
          .stride = 0,
          .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
        }
      };

      // Vertex format
      std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescriptions;
      VkPipelineVertexInputStateCreateInfo PipelineVertexInputStateCreateInfo
      {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount = static_cast<UINT>(VertexInputBindingDescriptions.size()),
        .pVertexBindingDescriptions = VertexInputBindingDescriptions.data(),
        .vertexAttributeDescriptionCount = static_cast<UINT>(VertexInputAttributeDescriptions.size()),
        .pVertexAttributeDescriptions = VertexInputAttributeDescriptions.data(),
      };

      // Shaders
      std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageCreateInfo;
      if (Shds[i]->ShaderModuleVert != VK_NULL_HANDLE)
        PipelineShaderStageCreateInfo.push_back(
          {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_VERTEX_BIT,
            .module = Shds[i]->ShaderModuleVert,
            .pName = "main",
          });
      if (Shds[i]->ShaderModuleGeom != VK_NULL_HANDLE)
        PipelineShaderStageCreateInfo.push_back(
          {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_GEOMETRY_BIT,
            .module = Shds[i]->ShaderModuleGeom,
            .pName = "main",
          });
      if (Shds[i]->ShaderModuleCtrl != VK_NULL_HANDLE)
        PipelineShaderStageCreateInfo.push_back(
          {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
            .module = Shds[i]->ShaderModuleCtrl,
            .pName = "main",
          });
      if (Shds[i]->ShaderModuleEval != VK_NULL_HANDLE)
        PipelineShaderStageCreateInfo.push_back(
          {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
            .module = Shds[i]->ShaderModuleEval,
            .pName = "main",
          });
      if (Shds[i]->ShaderModuleFrag != VK_NULL_HANDLE)
        PipelineShaderStageCreateInfo.push_back(
          {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = Shds[i]->ShaderModuleFrag,
            .pName = "main",
          });

      ////////////////////
      // Draw parameters
      ////////////////////

      // Multisampling settings
      VkPipelineMultisampleStateCreateInfo PipelineMultisampleStateCreateInfo
      {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable = VK_FALSE,
      };

      // Depth and stencil setup
      VkPipelineDepthStencilStateCreateInfo PipelineDepthStencilStateCreateInfo
      {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .depthTestEnable = VK_TRUE,
        .depthWriteEnable = VK_TRUE,
        .depthCompareOp = VK_COMPARE_OP_LESS,
        .depthBoundsTestEnable = VK_FALSE,
        .stencilTestEnable = VK_FALSE,
      };

      // Rasterization setup
      VkPipelineRasterizationStateCreateInfo PipelineRasterizationStateCreateInfo
      {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = VK_POLYGON_MODE_LINE,
        .cullMode = VK_CULL_MODE_NONE,
        .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
        .depthBiasEnable = VK_FALSE,
        .depthBiasConstantFactor = 1.0,
        .depthBiasClamp = 0.0,
        .depthBiasSlopeFactor = 0.0,
        .lineWidth = 1.0f,
      };

      // Set blend color components and enable status
      VkPipelineColorBlendAttachmentState PipelineColorBlendAttachmentState
      {
        .blendEnable = VK_FALSE,
        .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
        .colorBlendOp = VK_BLEND_OP_ADD,
        .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        .alphaBlendOp = VK_BLEND_OP_ADD,
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
      };

      // Set blending parameters
      VkPipelineColorBlendStateCreateInfo PipelineColorBlendStateCreateInfo
      {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .logicOpEnable = VK_FALSE,
        .logicOp = VK_LOGIC_OP_COPY,
        .attachmentCount = 1,
        .pAttachments = &PipelineColorBlendAttachmentState,
        .blendConstants {0, 0, 0, 0},
      };

      // Pipeline geometry type (assembling)
      VkPipelineInputAssemblyStateCreateInfo PipelineInputAssemblyStateCreateInfo
      {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST, // VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE,
      };

      // Tessellation parameters
      VkPipelineTessellationDomainOriginStateCreateInfo PipelineTessellationDomainOriginStateCreateInfo
      {
        .sType =  VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_DOMAIN_ORIGIN_STATE_CREATE_INFO,
        .domainOrigin = VK_TESSELLATION_DOMAIN_ORIGIN_UPPER_LEFT,
      };
      VkPipelineTessellationStateCreateInfo PipelineTessellationStateCreateInfo
      {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
        .pNext = &PipelineTessellationDomainOriginStateCreateInfo,
        .patchControlPoints = 1,
      };

      // Dynamic state setup
      std::vector<VkDynamicState> DynamicStates
      {
        VK_DYNAMIC_STATE_POLYGON_MODE_EXT,
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
      };
      VkPipelineDynamicStateCreateInfo PipelineDynamicStateCreateInfo
      {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = static_cast<UINT>(DynamicStates.size()),
        .pDynamicStates = DynamicStates.data(),
      };

      // Setup viewport state
      VkPipelineViewportStateCreateInfo ViewportStateCreateInfo = 
      {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .viewportCount = 1,
        .pViewports = &RndRef.VulkanCore.Viewport,
        .scissorCount = 1,
        .pScissors = &RndRef.VulkanCore.Scissor,
      };

      // Pipeline create struct filling
      VkGraphicsPipelineCreateInfo GraphicsPipelineCreateInfo
      {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = static_cast<UINT>(PipelineShaderStageCreateInfo.size()),
        .pStages = PipelineShaderStageCreateInfo.data(),
        .pVertexInputState = &PipelineVertexInputStateCreateInfo,
        .pInputAssemblyState = &PipelineInputAssemblyStateCreateInfo,
        .pTessellationState = &PipelineTessellationStateCreateInfo,
        .pViewportState = &ViewportStateCreateInfo,
        .pRasterizationState = &PipelineRasterizationStateCreateInfo,
        .pMultisampleState = &PipelineMultisampleStateCreateInfo,
        .pDepthStencilState = &PipelineDepthStencilStateCreateInfo,
        .pColorBlendState = &PipelineColorBlendStateCreateInfo,
        .pDynamicState = &PipelineDynamicStateCreateInfo,
        .layout = PipelineLayouts[i],
        .renderPass = RndRef.VulkanCore.RenderPass,
        .basePipelineHandle = VK_NULL_HANDLE,
        .basePipelineIndex = -1,
      };

      if (Shds[i]->ShaderModuleVert != VK_NULL_HANDLE && Shds[i]->ShaderModuleFrag != VK_NULL_HANDLE)
      {
        if (vkCreateGraphicsPipelines(RndRef.VulkanCore.Device, VK_NULL_HANDLE, 1, &GraphicsPipelineCreateInfo, nullptr, &Pipelines[i]) != VK_SUCCESS)
          throw "Error test pipeline creation";
      }
      else
        Pipelines[i] = VK_NULL_HANDLE;
    }
  }

  // Draw markers:
  static PFN_vkCmdSetPolygonModeEXT vkCmdSetPolygonModeEXT = reinterpret_cast<PFN_vkCmdSetPolygonModeEXT>(vkGetDeviceProcAddr(RndRef.VulkanCore.Device, "vkCmdSetPolygonModeEXT"));
  // reinterpret_cast<PFN_vkCmdSetPolygonModeEXT>(vkGetInstanceProcAddr(RndRef.Vlk.Instance, "vkCmdSetPolygonModeEXT"));

  if (vkCmdSetPolygonModeEXT != nullptr)
    if ((GetAsyncKeyState('W') & 0x8000) != 0 && (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0)
      vkCmdSetPolygonModeEXT(RndRef.VulkanCore.CommandBuffer, VK_POLYGON_MODE_LINE);
    else
      vkCmdSetPolygonModeEXT(RndRef.VulkanCore.CommandBuffer, VK_POLYGON_MODE_FILL);

  try
  {
    //if (Pipelines[0] != VK_NULL_HANDLE)
    //{
    //  vkCmdBindDescriptorSets(RndRef.VulkanCore.CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, PipelineLayouts[0],
    //                          0, 1, &RndRef.VulkanCore.DescriptorSet, 0, nullptr);
    //  vkCmdBindPipeline(RndRef.Vlk.CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Pipelines[0]);
    //
    //  struct  
    //  {
    //    matr::matr_data WVP, W, InvW;
    //    vec4 CR, Color;
    //  } bp {wvp, w, invw};
    //
    //  for (auto &sph : Spheres)
    //  {
    //    bp.CR = std::get<0>(sph);
    //    bp.Color = std::get<1>(sph);
    //    VkBuffer VertexBuffers[] = {VertexBuffer};
    //    VkDeviceSize Offsets[] = {0};
    //    vkCmdPushConstants(RndRef.Vlk.CommandBuffer, PipelineLayouts[0], VK_SHADER_STAGE_ALL_GRAPHICS/*VK_SHADER_STAGE_ALL*/, 0, sizeof(bp), &bp);
    //    vkCmdBindVertexBuffers(RndRef.Vlk.CommandBuffer, 0, 1, VertexBuffers, Offsets);
    //    vkCmdDraw(RndRef.Vlk.CommandBuffer, 1, 1, 0, 0);
    //  }
    //}
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
    //if (Pipelines[1] != VK_NULL_HANDLE)
    //{
    //  vkCmdBindDescriptorSets(RndRef.Vlk.CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, PipelineLayouts[1],
    //                          0, 1, &RndRef.Vlk.FrameDescriptorSet, 0, nullptr);
    //  vkCmdBindPipeline(RndRef.Vlk.CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Pipelines[1]);
    //  struct  
    //  {
    //    matr::matr_data WVP, W, InvW;
    //    vec4 PR0, PR1, Color;
    //  } bp {wvp, w, invw};
    //
    //  for (auto &cyl : Cylinders)
    //  {
    //    bp.PR0 = std::get<0>(cyl);
    //    bp.PR1 = std::get<1>(cyl);
    //    bp.Color = std::get<2>(cyl);
    //    VkBuffer VertexBuffers[] = {VertexBuffer};
    //    VkDeviceSize Offsets[] = {0};
    //    vkCmdPushConstants(RndRef.Vlk.CommandBuffer, PipelineLayouts[0], VK_SHADER_STAGE_ALL_GRAPHICS/*VK_SHADER_STAGE_ALL*/, 0, sizeof(bp), &bp);
    //    vkCmdBindVertexBuffers(RndRef.Vlk.CommandBuffer, 0, 1, VertexBuffers, Offsets);
    //    vkCmdDraw(RndRef.Vlk.CommandBuffer, 1, 1, 0, 0);
    //  }
    //}
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
