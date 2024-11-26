/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        u_ctrl.h
 * PROGRAMMER:  IP5
 * LAST UPDATE: 13.09.2024
 * PURPOSE:     Control unit header file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#include "pivk.h"

/* Base project namespace */
namespace pivk
{
  /* Unit control class */
  class unit_control : public unit
  {
    /* Image Block */
    INT ImgW, ImgH;    /* Logo image size */
    HBITMAP hImgBm;    /* Logo image bitmap handle */
    HDC hImgDC;        /* Logo image device context */
    DWORD *Img;        /* Logo image pixels */

    /* Text drawing sample */
    HFONT hTextFnt;    /* Text font */
    HDC hTextDC;       /* Text device context */
    HBITMAP hTextBm;   /* Text draw bitmap */
    DWORD *TextBits;   /* Text pixel bits */
    SIZE TextSize;     /* Text size */
    texture *Tex;      /* Texture */
    shader *Prog;      /* Shader program */
    material *Mtl;     /* Material for texture of poster */
    prim *Pr;          /* Primitive for poster */

  public:
    /* Unit control constructor 
     * ARGUMENTS:
     *   - animation reference:
     *       anim &A;
     */
    unit_control( anim &A )
    {
      //material mtl;
      //
      ///* Poster */
      //mtl = A.MtlGetDef();
      //Prog = A.ShdCreate("poster");
      //mtl.Name = "poster";
      //mtl.Tex[0] = A.TextureCreateFromImg("Poster", 1, 1, 4, NULL);
      //std::vector<vertex::std> V;
      //V.resize(4);
      ////V.
      //topology::base<vertex::std> S = {prim_type::STRIP, V, {}};
      //Pr = A.PrimCreate<vertex::std>({prim_type::STRIP, {}, {}}, 4);
      ////Uni->Pr.MtlNo = IP5_RndMtlAdd(&mtl);
      ////this->Pr->Mtl = A.MtlAdd(mtl);
      //Mtl = Pr->Mtl = A.MtlAdd(mtl);
      //Mtl->Shd = Prog;
      //
      //hTextDC = CreateCompatibleDC(A.hDC);
      //hTextBm = NULL;
      //hTextFnt = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
      //                      RUSSIAN_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
      //                      DEFAULT_QUALITY, VARIABLE_PITCH | FF_ROMAN, "Bookman Old Style");
      //SelectObject(this->hTextDC, this->hTextFnt);
      //SelectObject(this->hTextDC, GetStockObject(BLACK_BRUSH));
      //SelectObject(this->hTextDC, GetStockObject(NULL_PEN));
      //SetBkMode(this->hTextDC, TRANSPARENT);
      //SetTextColor(this->hTextDC, RGB(255, 255, 255));

      //this->Mtl = A.MtlAdd("poster");
    } /* End of 'unit' function */

    /* Default destructor */
    ~unit_control() override
    {
      //DeleteDC(this->hTextDC);
      //if (this->hImgBm != nullptr)
      //  DeleteObject(this->hImgBm);
      //if (this->hImgDC != nullptr)
      //  DeleteDC(this->hImgDC);
      //DeleteObject(this->hTextFnt);
    } /* End of '~unit' function */

