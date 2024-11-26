/*  */
#include "pivk.h"

/* Base project namespace */ 
namespace pivk
{
  /* Init primitive manager function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID primitive_manager::PrimInit( VOID )
  {
    PrimitiveSSBO = RndRef.VulkanCore.BufCreate(0, 0, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
  } /* End of 'primtiive_manager::PrimInit' function */

  /* Primitive update storage buffer function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID primitive_manager::PrimUpdateSSBO( VOID )
  {
    if (UpdatePrimitiveSSBO == FALSE)
      return;

    UpdatePrimitiveSSBO = FALSE;
    PrimitiveSSBO->Resize(resource_manager::TotalRefCnt * sizeof(material::BUF_MTL), resource_manager::TotalRefCnt);

    BYTE *MapPointer;

    if (vmaMapMemory(RndRef.VulkanCore.Allocator, PrimitiveSSBO->Allocation, reinterpret_cast<VOID **>(&MapPointer)) != VK_SUCCESS)
    {
      throw;
    }

    material::BUF_MTL Buf;
    INT count = 0;

    for (auto &i : Stock)
    {
      i.second.Id = count++;
      memcpy(MapPointer, &i.second.PrimitiveData, sizeof(prim::PRIM_BUF));
      MapPointer += sizeof(prim::PRIM_BUF);
    }

    vmaUnmapMemory(RndRef.VulkanCore.Allocator, PrimitiveSSBO->Allocation);

    RndRef.VulkanCore.UpdateDescriptor(PrimitiveSSBO, BufferBindPrim, 1);
  } /* End of 'primitive_manager::PrimUpdateSSBO' function */

  /* Update primitive buffer function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID prim::PrimUpdateBuffer( VOID )
  {
    //matr
    //  w = Transform * World,
    //  wnormal = w.Inverse().Transpose(),
    //  wvp = w * Rnd->Camera.VP;
    memset(&PrimitiveData.AddonI, 0, sizeof(INT) * 4);
    memset(&PrimitiveData.AddonF, 0, sizeof(FLOAT) * 4);
    memset(&PrimitiveData.AddonV, 0, sizeof(vec4) * 4);
  } /* End of 'PrimUpdateBuffer' function */
} /* End of 'pivk' function */

/* END OF 'prim.cpp' FILE */