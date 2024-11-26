/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        input.h
 * PROGRAMMER:  IP5
 * LAST UPDATE: 30.07.2024
 * PURPOSE:     Input header file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#ifndef __input_h_
#define __input_h_

#include "keyboard.h"
#include "mouse.h"
#include "joystick.h"

/* Base project namespace */
namespace pivk
{
  /* Input class */
  class input : public keyboard, public mouse, public joystick
  {
  public:
    /* Default constructor */
    input() : keyboard(), mouse(), joystick()
    {
    } /* End of 'input' function */

    /* Default destructor */
    ~input()
    {
    } /* End of '~input' function */

    /* Input response function.
     * ARGUMENTS:
     *   - window handle:
     *       HWND hWnd;
     *   - mouse wheel:
     *       INT MouseWheel;
     * RETURNS: None.
     */
    VOID Response( HWND hWnd, INT MouseWheel )
    {
      keyboard::Response();
      mouse::Response(hWnd, MouseWheel);
      joystick::Response();
    } /* End of 'Response' function */

  }; /* End of 'input' class */
} /* end of 'pivk' file */

#endif // !__input_h_

/* END OF 'input.h' FILE */
