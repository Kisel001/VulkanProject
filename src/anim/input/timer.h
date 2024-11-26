/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        timer.h
 * PROGRAMMER:  IP5
 * LAST UPDATE: 31.07.2024
 * PURPOSE:     Timer header file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#ifndef __timer_h_
#define __input_h_

#include "def.h"

/* Base project namespace */
namespace pivk
{
  /* Timer class */
  class timer
  {
  private:
    /* Local variables */
    UINT64
      StartTime,                   // Start program time
      OldTime,                     // Previous frame time
      OldTimeFPS,                  // Old time FPS measurement
      PauseTime,                   // Time during pause period
      TimePerSec,                  // Timer resolution
      FrameCounter;                // Frames counter

  public:
    /* Global variables */
    DBL
      GlobalTime, GlobalDeltaTime, // Global time and interframe interval
      Time, DeltaTime,             // Time with pause and interframe interval
      FPS;                         // Frames per second value
    BOOL
      IsPause;                     // Pause flag

    /* Default constructor */
    timer() : 
      FPS(30.0), PauseTime(0), FrameCounter(0), IsPause(FALSE),
      Time(0.0), DeltaTime(0.0), GlobalTime(0.0), GlobalDeltaTime(0.0)
    {
      LARGE_INTEGER t;

      QueryPerformanceFrequency(&t);
      TimePerSec = t.QuadPart;
      QueryPerformanceCounter(&t);
      StartTime = OldTime = OldTimeFPS = t.QuadPart;
    } /* End of 'timer' function */

    /* Default destructor */
    ~timer()
    {
    } /* End of '~timer' function */

    /* Timer response function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( HWND hWnd )
    {
      LARGE_INTEGER t;

      QueryPerformanceCounter(&t);

      /* Global time */
      GlobalTime = (DBL)(t.QuadPart - StartTime) / TimePerSec;
      GlobalDeltaTime = (DBL)(t.QuadPart - OldTime) / TimePerSec;

      /* Time with pause */
      if (IsPause)
      {
        DeltaTime = 0;
        PauseTime += t.QuadPart - OldTime;
      }
      else
      {
        DeltaTime = GlobalDeltaTime;
        Time = (DBL)(t.QuadPart - PauseTime - StartTime) / TimePerSec;
      }

      /* FPS */
      FrameCounter++;
      if (t.QuadPart - OldTimeFPS > TimePerSec)
      {
        FPS = FrameCounter * TimePerSec / (DBL)(t.QuadPart - OldTimeFPS);
        OldTimeFPS = t.QuadPart;
        FrameCounter = 0;

        static CHAR Buf[100];

        sprintf(Buf, "%lf", FPS);

        SetWindowText(hWnd, Buf);
      }
      OldTime = t.QuadPart;
    } /* End of 'Response' function */

  }; /* End of 'timer' class */
} /* end of 'pivk' file */

#endif // !__timer_h_

/* END OF 'timer.h' FILE */
