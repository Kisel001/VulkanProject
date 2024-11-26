/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        material.h
 * PROGRAMMER:  IP5
 * LAST UPDATE: 27.10.2024
 * PURPOSE:     Resource material class header file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#ifndef __material_h_
#define __material_h_

#include "shader.h"
#include "buffer.h"
#include "texture.h"

/* Base project namespace */
namespace pivk
{
  /* Material class */
  class material : public resource
  {
  public:
    // Material buffer data type
    struct BUF_MTL
    {
      vec4 Ka;             // Ambient
      vec4 KdTrans;        // Diffuse + Transparency
      vec4 KsPh;           // Specular + Phong
      INT TextureFlags[8]; // Material texture flags
    }; /* End of 'BUF_MTL' structure */
 
    // Material name
    std::string Name;
 
    // Illumination coefficients (anbient, diffuse, specular)
    vec3 Ka, Kd, Ks;
    // Phong coefficient and transparency value
    FLT Ph, Trans;
    // Shader pointer
    shader *Shd {};
    // Texture pointers
    texture *Tex[8];
    // Id in SSBO buffer
    INT Id = -1;
 
    // Material data buffer
    //buffer *BufferMtl {};
    //vlk_buf *BufferMtl {};
 
    /* Class default constructor */
    material( VOID )
    {
    } /* End of 'material' function */
 
    /* Class destructor */
    ~material( VOID )
    {
    } /* End of '~material' function */

    /* Material creation function.
     * ARGUMENTS:
     *   - material name:
     *       const std::string &NewName;
     *   - illumination coefficients (ambient. diffuse, specular):
     *       const color3 &NewKa, &NewKd, &NewKs;
     *   - Phong shininess coefficient:
     *       FLT NewPh;
     *   - transparency coefficient:
     *       FLT NewTrans;
     *   - shader pointer:
     *       shader *NewShd;
     *   - textures list:
     *       std::initializer_list<texture *> Textures;
     *   - pointer to render (nullptr if old):
     *       render *NewRnd;
     * RETURNS:
     *   (material &) self reference.
     */
    material & Create( const std::string &NewName,
                       const vec3 &NewKa = vec3(0.1),
                       const vec3 &NewKd = vec3(0.9),
                       const vec3 &NewKs = vec3(0.0),
                       FLT NewPh = 30, FLT NewTrans = 1,
                       shader *NewShd = nullptr,
                       std::initializer_list<texture *> Textures = {},
                       render *NewRnd = nullptr );

    /* Material destroy function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID )
    {
      //if (this->BufferMtl != nullptr)
      //  this->BufferMtl->Free();
      //this->BufferMtl = nullptr;
    } /* End of 'Free' function */

    /* Get shader ProgID function.
     * ARGUMENTS:
     *   - pointer to material:
     *       INT MtlNo;
     * RETURNS:
     *   (INT) ProgID of shader.
     */
    INT Apply( VOID );

    /* Update buffers and textures function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Update( VOID );

  }; /* End of 'material' class */

  /* Material manager class */
  class material_manager : public resource_manager<material, std::string>
  {
  public:
    BOOL IsNoAddedDefMtl = TRUE;    // Flag of adding default material
    material *DefMtl = nullptr;     // Pointer to default material
    BOOL UpdateMaterialSSBO = TRUE; // Update material SSBO flag.
    vlk_buf *MaterialSSBO;          // Material storage buffer

    /* Material manager constructor.
     * ARGUMENTS:
     *   - ref to render:
     *       render &Rnd;
     */
    material_manager( render &Rnd ) : resource_manager(Rnd)
    {
    } /* End of 'material_manager' function */

    /* Init material manager function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID MtlInit();

    /* Get default material function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (material *) pointer to default material.
     */
    material MtlGetDef( VOID )
    {
      //static BOOL IsNoAddedDefMtl = true;
      //static material *Def = nullptr;

      // Check that default material no added.
      // Then added him.
      if (IsNoAddedDefMtl)
      {
        IsNoAddedDefMtl = false;

        DefMtl = material_manager::Add(material().Create("Default",
                                    {0.1, 0.1, 0.1},
                                    {0.9, 0.9, 0.9},
                                    {0.3, 0.3, 0.3},
                                    30, 1, nullptr, {}, &RndRef));
      }

      return *DefMtl;
    } /* End of 'MtlGetDef' function */

    /* Add material function.
     * ARGUMENTS:
     *   - reference to material:
     *       const material &Mat;
     * RETURNS:
     *   (material *) pointer to new material.
     */
    material * MtlAdd( const material &Mat )
    {
      material *mtl = resource_manager::Find(Mat.Name);

      if (mtl == nullptr)
      {
        UpdateMaterialSSBO = TRUE;
        return resource_manager::Add(material().Create(Mat.Name, Mat.Ka, Mat.Kd, Mat.Ks, Mat.Ph, Mat.Trans, Mat.Shd, 
                                     {Mat.Tex[0], Mat.Tex[1], Mat.Tex[2], Mat.Tex[3], Mat.Tex[4], Mat.Tex[5], Mat.Tex[6], Mat.Tex[7]}, &RndRef));
      }
      return resource_manager::Find(Mat.Name);
    } /* End of 'MtlAdd' function */

    /* Get shader ProgID
     * ARGUMENTS:
     *   - pointer to material:
     *       INT MtlNo;
     * RETURNS:
     *   (INT) ProgID of shader.
     */
    INT MtlApply( material *Mtl )
    {
      Mtl->Apply();
    } /* End of 'MtlApply' function */

    /* Free material function.
     * ARGUMENTS:
     *   - pointer to material:
     *       material *Mtl;
     * RETURNS: None.
     */
    VOID MtlFree( material *Mtl )
    {
      material *i = resource_manager::Find(Mtl->Name);

      if (i != nullptr)
        resource_manager::Delete(i);
    } /* End of 'MtlFree' function */

    /* Get material by name function.
     * ARGUMENTS:
     *   - name of material:
     *       std::string Name;
     * RETURNS:
     *   (material *) pointer to material.
     */
    material * MtlFind( std::string Name )
    {
      return resource_manager::Find(Name);
    } /* End of 'MtlFind' function */

    /* Update material storage buffer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID MtlUpdateSSBO( VOID );

  }; /* End of 'material_manager' class */

} /* end of 'pivk' namespace */

#endif // !__material_h_

/* END OF 'material.h' FILE */
