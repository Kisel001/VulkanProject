/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

 /* FILE:        image.h
  * PROGRAMMER:  IP5
  * LAST UPDATE: 13.09.2024
  * PURPOSE:     Image class header file.
  * NOTE:        None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum.
  */

#ifndef __image_h_
#define __image_h_

#include "res.h"

/* Base project namespace */
namespace pivk
{
  /* Image type enum type */
  enum struct img_type
  {
    G24, G32 // Types of image
  }; /* End of 'img_type' enum */

  /* Image class */
  class image
  {
  public:
    DWORD *Pixels; // Image pixels data.
    INT
      W, H;        // Size of image.

    /* Image constructor */
    image( std::string Filename, img_type Type = img_type::G32 ) : Pixels(nullptr), W(0), H(0)
    {
      FILE *F;
      DWORD *dest;
      BYTE r, g, b, a;
 
      if ((F = fopen(Filename.c_str(), "rb")) == nullptr)
        return;

      fread(&W, 2, 1, F);
      fread(&H, 2, 1, F);

      Pixels = new DWORD[W * H * sizeof(DWORD)];

      switch (Type)
      {
      case img_type::G24: /* G24 */
        dest = Pixels;

        for (INT i = 0; i < W * H; ++i)
        {
          b = fgetc(F);
          g = fgetc(F);
          r = fgetc(F);

          *dest++ = COM_MAKELONG0123(b, g, r, 0xFF);
        }

        break;

      case img_type::G32: /* G32 */
        dest = Pixels;

        for (INT i = 0; i < W * H; ++i)
        {
          b = fgetc(F);
          g = fgetc(F);
          r = fgetc(F);
          a = fgetc(F);

          *dest++ = COM_MAKELONG0123(b, g, r, a);
        }

        break;

      default: /* Invalid format */
        W = 0;
        H = 0;
        delete[] Pixels;
        Pixels = nullptr;
        fclose(F);
        return;
      }
    } /* End of 'image' function */

    /* Get pointer to image data function
     * ARGUMENTS: None.
     * RETURNS:
     *   (DWORD *) pointer to image.
     */
    DWORD * GetPtrToImgData()
    {
      return Pixels;
    } /* End of 'GetPtrToImgData' function */

    ~image()
    {
      if (Pixels != nullptr)
        delete[] Pixels, Pixels = nullptr;
      W = 0;
      H = 0;
    }
  };
} /* end of 'pivk' namespace */

#endif // !__image_h_

/* END OF 'image.h' FILE */