    /* Unit response function.
     * ARGUMENTS:
     *   - animation reference:
     *       const pivk::anim &A;
     * RETURNS: NONE.
     */
    VOID Response( anim &A ) override
    {
      FLT
        Dist = !(A.Camera.At - A.Camera.Loc),
        cosT = (A.Camera.Loc.Y - A.Camera.At.Y) / Dist,
        sinT = sqrt(1 - cosT * cosT),
        plen = Dist * sinT,
        cosP = (A.Camera.Loc.Z - A.Camera.At.Z) / plen,
        sinP = (A.Camera.Loc.X - A.Camera.At.X) / plen,
        Elevator = R2D(atan2(sinT, cosT)),
        Azimuth = R2D(atan2(sinP, cosP)),
        Wp, Hp,
        sx, sy,
        twist = -100, /* Special coefficient */
        dspeed = 10,
        changeY;
      vec3 NewLoc, dv;

      /***
       * Light dir control
       ***/
      if (A.Keys[VK_SHIFT] && A.KeysClick['I'])
        A.Camera.Dir.X -= 0.1 * A.GlobalDeltaTime;
      if (A.Keys[VK_SHIFT] && A.KeysClick['K'])
        A.Camera.Dir.X += 0.1 * A.GlobalDeltaTime;
      if (A.Keys[VK_SHIFT] && A.KeysClick['L'])
        A.Camera.Dir.Z -= 0.1 * A.GlobalDeltaTime;
      if (A.Keys[VK_SHIFT] && A.KeysClick['J'])
        A.Camera.Dir.Z += 0.1 * A.GlobalDeltaTime;

      changeY =
         (dspeed * A.GlobalDeltaTime) *
        ((A.Keys[VK_NUMPAD8] - A.Keys[VK_NUMPAD2]) +
          29 * A.Keys[VK_CONTROL] *
         (A.Keys[VK_NUMPAD8] - A.Keys[VK_NUMPAD2]));

      A.Camera.Loc.Y += changeY;
      A.Camera.At.Y += changeY;

      if (A.Keys[VK_LCONTROL] && A.KeysClick['F'])
      {
        A.Camera.Loc = vec3(100);
        A.Camera.At = vec3(0);
        A.Camera.Up = vec3(0, 1, 0);
        A.Camera.Dir = vec3(1, 1, 1);
        A.Camera.SetLocAtUp(A.Camera.Loc, A.Camera.At, A.Camera.Up);
        A.UpdateCameraBuf();

        return;
      }
      else
      {
        /*** Shift angles ***/
        Azimuth += A.GlobalDeltaTime *
          (twist * A.Keys[VK_LBUTTON] * A.Mdx +           // Mouse
           twist * (A.Keys[VK_LEFT] - A.Keys[VK_RIGHT])); // Keyboard

        Elevator += A.GlobalDeltaTime *
          (twist * A.Keys[VK_LBUTTON] * A.Mdy +           // Mouse
           twist * (A.Keys[VK_UP] - A.Keys[VK_DOWN]));    // Keyboard

        Dist += A.GlobalDeltaTime *
          (-dspeed * A.Mdz +                              // Mouse
             dspeed * (1 + A.Keys[VK_SHIFT] * 29) *
               (A.Keys[VK_NEXT] - A.Keys[VK_PRIOR]));     // Keyboard

        // Check, that Elevator >= 0.08 and <= +178
        if (Elevator < 0.08)
          Elevator = 0.08;
        else if (Elevator > 178.0)
          Elevator = 178.0;

        // Check, that dist >= 0.001
        if (Dist < 0.001)
          Dist = 0.001;

        NewLoc = (matr::RotateX(Elevator) * matr::RotateY(Azimuth)).TransformPoint(vec3(0, Dist, 0));
        NewLoc = A.Camera.At + NewLoc;

        // Get projection
        Wp = A.Camera.Size;
        Hp = A.Camera.Size;

        if (A.W > A.H)
          Wp *= (FLT)A.W / A.H;
        else
          Hp *= (FLT)A.H / A.W;

        // Get axis shifts
        sx = A.Keys[VK_RBUTTON] * -A.Mdx * Wp / A.W * Dist / A.Camera.ProjDist;
        sy = A.Keys[VK_RBUTTON] *  A.Mdy * Hp / A.H * Dist / A.Camera.ProjDist;

        dv = (A.Camera.Right * sx) + (A.Camera.Up * sy);
        A.Camera.At = A.Camera.At + dv;
        A.Camera.Loc = A.Camera.Loc + dv;
      }

      A.Camera.SetLocAtUp((matr::RotateX(Elevator) *
                           matr::RotateY(Azimuth) *
                           matr::Translate(A.Camera.At)).TransformPoint(vec3(0, Dist, 0)),
                           A.Camera.At,
                           vec3(0, 1, 0));
      A.UpdateCameraBuf();

      //anim::CAMERA_BUF Buf {};
      //Buf.CamAt4 = vec4(A.Camera.At, 0);
      //Buf.CamDir4 = vec4(A.Camera.Dir, 0);
      //Buf.CamLoc4 = vec4(A.Camera.Loc, 0);
      //Buf.CamRight4 = vec4(A.Camera.Right, 0);
      //Buf.CamUp4 = vec4(A.Camera.Up, 0);
      //Buf.FrameWHProjSizeDist = vec4(A.W, A.H, A.Camera.ProjDist, A.Camera.ProjDist);
      //Buf.LightColor4 = vec4(1);
      //Buf.LightDir4 = vec4(A.Camera.Dir, 0);
      //Buf.MatrP = A.Camera.Proj;
      //Buf.MatrShadow = matr::Identity();
      //Buf.MatrV = A.Camera.View;
      //Buf.MatrVP = A.Camera.VP;
      //A.CamBuf->Write(&Buf);
      //A.VulkanCore.UpdateDescriptor(A.CamBuf, BufferBindCamera);
    } /* End of 'Response' function */

