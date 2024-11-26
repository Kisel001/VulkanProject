/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        u_ctrl.h
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
  class unit_model : public unit
  {
    model *Model;   // Pointer to model
    matr Trans;     // Trans matr
    vec3 Pos;       // Position vector
    FLT Angle;      // Rotate XZ angle
    FLT Speed;      // Speed of car
    FLT AngleWheel; // Rotate Wheel

  public:
    /* Unit model constructor
     * ARGUMENTS:
     *   - animation reference:
     *       anim &A;
     */
    unit_model( anim &A ) : Pos(0, 0, 0), Angle(0), Speed(0), AngleWheel(0)
    {
      Model = A.ModelLoadG3DM("bin/models/x6.g3dm");
    } /* End of 'unit' function */

    /* Default destructor */
    ~unit_model() override
    {
      Model->Free();
    } /* End of '~unit' function */

    /* Unit response function.
     * ARGUMENTS:
     *   - animation reference:
     *       const pivk::anim &A;
     * RETURNS: NONE.
     */
    VOID Response( anim &A ) override
    {
      static FLT twist = 5, AngleTwist = 100;
      /*+++ Array with prims:
      static INT prms[] =
      {
        315, 306, 305, 245, 253, 252, 251, 250, 375, 367, 183, 184, 192
      };---*/
      INT i;

#if 1

      /* Changing Angle of horizontal rotate */
      Angle += (AngleTwist * A.DeltaTime) *
        (A.Keys['A'] - A.Keys['D']);

      /* Change angle for safety */
      if (Angle > 180.0)
        Angle -= 360.0;
      if (Angle < -180.0)
        Angle += 360.0;

      /* Changing horizontal position and rotate wheels */
      Speed += A.DeltaTime * twist *
        (A.Keys['W'] - A.Keys['S']);

      AngleWheel += Speed / 200;

      /* Changing for safety */
      if (abs(Speed) < A.DeltaTime * twist / 3)
        Speed = 0;

      if (Speed > 0)
        Speed -= A.DeltaTime * twist / 3;
      if (Speed < 0)
        Speed += A.DeltaTime * twist / 3;

      Pos.X += A.DeltaTime * twist * sin(D2R(Angle)) * Speed;
      Pos.Z += A.DeltaTime * twist * cos(D2R(Angle)) * Speed;

      /* Change AngleWheel for safety */
      if (AngleWheel > PI)
        AngleWheel -= 2 * PI;
      if (AngleWheel < -PI)
        AngleWheel += 2 * PI;

      /* Changing Vertical position */
      Pos.Y += twist * A.DeltaTime * (A.Keys['Q'] - A.Keys['E']);

      A.Pos = Pos;
      A.Wheel = Angle;

      Trans = matr::Scale(vec3(5, 5, 5)) *
              matr::Rotate(-90, vec3(1, 0, 0)) *
              matr::RotateY(Angle) *
              matr::Translate(Pos);

      /* NUMBERS OF WHEEL'S PRIMITIVES
       * 315, 306, 305
       * 245, 253, 252, 251, 250
       * 375, 367
       * 183, 184, 192
       * Start on 125th primitive
       */
      for (i = 125; i < Model->Prims.size(); i++)
      {
        vec3 B = (Model->Prims[i]->Min + Model->Prims[i]->Max) / 2;
        Model->Prims[i]->Transform = matr::Translate(-B) * matr::RotateX(R2D(AngleWheel)) * matr::Translate(B);
      }

#endif
    } /* End of 'Response' function */

    /* Unit render function.
     * ARGUMENTS:
     *   - animation reference:
     *       const pivk::anim &A;
     * RETURNS: NONE.
     */
    VOID Render( pivk::anim &A ) override
    {
   
      Model->Draw(Trans);
      //Model->
    } /* End of 'Render' function */

  }; /* End of 'unit_model' class */

  static anim::unit_register<unit_model> _UnitModelInitSecurity("Model");
} /* end of 'pivk' namespace */

