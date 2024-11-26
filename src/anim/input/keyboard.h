/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        keyboard.h
 * PROGRAMMER:  IP5
 * LAST UPDATE: 30.07.2024
 * PURPOSE:     Keyboard header file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#ifndef __keyboard_h_
#define __keyboard_h_

#include "def.h"

/* Base project namespace */
namespace pivk
{
  /* Total count of keys */
  inline constexpr UINT KeysCount = 256;

  /* Keyboard class */
  class keyboard
  {
  private:
    BYTE KeysOld[KeysCount];   // Old keys data

  public:
    BYTE Keys[KeysCount];      // Keys data
    BYTE KeysClick[KeysCount]; // Keys click data

    enum struct layout
    {
      ENGLISH_LAYOUT, // English layout
      RUSSIAN_LAYOUT  // Russian layout
    } Layout;                  // Current keyboard layout

    BOOL IsCapsActive;         // Status of activing caps lock

    /* Default constructor */
    keyboard( VOID ) : Layout(layout::ENGLISH_LAYOUT), IsCapsActive(FALSE)
    {
      /* Fill arrays with keys by zero value */
      std::memset(KeysOld, 0, KeysCount * sizeof(BYTE));
      std::memset(Keys, 0, KeysCount * sizeof(BYTE));
      std::memset(KeysClick, 0, KeysCount * sizeof(BYTE));
    } /* End of 'keyboard' function */

    /* Default destuctor */
    ~keyboard( VOID )
    {
    } /* End of '~keyboard' function */

    /* Response of keyboard function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( VOID )
    {
      // Layout state
      if (INT kbl = LOWORD(GetKeyboardLayout(0)); kbl == 1049)
        Layout = layout::RUSSIAN_LAYOUT;
      else
        Layout = layout::ENGLISH_LAYOUT;

      // Caps active state
      IsCapsActive = GetKeyState(VK_CAPITAL) & 1;

      memcpy(KeysOld, Keys, KeysCount * sizeof(BYTE));

      // Get keyboard state
      GetKeyboardState(Keys);

      for (INT i = 0; i < KeysCount; ++i)
      {
        Keys[i] >>= 7;
        KeysClick[i] = (Keys[i] && !KeysOld[i]);
      }
    } /* End of 'Response' function */

  }; /* End of 'keyboard' class */
} /* end of 'pivk' file */

#endif // !__keyboard_h_

/* END OF 'keyboard.h' FILE */
