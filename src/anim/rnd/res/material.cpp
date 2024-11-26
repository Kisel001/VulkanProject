/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        material.cpp
 * PROGRAMMER:  IP5
 * LAST UPDATE: 27.10.2024
 * PURPOSE:     Material functions declaring file.
 * NOTE:        None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#include "pivk.h"

/* Base project namespace */
namespace pivk
{
  /* Init material manager function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID material_manager::MtlInit()
  {
    // Default materials list
    static struct _MatLibType
    {
      std::string Name;
      vec3 amb, dif, spec;
      FLOAT shin;
    } MatLib[] =
    {
      {"Black Plastic", {0.0, 0.0, 0.0},             {0.01, 0.01, 0.01},           {0.5, 0.5, 0.5},               32},
      {"Brass",         {0.329412,0.223529,0.027451}, {0.780392,0.568627,0.113725}, {0.992157,0.941176,0.807843}, 27.8974},
      {"Bronze",        {0.2125,0.1275,0.054},       {0.714,0.4284,0.18144},       {0.393548,0.271906,0.166721},  25.6},
      {"Chrome",        {0.25, 0.25, 0.25},          {0.4, 0.4, 0.4},              {0.774597, 0.774597, 0.774597}, 76.8},
      {"Copper",        {0.19125,0.0735,0.0225},     {0.7038,0.27048,0.0828},      {0.256777,0.137622,0.086014},  12.8},
      {"Gold",          {0.24725,0.1995,0.0745},     {0.75164,0.60648,0.22648},    {0.628281,0.555802,0.366065},  51.2},
      {"Peweter",       {0.10588,0.058824,0.113725}, {0.427451,0.470588,0.541176}, {0.3333,0.3333,0.521569},      9.84615},
      {"Silver",        {0.19225,0.19225,0.19225},   {0.50754,0.50754,0.50754},    {0.508273,0.508273,0.508273},  51.2},
      {"Polished Silver", {0.23125,0.23125,0.23125}, {0.2775,0.2775,0.2775},       {0.773911,0.773911,0.773911},  89.6},
      {"Turquoise",     {0.1, 0.18725, 0.1745},      {0.396, 0.74151, 0.69102},    {0.297254, 0.30829, 0.306678}, 12.8},
      {"Ruby",          {0.1745, 0.01175, 0.01175},  {0.61424, 0.04136, 0.04136},  {0.727811, 0.626959, 0.626959}, 76.8},
      {"Polished Gold", {0.24725, 0.2245, 0.0645},   {0.34615, 0.3143, 0.0903},    {0.797357, 0.723991, 0.208006}, 83.2},
      {"Polished Bronze", {0.25, 0.148, 0.06475},    {0.4, 0.2368, 0.1036},        {0.774597, 0.458561, 0.200621}, 76.8},
      {"Polished Copper", {0.2295, 0.08825, 0.0275}, {0.5508, 0.2118, 0.066},      {0.580594, 0.223257, 0.0695701}, 51.2},
      {"Jade",          {0.135, 0.2225, 0.1575},     {0.135, 0.2225, 0.1575},      {0.316228, 0.316228, 0.316228}, 12.8},
      {"Obsidian",      {0.05375, 0.05, 0.06625},    {0.18275, 0.17, 0.22525},     {0.332741, 0.328634, 0.346435}, 38.4},
      {"Pearl",         {0.25, 0.20725, 0.20725},    {1.0, 0.829, 0.829},          {0.296648, 0.296648, 0.296648}, 11.264},
      {"Emerald",       {0.0215, 0.1745, 0.0215},    {0.07568, 0.61424, 0.07568},  {0.633, 0.727811, 0.633},       76.8},
      {"Black Plastic", {0.0, 0.0, 0.0},             {0.01, 0.01, 0.01},           {0.5, 0.5, 0.5},                32.0},
      {"Black Rubber",  {0.02, 0.02, 0.02},          {0.01, 0.01, 0.01},           {0.4, 0.4, 0.4},                10.0},
    };
    constexpr auto MatLibSize = sizeof(MatLib) / sizeof(MatLib[0]);

    for (INT i = 0; i < MatLibSize; ++i)
    {
      material *M = material_manager::Add(material().Create(MatLib[i].Name,
                                          MatLib[i].amb,
                                          MatLib[i].dif,
                                          MatLib[i].spec,
                                          MatLib[i].shin, 1, nullptr, {}, &RndRef));

      //M->Shd = RndRef.ShdFind("default");
    }

    MaterialSSBO = RndRef.VulkanCore.BufCreate(10000 * sizeof(material::BUF_MTL), 10000, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
  } /* End of 'MtlInit' function */

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
   *   - pointer to new render:
   *       render *NewRnd;
   * RETURNS:
   *   (material &) self reference.
   */
  material & material::Create( const std::string &NewName,
                               const vec3 &NewKa,
                               const vec3 &NewKd,
                               const vec3 &NewKs,
                               FLT NewPh, FLT NewTrans,
                               shader *NewShd,
                               std::initializer_list<texture *> Textures,
                               render *NewRnd )
  {
    Name = NewName;
    Ka = NewKa;
    Kd = NewKd;
    Ks = NewKs;
    Ph = NewPh;
    Trans = NewTrans;
    Shd = NewShd;
    if (NewRnd != nullptr)
      Rnd = NewRnd;
    assert(Rnd != nullptr);

    INT count = 0;

    for (const auto &i : Textures)
      Tex[count++] = i;
    for (; count < 8; ++count)
      Tex[count] = nullptr;

    //BUF_MTL MtlData = {vec4(Ka, 0), vec4(Kd, Trans), vec4(Ks, Ph)};

    //for (INT i = 0; i < 8; ++i)
    //  MtlData.TextureFlags[i] = this->Tex[i] == nullptr ? 0  : 1;

    //this->BufferMtl = Rnd->VulkanCore.BufCreate(sizeof(BUF_MTL), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);//Rnd->BufCreate(this->Name, BufferBindMaterial, 1, &MtlData);
    //this->BufferMtl->Write(&MtlData);

    return *this;
  } /* End of 'material::Create' function */

