/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        vlk_descriptor.cpp
 * PROGRAMMER:  IP5
 * LAST UPDATE: 30.10.2024
 * PURPOSE:     Vulkan descriptor file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#include "pivk.h"

/* Base project namespace */
namespace pivk
{
  /* Create descriptors function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID vlk::CreateDescriptors( VOID )
  {
    /* Declare descriptor layouts bindings */
    // 0 - camera buffer          | UBO
    // 1 - synchronization buffer | UBO
    // 2 - primitives buffer      | SSBO (temp one prim, UBO)
    // 3 - materials buffer       | SSBO (temp one mtl, UBO)
    // 4 - texture buffer 

    VkDescriptorSetLayoutBinding DescriptorSetLayoutBindings[]
    {
      /* Camera Buffer */
      {
        .binding = 0,                                        // Number of binding on shader
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, // Object type.
        .descriptorCount = 1,                                // Count in array (may be binding
                                                             // some objects at once).
                                                             // For VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK -
                                                             // his size in bytes.
        .stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS,          // Flag, indication, on which shaders this resource
                                                             // will be avaliable. Also avaliable uniting
                                                             // some flags stages.
        .pImmutableSamplers = nullptr,                       // Pointer to immutable samplers.
      },
      /* Synchronization buffer */
      {
        .binding = 1,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS,
        .pImmutableSamplers = nullptr,
      },
      /* Primitives buffer */
      {
        .binding = 2,
        .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS,
        .pImmutableSamplers = nullptr,
      },
      /* Materials buffer */
      {
        .binding = 3,
        .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS,
        .pImmutableSamplers = nullptr,
      },
    };
  
    VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutCreateInfo
    {
      .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,   // Structure type.
      .flags = 0,                                                     // Flags.
      .bindingCount = (UINT32)std::size(DescriptorSetLayoutBindings), // Count of bindings.
      .pBindings = DescriptorSetLayoutBindings,                       // Pointer to first.
    };
  
    if (vkCreateDescriptorSetLayout(Device,                              // Logical device.
                                    &DescriptorSetLayoutCreateInfo,      // Create information.
                                    nullptr,                             // Memory allocator.
                                    &DescriptorSetLayout) != VK_SUCCESS) // Pointer to variable VkDescriptorSetLayout.
    {
      throw;
    }


    VkDescriptorSetLayoutBinding TexDescriptorSetLayoutBindings[]
    {
      /* Base texture binding */
      {
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS,
        .pImmutableSamplers = nullptr,
      },
      {
        .binding = 1,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS,
        .pImmutableSamplers = nullptr,
      },
      {
        .binding = 2,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS,
        .pImmutableSamplers = nullptr,
      },
      {
        .binding = 3,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS,
        .pImmutableSamplers = nullptr,
      },
      {
        .binding = 4,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS,
        .pImmutableSamplers = nullptr,
      },
      {
        .binding = 5,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS,
        .pImmutableSamplers = nullptr,
      },
      {
        .binding = 6,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS,
        .pImmutableSamplers = nullptr,
      },
      {
        .binding = 7,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS,
        .pImmutableSamplers = nullptr,
      },
      {
        .binding = 8,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS,
        .pImmutableSamplers = nullptr,
      },
    };

    VkDescriptorSetLayoutCreateInfo TexDescriptorSetLayoutCreateInfo
    {
      .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,      // Structure type.
      .flags = 0,                                                        // Flags.
      .bindingCount = (UINT32)std::size(TexDescriptorSetLayoutBindings), // Count of bindings.
      .pBindings = TexDescriptorSetLayoutBindings,                       // Pointer to first.
    };

    if (vkCreateDescriptorSetLayout(Device,
                                    &TexDescriptorSetLayoutCreateInfo,
                                    nullptr,
                                    &TexDescriptorSetLayout) != VK_SUCCESS) 
    {
      throw;
    }

    /* Create descriptor pool */
    std::array<VkDescriptorPoolSize, 5> PoolSizes {};
  
    PoolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;         // Descriptor type.
    PoolSizes[0].descriptorCount = 1;                              // Count of descriptor sizes.
    PoolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;         // Descriptor type.
    PoolSizes[1].descriptorCount = 1;                              // Count of descriptor sizes.
    PoolSizes[2].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;         // Descriptor type.
    PoolSizes[2].descriptorCount = 1;                              // Count of descriptor sizes.
    PoolSizes[3].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;         // Descriptor type.
    PoolSizes[3].descriptorCount = 1;                              // Count of descriptor sizes.
    PoolSizes[4].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; // Descriptor type.
    PoolSizes[4].descriptorCount = 10000;                          // Count of descriptor sizes.

    VkDescriptorPoolCreateInfo PoolCreateInfo
    {
      .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
      .maxSets = 20000,                        // Maximal count descriptor sets, which may be created.
      .poolSizeCount = (UINT)PoolSizes.size(), // Count of descriptor sizes.
      .pPoolSizes = PoolSizes.data(),          // Descriptor sizes.
    };

