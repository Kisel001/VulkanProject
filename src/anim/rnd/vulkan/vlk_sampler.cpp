/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        vlk_sampler.cpp
 * PROGRAMMER:  IP5
 * LAST UPDATE: 14.11.2024
 * PURPOSE:     Vulkan sampler realization file.
 * NOTE:        None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#include "pivk.h"

/* Base project namespace */
namespace pivk
{
  /* Create vulkan image sampler function.
   * ARGUMENTS:
   *   - vulkan image sampler:
   *       VkSampler &Sampler;
   *   - vulkan image view:
   *       VkImageView ImageView;
   * RETURNS: None.
   */
  VOID vlk::SamplerCreate( VkSampler &Sampler, VkImageView ImageView )
  {
    VkSamplerCreateInfo SamplerInfo {};
    SamplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    SamplerInfo.magFilter = VK_FILTER_LINEAR;
    SamplerInfo.minFilter = VK_FILTER_LINEAR;
    SamplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    SamplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    SamplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    SamplerInfo.anisotropyEnable = VK_TRUE;

    VkPhysicalDeviceProperties properties {};
    vkGetPhysicalDeviceProperties(PhysDevice, &properties);

    SamplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    SamplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    SamplerInfo.unnormalizedCoordinates = VK_FALSE;
    SamplerInfo.compareEnable = VK_FALSE;
    SamplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    SamplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    SamplerInfo.mipLodBias = 0.0f;
    SamplerInfo.minLod = 0.0f;
    SamplerInfo.maxLod = 0.0f;

    if (vkCreateSampler(Device, &SamplerInfo, nullptr, &Sampler) != VK_SUCCESS)
    {
      throw;// std::runtime_error("failed to create texture sampler!");
    }


  } /* END OF 'vlk::CreateSampler' function */

  /* Delete vulkan image sampler function.
   * ARGUMENTS:
   *   - vulkan image sampler:
   *       VkSampler &Sampler;
   * RETURNS: None.
   */
  VOID vlk::SamplerFree( VkSampler &Sampler )
  {
    assert(Sampler != nullptr);

    vkDestroySampler(Device, Sampler, nullptr);
    Sampler = VK_NULL_HANDLE;
  } /* END OF 'vlk::DestroySampler' function */

} /* end of 'pivk' namespace */

/* END OF 'vlk_sampler.cpp' FILE */
