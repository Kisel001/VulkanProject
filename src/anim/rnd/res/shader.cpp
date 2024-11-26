/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        shader.cpp
 * PROGRAMMER:  IP5
 * LAST UPDATE: 27.10.2024
 * PURPOSE:     Shader class file.
 * NOTE:        None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#include "pivk.h"

#include <format>

/* Base project namespace */
namespace pivk
{
  /* Load binary SPIR-V file and move to buffer function.
   * ARGUMENTS:
   *   - filename:
   *       const std::string &Filename;
   *   - reference to shader data buffer:
   *       std::vector<BYTE> ShaderDataBuf;
   * RETURNS:
   *   (BOOL) status of loading shader data.
   */
  inline BOOL LoadSPIRVShader( const std::string &Filename, std::vector<BYTE> &ShaderDataBuf )
  {
    if (FILE *F; (F = fopen(Filename.c_str(), "rb")) != nullptr)
    {
      fseek(F, 0, SEEK_END);
      UINT_PTR Size = ftell(F);
      fseek(F, 0, SEEK_SET);
      ShaderDataBuf.resize(Size);
      fread(ShaderDataBuf.data(), 1, Size, F);
      fclose(F);
      return TRUE;
    }
    return FALSE;
  } /* End of 'LoadSPRIVShader' function */

