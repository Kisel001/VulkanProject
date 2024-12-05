/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        render.cpp
 * PROGRAMMER:  IP5
 * LAST UPDATE: 27.10.2024
 * PURPOSE:     Render functions declaring file.
 * NOTE:        None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#include "pivk.h"

/* Base project namespace */
namespace pivk
{
  /* Primitive draw function.
   * ARGUMENTS:
   *   - primitive pointer:
   *       prim *Pr;
   *   - transformation matrix:
   *       const matr &World;
   * RETURNS: None.
   */
  VOID render::Draw( const prim *Pr, const matr &World, const std::array<fvec4, 4> *SubData ) //const
  {
    matr w = Pr->Transform * World;
    DRAW_BUF Buf =
    {
      w, w * Camera.VP, w.Inverse().Transpose(), // Matrixes
      {Pr->Mtl->Id, Pr->Id, 0, 0},               // Subdata for primnitives and materials
      {{}, {}, {},},                         // Array with subdata
    };

    // Copy subdata to buf
    if (SubData != nullptr)
      memcpy(&Buf.MarkersData, SubData->data(), sizeof(DRAW_BUF::MarkersData));

    // Apply material
    Pr->Mtl->Apply();

    // Push constants
    vkCmdPushConstants(VulkanCore.CommandBuffer, VulkanCore.PipelineLayout, VK_SHADER_STAGE_ALL_GRAPHICS, 0, sizeof(DRAW_BUF), &Buf);

#if 0 // prim patches not used
    if (Pr->Type == IP5_RND_PRIM_PATCH)
      ;// glPatchParameteri(GL_PATCH_VERTICES, Pr->NumOfPatchPoints);
#endif

    if (1)
    {
      if (Pr->VBuf != nullptr)
      {
        UINT64 Offset = 0;
        vkCmdBindVertexBuffers(VulkanCore.CommandBuffer, 0, 1, &Pr->VBuf->Buffer, &Offset);
        if (Pr->IBuf != nullptr)
        {
          vkCmdBindIndexBuffer(VulkanCore.CommandBuffer, Pr->IBuf->Buffer, 0, VK_INDEX_TYPE_UINT32);
          vkCmdDrawIndexed(VulkanCore.CommandBuffer, static_cast<UINT32>(Pr->NumOfElements), 1, 0, 0, 0);
        }
        else
          vkCmdDraw(VulkanCore.CommandBuffer, Pr->NumOfElements, 1, 0, 0);
      }
    }
  } /* End of 'render::Draw' function */
} /* end of 'pivk' namespace */

/* END OF 'render.cpp' FILE */
