/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        mouse.h
 * PROGRAMMER:  IP5
 * LAST UPDATE: 30.07.2024
 * PURPOSE:     Mouse header file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#ifndef __mouse_h_
#define __mouse_h_

#include "def.h"

/* Base project namespace */
namespace pivk
{
  /* Mouse class */
  class mouse
  {
  public:
    INT 
      Mx, My, Mz;     // Mouse current coordinates
    INT 
      Mdx, Mdy, Mdz;  // Mouse change coordinates

    /* Default constructor */
    mouse( VOID ) : Mx(0), My(0), Mz(0), Mdx(0), Mdy(0), Mdz(0)
    {
    } /* End of 'mouse' function */

    /* Default destuctor */
    ~mouse( VOID )
    {
    } /* End of '~mouse' function */

    /* Response of mouse function.
     * ARGUMENTS:
     *   - window handle:
     *       HWND hWnd;
     *   - mouse wheel:
     *       INT MouseWheel;
     * RETURNS: None.
     */
    VOID Response( HWND hWnd, INT MouseWheel )
    {
      POINT pt;

      GetCursorPos(&pt);
      ScreenToClient(hWnd, &pt);

      /* Coordinates to Screen */
      Mdx = pt.x - Mx;
      Mdy = pt.y - My;
      Mdz = MouseWheel;

      /* Absolute values */
      Mx = pt.x;
      My = pt.y;
      Mz += MouseWheel;
    } /* End of 'Response' function */

  }; /* End of 'mouse' class */
} /* end of 'pivk' file */

#endif // !__mouse_h_

/* END OF 'mouse.h' FILE */
