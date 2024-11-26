/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        prim.h
 * PROGRAMMER:  IP5
 * LAST UPDATE: 27.10.2024
 * PURPOSE:     Resource primitive class header file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#ifndef __prim_h_
#define __prim_h_

#include "topology.h"
#include "material.h"

/* Base project namespace */
namespace pivk
{
  /* Primitive handle class */
  class prim : public resource
  {
  public:
    /* Struct for storage ubo data of primitive */
    struct PRIM_BUF
    {
      INT  AddonI[4]; // Special integer parameters for shader
      FLT  AddonF[4]; // Special float parameters for shader
      vec4 AddonV[4]; // Special vector 4d parameters for shader
    };

    friend class render;
  private:
    prim_type Type {};                 // Primitive type
    vlk_buf *VBuf{};                   // Vertex buffer
    vlk_buf *IBuf{};                   // Index buffer
    //UINT VA {};                      // Primitive vertex array
    //UINT VBuf {};                    // Vertex buffer
    //UINT IBuf {};                    // Index buffer
    INT NumOfElements {};              // Number of elements for OpenGL
 
    //mutable BOOL IsVAUpdated = FALSE;// Vertex array update flag
 
    // Vertex parameters map (vertex attribute field, offset)
    std::map<std::string, INT_PTR> VertexMap;
    INT VertexStride {};               // Vertex stride in bytes
 
  public:
    material *Mtl {};                  // Material pointer
    matr Transform = matr::Identity(); // Primitive transformation matrix
    vec3
      Min {0},                         // Minimal primitive position
      Max {0};                         // Maximal primitive position
    PRIM_BUF PrimitiveData{};          // Primitive data
    INT Id = -1;                       // Primitive ID in storage buffer

