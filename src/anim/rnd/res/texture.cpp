/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        texture.h
 * PROGRAMMER:  IP5
 * LAST UPDATE: 03.09.2024
 * PURPOSE:     Texture functions declare file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#include "pivk.h"

 /* Base project namespace */
namespace pivk
{
  /* Texture add from image function.
   * ARGUMENTS:
   *   - texture size in pixels:
   *       INT W, INT H;
   *   - number of components per point:
   *       INT C;
   *   - pointer to an array with components:
   *       VOID *Bits;
   *   - render:
   *       render *NewRnd;
   * RETURNS:
   *   (texture &) result texture.
   */
  texture & texture::TexAddImg( INT W, INT H, INT C, const VOID *Bits, render *NewRnd )
  {
    if (NewRnd != nullptr)
      Rnd = NewRnd;
    assert(Rnd != nullptr);

    TexW = W;
    TexH = H;

    INT mips;

    mips = log(W > H ? W : H) / log(2);
    mips = mips < 1 ? 1 : mips;

    VkFormat Format;

    // Set format by 'C'
    switch (C)
    {
    // One color
    case 1:
      Format = VK_FORMAT_R8_UINT;
      break;

    // RGB
    case 3:
      throw;
      Format = VK_FORMAT_R8G8B8_SRGB;
      break;

    // RGBA
    case 4:
      Format = VK_FORMAT_B8G8R8A8_SRGB;
      break;

    // Error type
    default:
      throw;
      break;
    }

    // Image usage flags:
    VkImageUsageFlags UsageFlags = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | // Used as source on shader
                                   VK_IMAGE_USAGE_SAMPLED_BIT;       // Used for image_view for sampler

    Rnd->VulkanCore.ImageCreate(Image, ImageView, Allocation,
                                W, H, VK_TRUE, VK_FALSE, Format, UsageFlags,
                                VK_IMAGE_LAYOUT_UNDEFINED, Bits, W * H * C, mips);
    Rnd->VulkanCore.SamplerCreate(Sampler, ImageView);
    
    VkDescriptorSetLayout DSL = Rnd->VulkanCore.TexDescriptorSetLayout;

    std::array<VkDescriptorSetLayout, 9> DSLA 
    {
      DSL, DSL, DSL,
      DSL, DSL, DSL,
      DSL, DSL, DSL,
    };

    VkDescriptorSetAllocateInfo AllocInfo {};
    AllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    AllocInfo.descriptorPool = Rnd->VulkanCore.DescriptorPool;
    AllocInfo.descriptorSetCount = 9;
    AllocInfo.pSetLayouts = DSLA.data();

    if (VkResult res; (res = vkAllocateDescriptorSets(Rnd->VulkanCore.Device, &AllocInfo, TextureDescriptorSets)) != VK_SUCCESS) 
    {
      std::cout << res;
      throw; // std::runtime_error("failed to allocate descriptor sets!");
    }

    for (INT i = 0; i < 9; i++)
      Rnd->VulkanCore.TexUpdateDescriptor(TextureDescriptorSets[i], ImageView, Sampler, i);

    return *this;
  } /* End of 'texture::TexAddImg' function */

  /* Texture cube map add function.
   * ARGUMENTS:
   *   - render:
   *       render *NewRnd;
   * RETURNS:
   *   (texture &) result texture.
   */
  texture & texture::TexAddCubeMap( render *NewRnd )
  {
    // Temporary not working
    throw;

    if (NewRnd != nullptr)
      Rnd = NewRnd;
    assert(Rnd != nullptr);

    INT res = -1;
    HBITMAP hBm;
    INT mips;
    std::string Names[] = {"Posx", "Negx", "Posy", "Negy", "Posz", "Negz"};


    std::string FileName = Rnd->AnimRef.Path + "\\" + Name;

    //IsCubeMap = TRUE;

    for (INT i = 0; i < 6; i++)
    {
      if ((hBm = (HBITMAP)LoadImage(nullptr, (FileName + "\\" + Names[i] + ".bmp").c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION)) != NULL)
      {
        BITMAP bm {};

        GetObject(hBm, sizeof(bm), &bm);
        mips = log(bm.bmWidth > bm.bmHeight ? bm.bmWidth : bm.bmHeight) / log(2);
        mips = mips < 1 ? 1 : mips;
        INT C = bm.bmBitsPixel / 8;
        DeleteObject(hBm);
      }
    }
    return *this;
  } /* End of 'texture::TexAddCubeMap' function */

  /* Texture add from file function.
   * ARGUMENTS: None.
   * RETURNS:
   *   (texture &) result texture.
   */
  texture & texture::TexAddFile( render *NewRnd )
  {
    if (NewRnd != nullptr)
      Rnd = NewRnd;
    assert(Rnd != nullptr);

    INT res = -1;
    HBITMAP hBm;

    std::string FileName = Rnd->AnimRef.Path + "/bin/textures/" + Name;

    if ((hBm = (HBITMAP)LoadImage(nullptr, FileName.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION)) != nullptr)
    {
      BITMAP bm {};

      GetObject(hBm, sizeof(bm), &bm);
      TexAddImg(bm.bmWidth, bm.bmHeight, bm.bmBitsPixel / 8, bm.bmBits);

      DeleteObject(hBm);
    }
    
    return *this;
  } /* End of 'texture::TexAddFile' function */

  /* Texture free function.
   * ARGUMENTS: None.
   * RETURNS:
   *   - (texture &) this texture.
   */
  texture & texture::TexFree( VOID )
  {
    Rnd->VulkanCore.ImageFree(Image, ImageView, Allocation);
    return *this;
  } /* End of 'texture::TexFree' function */

  /* Texture apply function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID texture::TexApply( INT BindPoint )
  {
    vkCmdBindDescriptorSets(Rnd->VulkanCore.CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Rnd->VulkanCore.PipelineLayout, 1, 1, &TextureDescriptorSets[BindPoint], 0, nullptr);
  } /* End of 'texture::TexApply' function */

  /* Update texture array function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID texture_manager::TexUpdateArray( VOID )
  {
  } /* End of 'TexUpdateArray' function */

} /* end of 'pivk' namespace */

/* END OF 'texture.cpp' FILE */
