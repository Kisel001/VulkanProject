/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        joystick.h
 * PROGRAMMER:  IP5
 * LAST UPDATE: 31.07.2024
 * PURPOSE:     Joystick header file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#ifndef __joystick_h_
#define __joystick_h_

#include <windows.h>
#include <mmsystem.h>

#include "def.h"

/* Base project namespace */
namespace pivk
{
  /* Total count of keys */
  inline constexpr UINT JButCount = 32;

  /* Keyboard class */
  class joystick
  {
  private:
    BYTE JButOld[JButCount];   // Previus frame joystick keys states
  public:
    BYTE JBut[JButCount];      // Joystick press keys states
    BYTE JButClick[JButCount]; // Joystick ckick keys states
    
    INT JPov;                  // Joystick point-of-view control [-1, 0, ..., 7]
    FLT
      JX, JY, JZ, JR;          // Joystick axes

    /* Default constructor */
    joystick( VOID ) : JPov(0), JX(0), JY(0), JZ(0), JR(0)
    {
      std::memset(JBut, 0, sizeof(BYTE) * JButCount);
      std::memset(JButClick, 0, sizeof(BYTE) * JButCount);
      std::memset(JButOld, 0, sizeof(BYTE) * JButCount);
    } /* End of 'joystick' function */

    /* Default destuctor */
    ~joystick( VOID )
    {
    } /* End of '~joystick' function */

    /* Response of joystick function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( VOID )
    {
      /* Special define for get joystick axis */
#define IPGL_GET_JOYSTIC_AXIS(A) \
  2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / (jc.w ## A ## max - jc.w ## A ## min) - 1

      // Get status about connected joysticks
      // If count of devs > 0, then we need
      // response data of joys devs.
      if (joyGetNumDevs() > 0)
      {
        JOYCAPS jc;

        /* Get joystick info */
        if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
        {
          JOYINFOEX ji = { 0 };

          ji.dwSize = sizeof(JOYINFOEX);
          ji.dwFlags = JOY_RETURNALL;

          if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
          {
            /* Buttons */
            for (INT i = 0; i < 32; i++)
            {
              JBut[i] = (ji.dwButtons >> i) & 1;
              JButClick[i] = JBut[i] && !JButOld[i];
              JButOld[i] = JBut[i];
            }

            /* Axes */
            JX = IPGL_GET_JOYSTIC_AXIS(X);
            JY = IPGL_GET_JOYSTIC_AXIS(Y);
            JZ = IPGL_GET_JOYSTIC_AXIS(Z);
            JR = IPGL_GET_JOYSTIC_AXIS(R);

            /* Point of view */ 
            JPov = ji.dwPOV == 0xFFFF ? -1 : ji.dwPOV / 4500;
          }
        }
      }
#undef IPGL_GET_JOYSTIC_AXIS

    } /* End of 'Response' function */

  }; /* End of 'joystick' class */
} /* end of 'pivk' file */

#endif // !__joystick_h_

/* END OF 'joystick.h' FILE */