  /* Get shader ProgID function.
   * ARGUMENTS:
   * RETURNS:
   *   (INT) ProgID of shader.
   */
  INT material::Apply( VOID )
  {
    // Apply ubo buffer
    this->Rnd->VulkanCore.BindDescriptor(BufferBindMaterial);

    /* Set textures */
    for (INT i = 0; i < 8; i++)
    {
      if (this->Tex[i] != nullptr)
        Tex[i]->TexApply(i);
    }
    if (auto *ptr = Rnd->TextureFind("Skytex"); ptr != nullptr)
      ptr->TexApply(8);

    return INT();
  } /* End of 'material::Apply' function */

  /* Update buffers and textures function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID material::Update( VOID )
  {
    //BufferMtl
    //BUF_MTL buf;
    //buf.Ka = vec4(Ka, 0);
    //buf.KdTrans = vec4(Kd, Trans);
    //buf.KsPh = vec4(Ks, Ph);
    //for (INT i = 0; i < 8; i++)
    //{
    //  if (this->Tex[i] != nullptr)
    //    buf.TextureFlags[i] = 1;
    //  else
    //    buf.TextureFlags[i] = -1;
    //}
    ////BufferMtl->Write(&buf);
    //
    ////this->Rnd->VulkanCore.UpdateDescriptor(BufferMtl, BufferBindMaterial);
    ////this->Rnd->VulkanCore.BindDescriptor(BufferBindMaterial);
    //
    ///* Set textures */
    //for (INT i = 0; i < 8; i++)
    //{
    //  if (this->Tex[i] != nullptr)
    //  {
    //    this->Rnd->VulkanCore.TexUpdateDescriptor(Tex[i]->ImageView, Tex[i]->Sampler, i + 4);
    //    //this->Rnd->VulkanCore.BindDescriptor(BufferBindMaterial);
    //  }
    //}
    //if (auto *ptr = Rnd->TextureFind("Skytex"); ptr != nullptr)
    //{
    //  this->Rnd->VulkanCore.TexUpdateDescriptor(ptr->ImageView, ptr->Sampler, 12);
    //  //this->Rnd->VulkanCore.BindDescriptor(BufferBindMaterial);
    //}
  } /* End of 'material::Update' function */

  /* Update material storage buffer function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID material_manager::MtlUpdateSSBO( VOID )
  {
    if (UpdateMaterialSSBO == FALSE)
      return;

    UpdateMaterialSSBO = FALSE;
    MaterialSSBO->Resize(resource_manager::TotalRefCnt * sizeof(material::BUF_MTL), resource_manager::TotalRefCnt);

    BYTE *MapPointer;

    if (vmaMapMemory(RndRef.VulkanCore.Allocator, MaterialSSBO->Allocation, reinterpret_cast<VOID **>(&MapPointer)) != VK_SUCCESS)
    {
      throw;
    }

    material::BUF_MTL Buf;

    INT count = 0;

    for (auto &i : Stock)
    {
      i.second.Id = count++;
      Buf.Ka = vec4(i.second.Ka, 0);
      Buf.KdTrans = vec4(i.second.Kd, i.second.Trans);
      Buf.KsPh = vec4(i.second.Ks, i.second.Ph);
      
      for (INT j = 0; j < 8; ++j)
        Buf.TextureFlags[j] = i.second.Tex[j] == nullptr ? 0 : 1;

      memcpy(MapPointer, &Buf, sizeof(material::BUF_MTL));
      MapPointer += sizeof(material::BUF_MTL);
    }

    vmaUnmapMemory(RndRef.VulkanCore.Allocator, MaterialSSBO->Allocation);

    RndRef.VulkanCore.UpdateDescriptor(MaterialSSBO, BufferBindMaterial, 1);
  } /* End of 'MtlUpdateSSBO' function */
} /* end of 'pivk' namespace */

/* END OF 'texture.cpp' FILE */
