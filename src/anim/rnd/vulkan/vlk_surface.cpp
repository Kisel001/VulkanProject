/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        vlk_surface.cpp
 * PROGRAMMER:  IP5
 * LAST UPDATE: 28.10.2024
 * PURPOSE:     Vulkan core surface file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#include "pivk.h"

/* Base project namespace */
namespace pivk
{
  /* Create surface of vulkan core function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID vlk::CreateSurface( VOID )
  {
#ifdef T53_WINDOWS_PLATFROM
    VkWin32SurfaceCreateInfoKHR SurfaceCreateInfo 
    {
      .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,   // Type of surface.
      .flags = 0,                                                 // Flags for surface.
      .hinstance = GetModuleHandle(nullptr),                      // Handle of copy of application.
      .hwnd = hWndRef,                                            // Handle of window, to which to link Vulkan.
    };

    if (vkCreateWin32SurfaceKHR(Instance, &SurfaceCreateInfo, nullptr, &Surface) != VK_SUCCESS)
    {
      MessageBox(hWndRef, "Error of creating vulkan surface", "T53VLK :: ERROR", MB_OK | MB_ICONERROR);
      std::exit(3);
    }
#endif
  } /* End of 'vlk::CreateSurface' function */

} /* end of 'pivk' namespace */

/* END OF 'vlk_surface.cpp' FILE */
