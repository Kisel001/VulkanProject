/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        console.h
 * PROGRAMMER:  IP5
 * LAST UPDATE: 29.10.2024
 * PURPOSE:     Utils for work with console header file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#ifndef __console_h_
#define __console_h_

#include "def.h"

/* Base project namespace */
namespace pivk
{
  /* Console class */
  class console
  {
  public:
    /* Type fo color */
    enum color
    {
      BLACK = 0x0,        // Black color
      BLUE = 0x1,         // Blue color
      GREEN = 0x2,        // Green color
      CYAN = 0x3,         // Cyan color
      RED = 0x4,          // Red color
      PURPLE = 0x5,       // Purple color
      YELLOW = 0x6,       // Yellow color
      WHITE = 0x7,        // White color
      GREY = 0x8,         // Grey color
      LIGHT_BLUE = 0x9,   // Light blue color
      LIGHT_GREEN = 0xA,  // Light green color
      LIGHT_CYAN = 0xB,   // Light cyan color
      LIGHT_RED = 0xC,    // Light red color
      LIGHT_PURPLE = 0xD, // Light purple color
      LIGHT_YELLOW = 0xE, // Light yellow color
      LIGHT_WHITE = 0xF,  // Light white color
    }; /* End of 'color' enum */

    /* Default constructor */
    console()
    {
    } /* End of 'console' function */

    /* Default destructor */
    ~console()
    {
    } /* End of '~console' function */

    /* Get console color function.
     * ARGUMENTS:
     *   - color of text:
     *       color TextColor;
     *   - color of background:
     *       color BackgroundColor;
     * RETURNS:
     *   (UINT) Color id.
     */
    static UINT GetConsoleColor( color TextColor = color::WHITE, color BackgroundColor = color::BLACK )
    {
      return UINT(TextColor) | (UINT(BackgroundColor) << 4);
    } /* End of 'GetConsoleColor' function */

    /* Init console function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID InitConsole( VOID ) const
    {
#ifdef T53VLK_DEBUG_MODE
      AllocConsole();
      SetConsoleTitleA("CGSG IP5 :: T53VLK :: Debug console");
      HWND hCnsWnd = GetConsoleWindow();
      RECT rc;
      GetWindowRect(hCnsWnd, &rc);
      MoveWindow(hCnsWnd, 102, 0, 500, 300, TRUE);
      std::freopen("CONOUT$", "w", stdout);
      //std::setlocale(LC_CTYPE, "ru-ru");
      //std::system("@chcp 1251 > nul");
      //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
#endif
    } /* End of 'InitConsole' function */

    /* Set console color function.
     * ARGUMENTS:
     *   - color id:
     *       UINT Color;
     * RETURNS: None.
     */
    VOID SetConsoleColor( UINT Color ) const
    {
#ifdef T53VLK_DEBUG_MODE
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Color);
#endif
    } /* End of 'SetConsoleColor' function */

  }; /* End of 'console' class */

  /* Console function */
  inline console Console {};

} /* end of 'pivk' namespace */

#endif // !__console_h_

/* END OF 'console.h' FILE */
