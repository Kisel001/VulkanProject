/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        u_triangle.h
 * PROGRAMMER:  IP5
 * LAST UPDATE: 21.11.2024
 * PURPOSE:     Unit triangle header file.
 * NOTE:        None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#include "pivk.h"

/* Base project namespace */
namespace pivk
{
  /* Unit triangle class */
  class unit_triangle : public unit
  {
  public:
    pipeline_pattern *PipelinePattern; // Pipeline pattern
    shader *Shd;                       // Pointer to shader
    prim *Pr;                          // Pointer to primitive
    texture *Tex;                      // Pointer to texture
    material *Mtl;                     // Pointer to material
    dae DaeParser;                     // DAE parser

    /* Unit triangle constructor.
     * ARGUMENTS:
     *   - animation reference:
     *       anim &A;
     */
    unit_triangle( anim &A ) : DaeParser("bin/models/ZebraLOD1.dae")
    {   
      std::vector<std::pair<std::vector<vertex::std>, std::vector<INT>>> PrimsData;

      DaeParser.LoadGeometry(PrimsData);

      PipelinePattern = A.PipelinePatternFind("Zebra pattern");

      PipelinePattern->Add("Position", GetAttributeDescription(0, VK_FORMAT_R32G32B32_SFLOAT, 0));
      PipelinePattern->Add("TC", GetAttributeDescription(1, VK_FORMAT_R32G32_SFLOAT, sizeof(vec3)));
      PipelinePattern->Add("Normal", GetAttributeDescription(2, VK_FORMAT_R32G32B32_SFLOAT, sizeof(vec3) + sizeof(vec2)));
      Shd = A.ShdCreate("test", PipelinePattern);

      image Img {"bin/textures/M.G24", img_type::G24};

      std::vector<vertex::std> V =
      {
        {{0.5f, 0.5f, 1.f}, {1.f, 1.f}, {0, 0, 1}},
        {{0.5f, -0.5f, 1.f}, {1.f, 0.f}, {0, 0, 1}},
        {{-0.5f, 0.5f, 1.f}, {0.f, 1.f}, {0, 0, 1}},
        {{-0.5f, -0.5f, 1.f}, {0.f, 0.f}, {0, 0, 1}},
      };
      std::vector<INT> I =
      {
        1, 0, 2, 1, 2, 3
      };

      //Pr = A.PrimCreate<vertex::std>(topology::trimesh<vertex::std>(V, I));
      Pr = A.PrimCreate<vertex::std>(topology::trimesh<vertex::std>(PrimsData[1].first, PrimsData[1].second));

      Pr->Mtl = A.MtlFind("Gold");
      material mtl;// = A.MtlGetDef();
      memcpy(&mtl, Pr->Mtl, sizeof(material));
      mtl.Name = std::move(std::string("MakakaTex"));
      Tex = A.TextureCreateFromImg("Makaka", Img.W, Img.H, 4, Img.GetPtrToImgData());
      //mtl.Tex[0] = Tex;

      Pr->Mtl = A.MtlAdd(mtl);
    } /* End of 'unit_triangle' function */

    /* Default destructor */
    ~unit_triangle() override
    {
      Tex->TexFree();

      //Pr->Mtl->Free();
      Pr->Free();
      /// Delete of primitive must be not in destructor
      /// Pr->Free();
    } /* End of '~unit_triangle' function */

    /* Unit response function.
     * ARGUMENTS:
     *   - animation reference:
     *       const pivk::anim &A;
     * RETURNS: NONE.
     */
    VOID Response( anim &A ) override
    {
      Pr->Mtl->Update();
    } /* End of 'Response' function */

    /* Unit render function.
     * ARGUMENTS:
     *   - animation reference:
     *       const pivk::anim &A;
     * RETURNS: NONE.
     */
    VOID Render( pivk::anim &A ) override
    {
      Shd->Apply();
      A.Draw(Pr);
      A.Draw(Pr, matr::Translate(vec3(0, 0, 5)));
    } /* End of 'Render' function */

  }; /* End of 'unit_triangle' class */

  static anim::unit_register<unit_triangle> _UnitTriangleInitSecurity("Triangle");
} /* end of 'pivk' namespace */

/* END OF 'u_triangle.cpp' FILE */