    if (vkCreateDescriptorPool(Device, &PoolCreateInfo, nullptr, &DescriptorPool) != VK_SUCCESS)
    {
      throw;
    }
  } /* End of 'vlk::CreateDescriptors' function */

  /* Create vulkan descriptor function.
   * ARGUMENTS:
   *   - descriptor set handle:
   *       VkDescriptorSet NDS;
   * RETURNS:
   *   (vlk_descriptor &) reference to this descriptor.
   */
  vlk_descriptor & vlk_descriptor::Create( VkDescriptorSet NDS )
  {
    this->DescriptorSet = NDS;
    return *this;
  } /* End of 'vlk_descriptor::Create' function */

  /* Delete vulkan descriptor function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID vlk_descriptor::Free( VOID )
  {
  } /* End of 'vlk_descriptor::Free' function */

  /* Bind descriptor function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID vlk_descriptor::Bind( VOID )
  {
    vkCmdBindDescriptorSets(VlkCore->CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VlkCore->PipelineLayout, 0, 1, &DescriptorSet, 0, nullptr);
  } /* End of 'vlk_descriptor::Bind' function */

  /* Update vulkan descriptor function.
   * ARGUMENTS:
   *   - buffer:
   *       vlk_buf *Buffer;
   *   - binding id:
   *       UINT BindPoint;
   *   - size of buffer (in bytes):
   *       UINT_PTR BufSize;
   *   - offset of buffer:
   *       UINT_PTR BufOffset;
   *   - type of buffer: (0 - UBO, 1 - SSBO)
   *       INT Type;
   * RETURNS: None.
   */
  VOID vlk_descriptor::Update( vlk_buf *Buffer, UINT BindPoint, UINT_PTR BufSize, UINT_PTR BufOffset, INT Type )
  {
    //std::vector<VkDescriptorBufferInfo> BuffersInfo;
    //BuffersInfo.resize(Buffer->CountOfElements);
    //
    //UINT_PTR OneSize = (BufSize == 0 ? Buffer->Size : BufSize) / Buffer->CountOfElements;
    //UINT_PTR CurOffset = 0;
    //
    //for (auto &i : BuffersInfo)
    //{
    //  i.buffer = Buffer->Buffer;
    //  i.offset = CurOffset;
    //  i.range = OneSize;
    //  CurOffset += OneSize;
    //}

    // For filling information about buffer using VkDescriptorBufferInfo.
    VkDescriptorBufferInfo BufferInfo
    {
      .buffer = Buffer->Buffer,                       // Buffer, which write to this descriptor.
      .offset = BufOffset,                            // Offset in bytes.
      .range = BufSize == 0 ? Buffer->Size : BufSize, // Buffer size.
    };

    std::array<VkWriteDescriptorSet, 1> DescriptorWrites {};
    
    DescriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;     // Structure type
    DescriptorWrites[0].dstSet = DescriptorSet;                             // Set, which need to update.
    DescriptorWrites[0].dstBinding = BindPoint;                             // Descriptor's binding.
    DescriptorWrites[0].dstArrayElement = 0;                                // Zero element in array.
    DescriptorWrites[0].descriptorType = Type == 0 ?                        // Descriptor type:
      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER :                                   // 0 - UBO
      VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;                                    // 1 - SSBO
    DescriptorWrites[0].descriptorCount = 1;                                // Count of descriptors for update.
    DescriptorWrites[0].pBufferInfo = &BufferInfo;                          // Pointer to array buffer info.
    
    vkUpdateDescriptorSets(VlkCore->Device,
                           (UINT)DescriptorWrites.size(), // Count of descriptor write structures.
                           DescriptorWrites.data(),       // Self descriptor write.
                           0, nullptr);
  } /* End of 'vlk_descriptor::Update' function */

  /* Update vulkan texture descriptor function.
   * ARGUMENTS:
   *   - vulkan image view:
   *       VkImageView ImageView;
   *   - vulkan image sampler:
   *       VkSampler Sampler;
   *   - bind point:
   *       UINT BindPoint;
   * RETURNS: None.
   */
  VOID vlk_descriptor::TexUpdate( VkImageView ImageView, VkSampler Sampler, UINT BindPoint )
  {
    VkDescriptorImageInfo ImageInfo {};

    ImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    ImageInfo.imageView = ImageView;
    ImageInfo.sampler = Sampler;

    std::array<VkWriteDescriptorSet, 1> DescriptorWrites {};

    DescriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    DescriptorWrites[0].dstSet = DescriptorSet;                                     // Set, which need to update.
    DescriptorWrites[0].dstBinding = BindPoint;                                     // Descriptor's binding.
    DescriptorWrites[0].dstArrayElement = 0;                                        // Zero element in array.
    DescriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; // Descriptor type - we have image sampler.
    DescriptorWrites[0].descriptorCount = 1;                                        // Count of descriptors for update.
    DescriptorWrites[0].pImageInfo = &ImageInfo;                                    // Pointer to array image info.

    vkUpdateDescriptorSets(VlkCore->Device,
                           (UINT32)DescriptorWrites.size(), // Coutn of descriptor write structures.
                           DescriptorWrites.data(),       // Self descriptor write.
                           0, nullptr);
  } /* End of 'vlk_descriptor::TexUpdate' function */

  /* Update vulkan texture descriptor function.
   * ARGUMENTS:
   *   - vulkan descriptor set:
   *       VkDescriptorSet DescriptorSet;
   *   - vulkan image view:
   *       VkImageView ImageView;
   *   - vulkan image sampler:
   *       VkSampler Sampler;
   *   - bind point:
   *       UINT BindPoint;
   * RETURNS: None.
   */
  VOID vlk_descriptor_manager::TexUpdateDescriptor( VkDescriptorSet DescriptorSet, VkImageView ImageView, VkSampler Sampler, UINT BindPoint )
  {
    VkDescriptorImageInfo ImageInfo {};

    ImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    ImageInfo.imageView = ImageView;
    ImageInfo.sampler = Sampler;

    std::array<VkWriteDescriptorSet, 1> DescriptorWrites {};

    DescriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    DescriptorWrites[0].dstSet = DescriptorSet;                                     // Set, which need to update.
    DescriptorWrites[0].dstBinding = BindPoint;                                     // Descriptor's binding.
    DescriptorWrites[0].dstArrayElement = 0;                                        // Zero element in array.
    DescriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; // Descriptor type - we have image sampler.
    DescriptorWrites[0].descriptorCount = 1;                                        // Count of descriptors for update.
    DescriptorWrites[0].pImageInfo = &ImageInfo;                                    // Pointer to array image info.

    vkUpdateDescriptorSets(VlkCore.Device,
                           (UINT32)DescriptorWrites.size(), // Count of descriptor write structures.
                           DescriptorWrites.data(),         // Self descriptor write.
                           0, nullptr);
  } /* End of 'vlk_descriptor_manager::TexUpdateDescriptor' function */

  /* Init descriptors function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID vlk_descriptor_manager::CreateDescriptors( VOID )
  {
    UINT Count = 1;// VlkCore.CountOfDescriptorSets;
    std::vector<VkDescriptorSetLayout> layouts(1, VlkCore.DescriptorSetLayout);
    VkDescriptorSetAllocateInfo AllocInfo {};
    AllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    AllocInfo.descriptorPool = VlkCore.DescriptorPool;
    AllocInfo.descriptorSetCount = Count;
    AllocInfo.pSetLayouts = layouts.data();

    //VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> DescriptorSets;
    DescriptorSets.resize(Count);

    if (VkResult res; (res = vkAllocateDescriptorSets(VlkCore.Device, &AllocInfo, DescriptorSets.data())) != VK_SUCCESS) 
    {
      std::cout << res;
      throw; // std::runtime_error("failed to allocate descriptor sets!");
    }

    for (INT i = 0; i < 1; ++i)
      this->Stock[StockSize++] = vlk_descriptor(&VlkCore).Create(DescriptorSets[i]);
  } /* End of 'vlk_descriptor_manager::CreateDescriptors' function */

  /* Update vulkan descriptor function.
   * ARGUMENTS:
   *   - buffer:
   *       vlk_buf *Buffer;
   *   - bind point:
   *       INT BindPoint;
   *   - type of buffer:
   *       INT Type;
   *   - size of buffer (in bytes):
   *       UINT_PTR BufSize;
   *   - offset of buffer:
   *       UINT_PTR BufOffset;
   * RETURNS: None.
   */
  VOID vlk_descriptor_manager::UpdateDescriptor( vlk_buf *Buffer, INT BindPoint, INT Type, UINT_PTR BufSize, UINT_PTR BufOffset )
  {
    INT BP = 0;

    auto find = this->Stock.find(0);

    if (find == this->Stock.end())
      return;

    this->Stock[0].Update(Buffer, BindPoint, BufSize, BufOffset, Type);
  } /* End of 'vlk_descriptor_manager::UpdateDescriptor' function */

  /* Delete vulkan descriptors function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID vlk_descriptor_manager::FreeDescriptors( VOID )
  {
    for (auto &i : Stock)
    {
      i.second.Free();
      //i.second = nullptr;
    }

    Stock.clear();
    StockSize = 0;
  } /* End of 'vlk_descriptor_manager::FreeDescriptors' function */

  /* Bind descriptor function.
   * ARGUMENTS:
   *    - bind point:
   *        INT BindPoint;
   * RETURNS: None.
   */
  VOID vlk_descriptor_manager::BindDescriptor( INT BindPoint )
  {
    auto find = this->Stock.find(0);

    if (find == this->Stock.end())
    {
      return;
    }

    this->Stock[0].Bind();
    //this->Stock[BindPoint].Update(Buffer, BufSize, BufOffset);
  } /* End of 'vlk_descriptor_manager::BindDescriptor' function */

} /* End of 'pivk' namespace */

/* END OF 'vlk_descriptor.cpp' FILE */