    /* Primitive creation function.
     * ARGUMENTS:
     *   - topology base reference:
     *       const topology::base &T;
     *   - new render:
     *       render *NewRnd;
     * RETURNS:
     *   (prim &) self reference.
     */
    template<class vertex>
      prim & Create( const topology::base<vertex> &T, INT NoofV = 0, render *NewRnd = nullptr )
      {
        Free();

        if (NewRnd != nullptr)
          Rnd = NewRnd;
        assert(Rnd != nullptr);

        Type = T.Type;
        Transform = matr::Identity();
        NumOfElements = 0;
        VertexStride = sizeof(vertex);
 
        if constexpr (requires{vertex::P;})
          VertexMap["InPosition"] =
            reinterpret_cast<INT_PTR>(&((vertex *)0)->P);
        if constexpr (requires{vertex::T;})
          VertexMap["InTexCoord"] =
            reinterpret_cast<INT_PTR>(&((vertex *)0)->T);
        if constexpr (requires{vertex::N;})
          VertexMap["InNormal"] =
            reinterpret_cast<INT_PTR>(&((vertex *)0)->N);
        //if constexpr (requires{vertex::C;})
        //  VertexMap["InColor"] =
        //    reinterpret_cast<INT_PTR>(&((vertex *)0)->C);
        //if constexpr (requires{vertex::Tangent;})
        //  VertexMap["InTangent"] =
        //    reinterpret_cast<INT_PTR>(&((vertex *)0)->Tangent);
        //if constexpr (requires{vertex::Bitangent;})
        //  VertexMap["InBitangent"] =
        //    reinterpret_cast<INT_PTR>(&((vertex *)0)->Bitangent);
        if (T.V.size() == 0 && T.I.size() == 0 && NoofV == 0)
        {
          // Empty primitive
          Max = Min = vec3(0);
          return *this;
        }
 
        // Empty primitive
        if (NoofV != 0)
        {
          NumOfElements = NoofV;
          return *this;
        }

        if (T.V.size() != 0)
        {
          // Collect min-max info
          if constexpr (requires{vertex::P;})
          {
            Min = Max = T.V[0].P;
            for (auto vrt : T.V)
              Min = vrt.P.Min(Min), Max = vrt.P.Max(Max);
          }

          INT count = 0;

          if constexpr (requires{vertex::P;})
          {
            //glEnableVertexAttribArray(count);
            //glVertexAttribPointer(count++, 3, GL_FLOAT, FALSE, sizeof(vertex), (VOID *)0);
          }
          if constexpr (requires{vertex::T;})
          {
            //glEnableVertexAttribArray(count);
            //glVertexAttribPointer(count++, 2, GL_FLOAT, FALSE, sizeof(vertex), (VOID *)12);
          }
          if constexpr (requires{vertex::N;})
          {
            //glEnableVertexAttribArray(count);
            //glVertexAttribPointer(count++, 3, GL_FLOAT, FALSE, sizeof(vertex), (VOID *)20);
          }
          //if constexpr (requires{vertex::C;})
          //{
          //  glEnableVertexAttribArray(count);
          //  glVertexAttribPointer(count, 4, GL_FLOAT, FALSE, sizeof(vertex), (VOID *)32);
          //}
          //if constexpr (requires{vertex::Tangent;})
          //{
          //}
          //if constexpr (requires{vertex::Bitangent;})
          //{
          //}

          VBuf = GetVlk(Rnd)->BufCreate(T.V.size() * sizeof(vertex), 1, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
          VBuf->Write(T.V.data());
          //VBuf.Create(T.V.size(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
          //VBuf.Write(T.V);
        } // vertex buffer creation
 
        // Indices
        if (T.I.size() > 0)
        {
          if (T.V.size() != 0)
          {
            IBuf = GetVlk(Rnd)->BufCreate(T.I.size() * sizeof(INT), 1, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
            IBuf->Write(T.I.data());
            //IBuf.Create(T.I.size(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
            //IBuf.Write(T.V.size())
          }
          NumOfElements = (INT)T.I.size();
        }
        else
          NumOfElements = (INT)T.V.size();
        //UpdateVA();
        return *this;
      } /* End of 'Create' function */
 
    /* Update primitive vertex array function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID UpdateVA( VOID ) const
    {
    } /* End of 'UpdateVA' function */

    /* Free primitive function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID )
    {
      if (VBuf != nullptr)
      {
        VBuf->Free();
        VBuf = nullptr;
      }
      if (IBuf != nullptr)
      {
        IBuf->Free();
        IBuf = nullptr;
      }
      NumOfElements = 0;
    } /* End of 'Free' function */

    /* Update primitive buffer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID PrimUpdateBuffer( VOID );

  }; /* End of 'prim' class */

  /* Primitive manager */
  class primitive_manager : public resource_manager<prim>
  {
  public:
    BOOL UpdatePrimitiveSSBO = TRUE;  // Update primitive SSBO flag.
    vlk_buf *PrimitiveSSBO = nullptr; // Material storage buffer

    /* Primitive manager constructor.
     * ARGUMENTS:
     *   - reference to render:
     *       render &Rnd;
     */
    primitive_manager( render &Rnd ) : resource_manager(Rnd)
    {
    } /* End of 'primitive_manager' function */

    /* Init primitive manager function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID PrimInit( VOID );

    /* Create primitive function.
     * ARGUMENTS:
     *   - topology base reference:
     *       const topology::base &Tpl;
     * RETURNS:
     *   (prim *) created primitive interface.
     */
    template<class vertex_type>
      prim * PrimCreate( const topology::base<vertex_type> &Tpl, INT NoofV = 0 )
      {
        return resource_manager::Add(prim().Create(Tpl, NoofV, &RndRef));
        UpdatePrimitiveSSBO = TRUE;
      } /* End of 'PrimCreate' function */

    /* Free primitive function.
     * ARGUMENTS:
     *   - primitive pointer:
     *       prim *Pr;
     * RETURNS: None.
     *   (prim *) created primitive interface.
     */
    VOID PrimFree( prim *Pr )
    {
      Pr->Free();
      resource_manager::Delete(Pr);
    } /* End of 'PrimFree' function */

    /* Primitive update storage buffer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID PrimUpdateSSBO( VOID );

    /* Update primitive buffer function.
     * ARGUMENTS:
     *   - pointer to primitive:
     *       prim *Pr;
     * RETURNS: None.
     */
    VOID PrimUpdateBuffer( prim *Pr )
    {
      Pr->PrimUpdateBuffer();
      UpdatePrimitiveSSBO = TRUE;
    } /* End of 'PrimUpdateBuffer' function */

  }; /* End of 'primitive_manager' class */

} /* end of 'pivk' namespace */

#endif // !__prim_h_

/* END OF 'prim.h' FILE */
