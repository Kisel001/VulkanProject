/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        main.cpp
 * PROGRAMMER:  IP5
 * LAST UPDATE: 28.10.2024
 * PURPOSE:     Main project startup file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#include "pivk.h"

/* Windows desktop application main function.
 * ARGUMENTS:
 *   - current instance:
 *       HINSTANCE hInstance;
 *   - prev instance (not used):
 *       HINSTANCE hPrevInstance;
 *   - command line string:
 *       CHAR *CmdLine;
 *   - show cmd parameter:
 *       INT ShowCmd;
 * RETURNS:
 *   (INT) application exit code.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT CmdShow )
{
  /* Init animation function */
  //pivk::anim &Ani = pivk::anim::GetRef();

#ifdef T53VLK_DEBUG_MODE
  // Enable debug console for DEBUG mode.
  pivk::console Con;
  Con.InitConsole();
#endif // T53VLK_DEBUG_MODE

  std::vector<std::thread> Ths;
  //
  for (INT i = 0; i < 1; i++)
    Ths.push_back(std::thread([hInstance]( VOID )
    {
      pivk::anim MyAnim;
      MyAnim.CreateClass();
      MyAnim.WindowCreate("CGSG IP5 :: ANIM Project");
 
      MyAnim << "Triangle";
      //MyAnim << "Model";
      //MyAnim << "Sky";
      ////MyAnim << "Map";
      MyAnim << "Control";
      //MyAnim << "Control" << new my_unit(&MyAnim1);
      MyAnim.Run();
    }));
  for (auto &t : Ths)
    t.join();

  //Ths.push_back(std::thread([hInstance](VOID)
  //{
  //  pivk::anim MyAnim;
  //  MyAnim.CreateClass();
  //  MyAnim.WindowCreate("CGSG IP5 :: ANIM Project");
  //
  //  MyAnim << "Model";
  //  MyAnim << "Sky";
  //  //MyAnim << "Map";
  //  MyAnim << "Control";
  //  //MyAnim << "Control" << new my_unit(&MyAnim1);
  //  MyAnim.Run();
  //}));
  //
  //
  //Ths[0].detach();
  //
  //time_t t = time(nullptr);
  //
  //while (t + 10 > time(nullptr))
  //  ;
  //
  //Ths.push_back(std::thread([]( VOID ) -> VOID
  //{
  //  pivk::anim MyAnim;
  //  MyAnim.CreateClass();
  //  MyAnim.WindowCreate("CGSG IP5 :: ANIM Project");
  //
  //  MyAnim << "Model";
  //  MyAnim << "Sky";
  //  //MyAnim << "Map";
  //  MyAnim << "Control";
  //  //MyAnim << "Control" << new my_unit(&MyAnim1);
  //  MyAnim.Run();
  //}));
  //
  //Ths[1].join();


  //Th = std::
  // Load unit samples
  //Ani << "Model";
  //Ani << "Sky";
  ////Ani << "Map";
  //
  //// Add control unit
  //Ani << "Control";
  //
  //Ani.Run();

  return 30;
} /* End of 'WinMain' function */

/* END OF 'main.cpp' FILE */
