/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        anim.cpp
 * PROGRAMMER:  IP5
 * LAST UPDATE: 28.10.2024
 * PURPOSE:     Animation of project file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#include "pivk.h"

/* Base project namespace */ 
namespace pivk
{
  /* Default destuctor */
  anim::~anim( VOID )
  {
    Units.Walk([]( unit *Uni )
    {
      delete Uni;
    });

    render::Close();
  } /* End of 'anim::~anim' function */

  /* Animation render function
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID anim::Render( VOID )
  {
    if (CloseState == TRUE)
      return;

    /* System responses */
    
    // Update response, if window active
    if (IsActive)
      input::Response(win::hWnd, MouseWheel);
    timer::Response(win::hWnd);

    /* Response all units */
    Units.Walk([&]( unit *Uni )
    {
      Uni->Response(*this);
    });

    render::FrameStart();

    /* Render all units */
    Units.Walk([&]( unit *Uni )
    {
      Uni->Render(*this);
    });

    render::FrameEnd();

    MouseWheel = 0;
  } /* End of 'Render' function */
} /* end of 'pivk' function */

/* Special static class for initialization of animation */
//static class anim_start
//{
//public:
//  /* Class default constructor */
//  anim_start( VOID )
//  {
//    /* Init animation function */
//    pivk::anim &Ani = pivk::anim::GetRef();
//    Ani.CreateClass();
//    Ani.WindowCreate("CGSG IP5 :: ANIM Project");
//  } /* End of 'anim_start' function */
//} _AnimStartStaticClassElementSecurity;

/* END OF 'anim.cpp' FILE */
