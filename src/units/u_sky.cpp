/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

 /* FILE:        u_sky.h
  * PROGRAMMER:  IP5
  * LAST UPDATE: 13.09.2024
  * PURPOSE:     Unit sky header file.
  * NOTE:        None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum.
  */

#include "pivk.h"

/* Base project namespace */
namespace pivk
{
  /* Unit skybox class */
  class unit_sky : public unit
  {
    prim *Pr;
    BOOL MtlIsCreated = FALSE;

  public:
    /* Unit model constructor
     * ARGUMENTS:
     *   - animation reference:
     *       anim &A;
     */
    unit_sky( anim &A )
    {
      Pr = A.PrimCreate<vertex::std>({prim_type::POINTS, {}, {}}, 1);
      //Pr->Mtl = A.MtlAdd(mtl);
    } /* End of 'unit_skt' function */

    /* Default destructor */
    ~unit_sky() override
    {
      Pr->Free();
    } /* End of '~unit_sky' function */

    /* Unit response function.
     * ARGUMENTS:
     *   - animation reference:
     *       const pivk::anim &A;
     * RETURNS: NONE.
     */
    VOID Response( pivk::anim &A ) override
    {
      if (!MtlIsCreated)
      {
        MtlIsCreated = TRUE;
        material mtl = A.MtlGetDef();

        mtl.Name = "Sky";
        mtl.Shd = A.ShdFind("sky");
        Pr->Mtl = A.MtlAdd(mtl);
      }
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
    } /* End of 'Render' function */

  }; /* End of 'unit_sky' class */

  static anim::unit_register<unit_sky> _UnitSkyInitSecurity("Sky");
} /* end of 'pivk' namespace */
