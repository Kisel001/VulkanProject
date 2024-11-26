/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        u_map.cpp
 * PROGRAMMER:  IP5
 * LAST UPDATE: 13.09.2024
 * PURPOSE:     Animation header file.
 * NOTE:        None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#include "pivk.h"

/* Base project namespace */
namespace pivk
{
  /* Unit model class */
  class unit_map : public unit
  {
    prim *Pr;     // Map primitive
    model *Oak;   // Oak model
    model *House; // House model

  public:
    /* Unit model constructor
     * ARGUMENTS:
     *   - animation reference:
     *       anim &A;
     */
    unit_map( anim &A )
    {
      material mtl = A.MtlGetDef();
      BYTE Txt[2][2] = {{255, 0}, {0, 255}};
    
      image Img = image("bin/textures/campmap.g24", img_type::G24);

      mtl.Name = "CampMap";
      if (Img.Pixels != nullptr)
        mtl.Tex[0] = A.TextureCreateFromImg("Campmap", Img.W, Img.H, 4, Img.Pixels);
      else
        mtl.Tex[0] = A.TextureCreateFromImg("Campmap", 2, 2, 1, Txt);

      std::vector<vertex::std> V;
      //V.push_back(vertex::std({-200, -1, -200}, {0, 1}, {0, 1, 0}, {0, 0, 0, 1}));
      //V.push_back(vertex::std({-200, -1,  200}, {0, 0}, {0, 1, 0}, {0, 0, 0, 1}));
      //V.push_back(vertex::std({ 200, -1, -200}, {1, 1}, {0, 1, 0}, {0, 0, 0, 1}));
      //V.push_back(vertex::std({ 200, -1,  200}, {1, 0}, {0, 1, 0}, {0, 0, 0, 1}));

      Pr = A.PrimCreate<vertex::std>({prim_type::STRIP, V, {}});
      Pr->Mtl = A.MtlAdd(mtl);
      Pr->Mtl->Shd = A.ShdFind("default");
      Pr->UpdateVA();

      Oak = new model();
      Oak->LoadG3DM("bin/models/oak.g3dm", &A);
      House = new model();
      House->LoadG3DM("bin/models/house.g3dm", &A);
    } /* End of 'unit_skt' function */

    /* Default destructor */
    ~unit_map() override
    {
      Pr->Free();
    } /* End of '~unit_map' function */

    /* Unit response function.
     * ARGUMENTS:
     *   - animation reference:
     *       const pivk::anim &A;
     * RETURNS: NONE.
     */
    VOID Response( anim &A ) override
    {
    } /* End of 'Response' function */

    /* Unit render function.
     * ARGUMENTS:
     *   - animation reference:
     *       const pivk::anim &A;
     * RETURNS: NONE.
     */
    VOID Render( pivk::anim &A ) override
    {
      A.Draw(Pr);
      Oak->Draw(matr::Translate(vec3(20, 0, 0)));
      House->Draw(matr::RotateY(100) * matr::Translate(vec3(29.3, 0, 15)));
      House->Draw(matr::RotateY(100) * matr::Translate(vec3(10.3, 0, 0.3)));
      House->Draw(matr::RotateY(100) * matr::Translate(vec3(34.7, 0, -7.5)));
      House->Draw(matr::RotateY(150) * matr::Translate(vec3(58, 0, -14)));
      House->Draw(matr::RotateY(170) * matr::Translate(vec3(-23.5, 0, -91)));
      House->Draw(matr::RotateY(-10) * matr::Translate(vec3(-36, 0, -16)));
      //House->Draw(matr::RotateY(-10) * matr::Translate(vec3(-44.7, 0, -58.4)));
      House->Draw(matr::RotateY(170) * matr::Translate(vec3(-58.5, 0, -115.5)));
      House->Draw(matr::RotateY(170) * matr::Translate(vec3(52, 0, -134.5)));
      House->Draw(matr::RotateY(65) * matr::Translate(vec3(-82, 0, -66)));
      House->Draw(matr::RotateY(175) * matr::Translate(vec3(2, 0, -66)));
      House->Draw(matr::RotateY(175) * matr::Translate(vec3(45, 0, -60)));
      House->Draw(matr::RotateY(-100) * matr::Translate(vec3(-85.7, 0, 109.6)));
      House->Draw(matr::RotateY(74) * matr::Translate(vec3(-42, 0, 92)));
    } /* End of 'Render' function */
  }; /* End of 'unit_map' class */

  static anim::unit_register<unit_map> _UnitMapInitSecurity("Map");
}; /* end of 'pivk' namespace */

/* END OF 'u_map.cpp' FILE */