  /* Load shader function.
   * ARGUMENTS: None.
   *   - pointer to new render:
   *       render *NewRnd;
   * RETURNS:
   *   (shader &) self reference.
   */
  shader & shader::Load(render *NewRnd)
  {
    if (NewRnd != nullptr)
      Rnd = NewRnd;
    assert(Rnd != nullptr);

    // TODO: Create GLSL shader compiler
    /* Step 1. Compile GLSL shader to SPIR-V */

    /* Step 2. Load SPIR-V shader */
    std::vector<BYTE> FragBuf;
    std::vector<BYTE> VertBuf;
    //std::vector<BYTE> GeomBuf;

    LoadSPIRVShader("bin/shaders/" + this->Name + "/frag.spv", FragBuf);
    LoadSPIRVShader("bin/shaders/" + this->Name + "/vert.spv", VertBuf);
    //LoadSPIRVShader("bin/shaders/" + this->Name + "/geom.spv", GeomBuf);

    // Byte code size multiple 4.
    assert(FragBuf.size() % 4 == 0);
    assert(VertBuf.size() % 4 == 0);
    //assert(GeomBuf.size() % 4 == 0);

    VkShaderModuleCreateInfo FragInfo
    {
      .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,      // Structure type.
      .codeSize = FragBuf.size(),                                // Byte code size.
      .pCode = reinterpret_cast<const UINT32 *>(FragBuf.data()), // Pointer to byte-code.
    };

    VkShaderModuleCreateInfo VertInfo
    {
      .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,      // Structure type.
      .codeSize = VertBuf.size(),                                // Byte code size.
      .pCode = reinterpret_cast<const UINT32 *>(VertBuf.data()), // Pointer to byte-code.
    };

    //VkShaderModuleCreateInfo GeomInfo
    //{
    //  .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,      // Structure type.
    //  .codeSize = GeomBuf.size(),                                // Byte code size.
    //  .pCode = reinterpret_cast<const UINT32 *>(GeomBuf.data()), // Pointer to byte-code.
    //};

    //ShaderModules.resize(2);
    //ShaderModules[0].second = FRAG;
    //ShaderModules[1].second = VERT;
    //ShaderModules[2].second = GEOM;

    // Create vulkan shader modules

    // FRAG
    if (vkCreateShaderModule(Rnd->VulkanCore.Device,                 // Vulkan core logical device.
                             &FragInfo,                              // Information about creating.
                             nullptr,                                // Allocator (optional).
                             &ShaderModules[FRAG]) != VK_SUCCESS)    // Pointer to creating VkShaderModule module.
    {
      throw;
    }

    // VERT
    if (vkCreateShaderModule(Rnd->VulkanCore.Device,                 // Vulkan core logical device.
                             &VertInfo,                              // Information about creating.
                             nullptr,                                // Allocator (optional).
                             &ShaderModules[VERT]) != VK_SUCCESS)    // Pointer to creating VkShaderModule module.
    {
      throw;
    }

    // GEOM
    //vkCreateShaderModule(Rnd->VulkanCore.Device,   // Vulkan core logical device.
    //                     &GeomInfo,                // Information about creating.
    //                     nullptr,                  // Allocator (optional).
    //                     &ShaderModules[2].first); // Pointer to creating VkShaderModule module.

    // Vert shader info
    VkPipelineShaderStageCreateInfo VertShaderStageInfo
    {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .stage = VK_SHADER_STAGE_VERTEX_BIT,
      .module = ShaderModules[VERT],
      .pName = "main",
    };
    // Frag shader info
    VkPipelineShaderStageCreateInfo FragShaderStageInfo
    {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
      .module = ShaderModules[FRAG],
      .pName = "main",
    };
    // Geom shader info
    //VkPipelineShaderStageCreateInfo GeomShaderStageInfo
    //{
    //  .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    //  .stage = VK_SHADER_STAGE_GEOMETRY_BIT,
    //  .module = ShaderModules[2].first,
    //  .pName = "main",
    //};

    // Shader stage info array
    std::array<VkPipelineShaderStageCreateInfo, 2> ShaderStageInfo =
    {
      VertShaderStageInfo, FragShaderStageInfo,
    };


    //uint32_t                                    vertexBindingDescriptionCount;
    //const VkVertexInputBindingDescription*      pVertexBindingDescriptions;
    //uint32_t                                    vertexAttributeDescriptionCount;
    //const VkVertexInputAttributeDescription*    pVertexAttributeDescriptions;

    std::array<VkVertexInputAttributeDescription, 3> AttributesDescription = {};
    VkVertexInputBindingDescription BindingDescription = 
    {
      .binding = 0,
      .stride = sizeof(FLOAT) * 8,
      .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
    };

    // Position
    AttributesDescription[0].binding = 0;
    AttributesDescription[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    AttributesDescription[0].location = 0;
    AttributesDescription[0].offset = 0;

    // TC
    AttributesDescription[1].binding = 0;
    AttributesDescription[1].format = VK_FORMAT_R32G32_SFLOAT;
    AttributesDescription[1].location = 1;
    AttributesDescription[1].offset = sizeof(FLOAT) * 3;

    // Normals
    AttributesDescription[2].binding = 0;
    AttributesDescription[2].format = VK_FORMAT_R32G32B32_SFLOAT;
    AttributesDescription[2].location = 2;
    AttributesDescription[2].offset = sizeof(FLOAT) * 3 + sizeof(FLOAT) * 2;

    // Setup vertex input
    VkPipelineVertexInputStateCreateInfo  VertexInputInfo = 
    {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
      .vertexBindingDescriptionCount = 1,
      .pVertexBindingDescriptions = &BindingDescription,
      .vertexAttributeDescriptionCount = UINT32(AttributesDescription.size()),
      .pVertexAttributeDescriptions = AttributesDescription.data(),
    };

    // Setup vertex assembly
    VkPipelineInputAssemblyStateCreateInfo InputAssembly = 
    {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      //.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
      .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
      .primitiveRestartEnable = VK_FALSE,
    };

    // Setup viewport state
    VkPipelineViewportStateCreateInfo ViewportState = 
    {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .viewportCount = 1,
      .pViewports = &Rnd->VulkanCore.Viewport,
      .scissorCount = 1,
      .pScissors = &Rnd->VulkanCore.Scissor,
    };

    // Setup rasterizer
    VkPipelineRasterizationStateCreateInfo Rasterizer = 
    {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .depthClampEnable = VK_FALSE,
      .rasterizerDiscardEnable = VK_FALSE,
      .polygonMode = VK_POLYGON_MODE_FILL,
      .cullMode = VK_CULL_MODE_NONE,//VK_CULL_MODE_BACK_BIT,
      .frontFace = VK_FRONT_FACE_CLOCKWISE,
      .depthBiasEnable = VK_FALSE,
      .depthBiasConstantFactor = 0.0f,
      .depthBiasClamp = 0.0f,
      .depthBiasSlopeFactor = 0.0f,
      .lineWidth = 1.0f,
    };

    // Setup multisampling
    VkPipelineMultisampleStateCreateInfo  Multisampling = 
    {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
      .sampleShadingEnable = VK_FALSE,
      .minSampleShading = 1.0f,
      .pSampleMask = nullptr,            // Optional
      .alphaToCoverageEnable = VK_FALSE, // Optional
      .alphaToOneEnable = VK_FALSE,      // Optional
    };

    // Setup color blending
    VkPipelineColorBlendAttachmentState  ColorBlendAttachment =
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

    // Setup color blend state
    VkPipelineColorBlendStateCreateInfo ColorBlending =
    {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .logicOpEnable = VK_FALSE,
      .logicOp = VK_LOGIC_OP_COPY,
      .attachmentCount = 1,
      .pAttachments = &ColorBlendAttachment,
      .blendConstants = {0.0f, 0.0f, 0.0f, 0.0f},
    };

    // Vulkan pipeline depth stencil state create info structure
    VkPipelineDepthStencilStateCreateInfo DepthStencil {};

    DepthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    DepthStencil.depthTestEnable = VK_TRUE;
    DepthStencil.depthWriteEnable = VK_TRUE;
    DepthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    DepthStencil.depthBoundsTestEnable = VK_FALSE;
    DepthStencil.minDepthBounds = 0.0f; // Optional
    DepthStencil.maxDepthBounds = 1.0f; // Optional
    DepthStencil.stencilTestEnable = VK_FALSE;
    DepthStencil.front = {}; // Optional
    DepthStencil.back = {}; // Optional

    // Graphics pipeline creare info
    VkGraphicsPipelineCreateInfo PipelineInfo =
    {
      .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .stageCount = 2,
      .pStages = ShaderStageInfo.data(),
      .pVertexInputState = &VertexInputInfo,
      .pInputAssemblyState = &InputAssembly,
      .pTessellationState = nullptr,
      .pViewportState = &ViewportState,
      .pRasterizationState = &Rasterizer,
      .pMultisampleState = &Multisampling,
      .pDepthStencilState = &DepthStencil,  // Optional
      .pColorBlendState = &ColorBlending,
      .pDynamicState = nullptr,
      .layout = Rnd->VulkanCore.PipelineLayout,
      .renderPass = Rnd->VulkanCore.RenderPass,
      .subpass = 0,
      .basePipelineHandle = nullptr,
      .basePipelineIndex = 0,
    };

    if (vkCreateGraphicsPipelines(Rnd->VulkanCore.Device, VK_NULL_HANDLE, 1, &PipelineInfo, nullptr, &Pipeline) != VK_SUCCESS )
    {
      throw;
    }

    return *this;
  } /* End of 'shader::Load' function */

  /* Apply shader function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID shader::Apply( VOID )
  {
    vkCmdBindPipeline(Rnd->VulkanCore.CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Pipeline);
  } /* End of 'shader::Apply' function */


  /* Text file load to memory function.
   * ARGUMENTS:
   *   - file name:
   *       const std::string &FileName;
   * RETURNS:
   *   (std::string) load text.
   */
  std::string shader::LoadTextFile( const std::string &FileName )
  {
    CHAR Buf[_MAX_PATH];
    GetCurrentDirectory(sizeof(Buf), Buf);
    std::string Bufstr = Buf;
    Bufstr += FileName;

    std::ifstream f(Bufstr);
 
    return std::string((std::istreambuf_iterator<char>(f)),
                        std::istreambuf_iterator<char>());
  } /* End of 'shader::LoadTextFile' function */

  /* Free shader function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID shader::Free( VOID )
  {
    //vkDestroyPipelineLayout(Rnd->VulkanCore.Device, PipelineLayout, nullptr);
    vkDestroyPipeline(Rnd->VulkanCore.Device, Pipeline, nullptr);

    for (auto &ShaderModule : ShaderModules)
      vkDestroyShaderModule(Rnd->VulkanCore.Device, ShaderModule.second, nullptr);
    ///vkDestroyShaderModule(Rnd->VulkanCore.Device, // Logical device
    ///                      ShaderModule,           // Deleting object.
    ///                      nullptr);               // Memory allocator.
  } /* End of 'shader::Free' function */


} /* end of 'pivk' namespace */

/* END OF 'shader.cpp' FILE */