    /* Unit render function.
     * ARGUMENTS:
     *   - animation reference:
     *       const pivk::anim &A;
     * RETURNS: NONE.
     */
    VOID Render( pivk::anim &A ) override
    {
      //static DBL OldTime = -2;
      ////CHAR buf1[200];
      //CHAR buf[1000];
      //BITMAPINFOHEADER bmih = {0};
      //RECT rc = {0};
      //matr m;
      //INT x, y;
      //vec4 KaKd[2];

      //IP5_RndPrimDraw(&this->DebugPr, matr::Identity());

      //sprintf(buf1, "Hello from St.Peterburg!\n"
      //        "FPS: %.2f, RTM: %.2f\n"
      //        "Current time: %.2f",
      //        A.FPS, A.FPS / 5, A.Time);

      //IP5_RndFntDraw(buf1, VecSet(-5, 100, 0), 30.0, MatrIdentity());

      //if (this->hTextBm != NULL)
      //  DeleteObject(this->hTextBm);
      //
      //x = sprintf(buf, /*"CGSG'Jr'2023 Animation Project * WinCamp 2024\n"*/
      //            "FPS: %.2f, RTM: %.2f\n"
      //            "CT: %.2f\n"
      //            "POS: (%.2f %.2f %.2f)\n"
      //            "WHL: %.2f\n",
      //            A.FPS, A.FPS / 5, A.Time, A.Pos.X, A.Pos.Y, A.Pos.Z, A.Wheel);
      //rc.left = 0;
      //rc.top = 0;
      //rc.right = 10;
      //rc.bottom = 10;
      //DrawText(this->hTextDC, buf, x, &rc, DT_CALCRECT);
      //this->TextSize.cx = rc.right;
      //this->TextSize.cy = rc.bottom;
      //
      //bmih.biSize = sizeof(BITMAPINFOHEADER);
      //bmih.biPlanes = 1;
      //bmih.biBitCount = 32;
      //bmih.biCompression = BI_RGB;
      //bmih.biWidth = this->TextSize.cx;
      //bmih.biHeight = -this->TextSize.cy;
      //bmih.biSizeImage = this->TextSize.cx * 4 * this->TextSize.cy;
      //this->hTextBm = CreateDIBSection(NULL, (BITMAPINFO *)&bmih, DIB_RGB_COLORS,
      //                                 (VOID **)&this->TextBits, NULL, 0);
      //SelectObject(this->hTextDC, this->hTextBm);
      //Rectangle(this->hTextDC, 0, 0, this->TextSize.cx, this->TextSize.cy);
      //DrawText(this->hTextDC, buf, x, &rc, 0);
      //
      //for (y = 0; y < this->TextSize.cy; y++)
      //  for (x = 0; x < this->TextSize.cx; x++)
      //  {
      //    BYTE a = (this->TextBits[y * this->TextSize.cx + x] >> 8) & 0x255;
      //
      //    if (a != 0)
      //      this->TextBits[y * this->TextSize.cx + x] |= 0xFF << 24;
      //    else
      //      this->TextBits[y * this->TextSize.cx + x] |= 0x47 << 24;
      //  }
      //
      //m = matr::Ortho(0, A.W, A.H, 0, -1, 1);
      //
      ///* Draw posters */
      //UINT *texid = &A.TextureFind("Poster")->TexId;
      //this->Mtl->Ka.X = 0;
      //this->Mtl->Ka.Y = 0;
      //this->Mtl->Kd.X = this->TextSize.cx;
      //this->Mtl->Kd.Y = this->TextSize.cy;
      //KaKd[0] = vec4(this->Mtl->Ka, 0);
      //KaKd[1] = vec4(this->Mtl->Kd, 0);
      //
      ////A.BufUpdate(this->Mtl->BufferMtl->Name, KaKd);
      ////A.BufUpdate<vec4>("")
      ////IP5_RndUBOUpdate(this->Mtl->UBONo, 0, sizeof(VEC4) * 2, KaKd);
      //A.Draw(this->Pr, m);

    } /* End of 'Render' function */

  }; /* End of 'unit_control' class */

  static anim::unit_register<unit_control> _UnitControlInitSecurity("Control");
} /* end of 'pivk' namespace */

