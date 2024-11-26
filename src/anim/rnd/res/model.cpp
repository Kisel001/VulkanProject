/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        model.cpp
 * PROGRAMMER:  IP5
 * LAST UPDATE: 27.10.2024
 * PURPOSE:     Model resource functions declaring file.
 * NOTE:        None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#include "pivk.h"

/* Base project namespace */
namespace pivk
{ 
  /* Load g3dm model function
   * ARGUMENTS:
   *   - name of file with model:
   *       const std::string &Filename;
   *   - render:
   *       render *NewRnd;
   * RETURNS:
   *   (model &) reference to this model.
   */
  model & model::LoadG3DM( const std::string &Filename, render *NewRnd )
  {
    this->Name = Filename;

    if (NewRnd != nullptr)
      Rnd = NewRnd;
    assert(Rnd != nullptr);

    std::fstream f(Filename, std::fstream::in | std::fstream::binary);

    if (!f.is_open())
      return *this;

    f.seekg(0, std::fstream::end);
    INT flen = (INT)f.tellg();                /// std::fpos_t
    f.seekg(0, std::fstream::beg);

    std::vector<BYTE> mem;
    mem.resize(flen);
    BYTE *ptr = mem.data();

    f.read((CHAR *)mem.data(), flen);

    rdr rd(ptr);

    DWORD sign;
    rd(&sign);

    if (sign != *(DWORD *)"G3DM")
      return *this;

    INT NoofP = 0, NoofM = 0, NoofT = 0;
    rd(&NoofP);
    rd(&NoofM);
    rd(&NoofT);

    /* Load primitives */
    std::vector<INT> prims_mtl;
    prims_mtl.resize(NoofP);
    Prims.resize(NoofP);

    for (INT i = 0; i < NoofP; i++)
    {
      INT nv = 0, ni = 0;

      rd(&nv);
      rd(&ni);
      rd(&prims_mtl[i]);

      //int i = sizeof(vertex::std);

      std::vector<vertex::std> V;
      V.resize(nv);

      std::vector<INT> I;
      I.resize(ni);

      rd(&V[0], nv);
      rd(&I[0], ni);

      //for (INT j = 0; j < nv; j++)
      //{
      //  V[j].P = Transform.TransformPoint(V[j].P);
      //  V[j].N = Transform.TransformNormal(V[j].N);
      //}
      Prims[i] = Rnd->PrimCreate(topology::trimesh(V, I));
    }


    // Load materials

    /* Stored material structure */
    struct STORE_MATERIAL
    {
      CHAR Name[300]; // Material name

      /* Illumination coefficients */
      vec3 Ka, Kd, Ks; // Ambient, diffuse, specular coefficients
      FLT Ph;            // Phong power coefficient
      FLT Trans;         // Transparency factor
      INT Tex[8];        // Texture references

      // Shader string
      CHAR ShaderStr[300];
      INT Shader;        // Shader index (not used)
    }; /* End of 'STORE_MATERIAL' struct */

    std::vector<STORE_MATERIAL> store_mtls;
    std::vector<material *>mtls;

    if (NoofM > 0)
    {
      store_mtls.resize(NoofM);
      //rd(&store_mtls[0], NoofM);
      for (INT i = 0; i < NoofM; i++)
      {
        material M = Rnd->MtlGetDef();

        rd(&store_mtls[i]);
        //M.Ka = store_mtls[i].Ka;
        //M.Kd = store_mtls[i].Kd;
        //M.Ks = store_mtls[i].Ks;
        //M.Ph = store_mtls[i].Ph;
        //M.Trans = store_mtls[i].Trans;
        //M.Name = store_mtls[i].Name;

        //for (INT j = 0; j < 8; ++j)
        //  store_mtls[i].Tex[j] = store_mtls[i].Tex[j] == -1 ? -1 : store_mtls[i]
      }
    }

    /* Stored material structure */
    struct STORE_TEX
    {
      CHAR Name[300]; // texture names
      DWORD W, H; // texture size 
      DWORD C;    // count of bytes to one pixel
      //BYTE *Ptr;  // image pixels data
    }; /* End of 'STORE_TEX' struct */

    // Load textures
    std::vector<STORE_TEX> store_texs;
    std::vector<texture *> texs;

    if (NoofT > 0)
    {
      BYTE* Ptr;

      store_texs.resize(NoofT);
      texs.resize(NoofT);

      for (INT i = 0; i < NoofT; ++i)
      {
        rd(store_texs[i].Name, 300);
        rd(&store_texs[i].W);
        rd(&store_texs[i].H);
        rd(&store_texs[i].C);
        Ptr = new BYTE[store_texs[i].W * store_texs[i].H * store_texs[i].C];
        rd(Ptr, store_texs[i].W * store_texs[i].H * store_texs[i].C);

        texs[i] = Rnd->TextureCreateFromImg(store_texs[i].Name, store_texs[i].W, store_texs[i].H, store_texs[i].C, Ptr);

        delete[] Ptr;
      }
    }

    mtls.resize(NoofM);

    // Correct material texture references
    for (INT i = 0; i < NoofM; i++)
    {
      material mtl = Rnd->MtlGetDef();

      mtl.Ka = store_mtls[i].Ka;
      mtl.Kd = store_mtls[i].Kd;
      mtl.Ks = store_mtls[i].Ks;
      mtl.Name = store_mtls[i].Name;
      mtl.Ph = store_mtls[i].Ph;
      mtl.Trans = store_mtls[i].Trans;

      for (INT t = 0; t < 8; t++)
        mtl.Tex[t] = store_mtls[i].Tex[t] == -1 ? nullptr : texs[store_mtls[i].Tex[t]];

      mtls[i] = Rnd->MtlAdd(mtl);
      mtls[i]->Shd = Rnd->ShdFind("Default");
    }

    // Correct primitives material references
    if (NoofM > 0)
      for (INT i = 0; i < NoofP; i++)
        if (prims_mtl[i] != -1)
          Prims[i]->Mtl = mtls[prims_mtl[i]];
        else
          Prims[i]->Mtl = Rnd->MtlFind("Gold");
    else
      for (INT i = 0; i < NoofP; i++)
        Prims[i]->Mtl = Rnd->MtlFind("Gold");

    for (INT i = 0; i < NoofP; i++)
      Prims[i]->UpdateVA();
    //else
    //{
    //  for (INT i = 0; i < NoofP; i++)
    //}

    // Obtain min-max info
    if (NoofP > 0)
    {
      Min = Prims[0]->Min;
      Max = Prims[0]->Max;
      for (INT i = 1; i < NoofP; i++)
      {
        Min = Min.Min(Prims[i]->Min);
        Max = Max.Max(Prims[i]->Max);
      }
    }
    return *this;
  } /* End of 'LoadG3DM' function */

  /* Draw model function.
   * ARGUMENTS:
   *   - world matrix:
   *       const matr &World;
   * RETURNS: None.
   */
  VOID model::Draw( const matr &World )
  {
    matr m = World;
    //anim &Ani = anim::GetRef();
    ///render Rnd;

    /* Draw all nontransparent primitives */
    for (auto &i : Prims)
      if (i->Mtl->Trans == 1)
        Rnd->Draw(i, i->Transform * m);

    /* Draw all transparent primitives */
    
    /* Draw all front-face-culling */
    for (auto &i : Prims)
      if (i->Mtl->Trans != 1)
        Rnd->Draw(i, i->Transform * m);
    
    /* Draw all back-face-culling */
    for (auto &i : Prims)
      if (i->Mtl->Trans != 1)
        Rnd->Draw(i, i->Transform * m);
    
    /* Pop changes */
  } /* End of 'model::Draw' function */
} /* End of 'pivk' namespace */

/* END OF 'model.cpp' FILE */
