/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        render.h
 * PROGRAMMER:  IP5
 * LAST UPDATE: 28.10.2024
 * PURPOSE:     Render core header file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#ifndef __render_h_
#define __render_h_

#include "res/image.h"
#include "res/model.h"
#include "res/target.h"
#include "res/markers.h"

/* Base project namespace */
namespace pivk
{
  /* Declare anim class for referencing in render class */
  class anim;

  /* Render class */
  class render : 
    public primitive_manager, public material_manager, public texture_manager, public buffer_manager, public shader_manager, public model_manager , public pipeline_pattern_manager, public marker_manager
  {
  private:
    HWND &hWnd;              // Window context
    FLT ProjSize;            // Proj size
    FLT ProjFarClip;         // Proj far clip
    INT 
      FrameW, FrameH;        // Frame size
  public:
    vlk VulkanCore;          // Vulkan core
    anim &AnimRef;           // Animation reference
    vlk_buf *CamBuf;         // Camera buffer
    vlk_buf *PrBuf;          // Primitive buffer
    vlk_buf *SyncBuf;        // Sync buffer
    camera Camera;           // Camera of render
    HDC hDC;                 // Program context

    /* Structure for storage ubo data of camera */
    struct CAMERA_BUF
    {
      matr MatrVP;              // Premultipled view and projection matrixes
      matr MatrV;               // Maxtrix view
      matr MatrP;               // Matrix projection
      vec4 CamLoc4;             // Camera location
      vec4 CamRight4;           // Camera right direction
      vec4 CamUp4;              // Camera up direction
      vec4 CamDir4;             // Camera forward direction
      vec4 CamAt4;              // Cam at direction
      vec4 FrameWHProjSizeDist; // Camera frame size, proj. size and dist
      vec4 LightDir4;           // Light direction source
      vec4 LightColor4;         // Light color
      matr MatrShadow;          // Shadow map matrix
    };

    /* Structure for storage sync ubo data */
    struct SYNC_BUF
    {
      vec4 TimeDTGlobalTimeDT;    // Time parameters
      vec4 IsWireFrameIsPause[4]; // WireFrame, pause, FPS, ... flags
    };

    /* Struct for push constants data */
    struct DRAW_BUF
    {
      matr MatrW, MatrWVP, MatrWInv; // World matrix
      ivec4 Id;                      // Array with id
      fvec4 MarkersData[3];          // Markers data
    };

    /* Render constructor
     * ARGUMENTS:
     *   - window handle reference:
     *       HWND &hNewWnd;
     *   - animation reference:
     *       anim &Ani;
     */
    render( HWND &hNewWnd, anim &Ani ) : 
      hWnd(hNewWnd), ProjSize(0.1f), ProjFarClip(1000.0f), FrameW(47), FrameH(47), Camera(),
      primitive_manager(*this), material_manager(*this), texture_manager(*this), buffer_manager(*this), shader_manager(*this), model_manager(*this), pipeline_pattern_manager(*this), marker_manager(*this),
      AnimRef(Ani), VulkanCore(hNewWnd)
    {
    } /* End of 'render' function */

    /* Default destructor */
    ~render( VOID )
    {
    } /* End of '~render' function */

    /* Init render function.
     * ARGUMENTS:
     *   - new window handle:
     *       HWND hNewWnd;
     * RETURNS: None.
     */
    VOID Init( HWND hNewWnd )
    {
      UINT nums = 0;

      render::hWnd = hNewWnd;
      //hDC = GetDC(hWnd);

      VulkanCore.Init();

      /*** Render parametrs setup ***/
      render::Camera.SetLocAtUp(vec3(20.0f), vec3(0.0f));

      // Create default buffers
      //PrBuf = VulkanCore.BufCreate(sizeof(PRIM_BUF), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT); //BufCreate<PRIM_BUF>("Primitive buffer", BufferBindPrim);
      //PrBuf->Apply();
      SyncBuf = VulkanCore.BufCreate(sizeof(SYNC_BUF), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);//BufCreate<SYNC_BUF>("Sync buffer", BufferBindSync);
      //SyncBuf->Apply();
      CamBuf = VulkanCore.BufCreate(sizeof(CAMERA_BUF), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);//BufCreate<CAMERA_BUF>("Camera buffer", BufferBindCamera);
      //CamBuf->Apply();

      TextureInit();
      //shader_manager::ShdCreate("Default");
      MtlInit();
      PrimInit();
      MarkersInit();
    } /* End of 'Init' function */

    /* DeInit render function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Close( VOID )
    {
      // Close shaders
      MarkersClose();
      ShdClose();

      // Buffers delete
      material_manager::MaterialSSBO->Free();

      //PrBuf->Free();
      SyncBuf->Free();
      CamBuf->Free();

      // Delete vulkan core
      VulkanCore.Close();
      //ReleaseDC(hWnd, hDC);
    } /* End of 'Close' function */

    /* Update camera buffer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID UpdateCameraBuf( VOID )
    {
      CAMERA_BUF Buf {};
      Buf.CamAt4 = vec4(Camera.At, 0);
      Buf.CamDir4 = vec4(Camera.Dir, 0);
      Buf.CamLoc4 = vec4(Camera.Loc, 0);
      Buf.CamRight4 = vec4(Camera.Right, 0);
      Buf.CamUp4 = vec4(Camera.Up, 0);
      Buf.FrameWHProjSizeDist = vec4(Camera.FrameW, Camera.FrameH, Camera.ProjDist, Camera.ProjDist);
      Buf.LightColor4 = vec4(1);
      Buf.LightDir4 = vec4(Camera.Dir, 0);
      Buf.MatrP = Camera.Proj;
      Buf.MatrShadow = matr::Identity();
      Buf.MatrV = Camera.View;
      Buf.MatrVP = Camera.VP;
      CamBuf->Write(&Buf);
      VulkanCore.UpdateDescriptor(CamBuf, BufferBindCamera);
    } /* End of 'UpdateCameraBuf' function */

    /* Resize function.
     * ARGUMENTS:
     *   - new size of window:
     *       INT NewW, NewH;
     * RETURNS: None.
     */
    VOID Resize( INT NewW, INT NewH )
    {
      FrameH = NewH;
      FrameW = NewW;
      VulkanCore.Resize(NewW, NewH);
      Camera.Resize(FrameW, FrameH);

      UpdateCameraBuf();
      //VulkanCore.UpdateDescriptor();
      //CamBuf->Update(&Buf);
    } /* End of 'Resize' function */

    /* Frame start function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */    
    VOID FrameStart( VOID )
    {
      /* Update buffer with materials */
      material_manager::MtlUpdateSSBO();

      /* Update SYNC buffer */
      SYNC_BUF Buf {};
      SyncBuf->Write(&Buf);
      VulkanCore.UpdateDescriptor(SyncBuf, BufferBindSync);

      VulkanCore.RenderStart();

      // Bind buffers
      VulkanCore.BindDescriptor(BufferBindCamera);
      VulkanCore.BindDescriptor(BufferBindSync);
      VulkanCore.BindDescriptor(BufferBindMaterial);
      VulkanCore.BindDescriptor(BufferBindPrim);
      // TODO: shd update
      //ShdUpdate(); NOT USED
    } /* End of 'FrameStart' function */

    /* Frame end function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID FrameEnd( VOID )
    {
      VulkanCore.RenderEnd();
      //SwapBuffers(hDC);
    } /* End of 'FrameEnd' function */

    /* Primitive draw function.
     * ARGUMENTS:
     *   - primitive pointer:
     *       prim *Pr;
     *   - transformation matrix:
     *       const matr &World;
     *   - array with subdata for drawing:
     *       const std::array<fvec4, 4> *SubData;
     * RETURNS: None.
     */
    VOID Draw( const prim *Pr, const matr &World = matr::Identity(), const std::array<fvec4, 4> *SubData = nullptr );// const;

  }; /* End of 'render' class */

} /* end of 'pivk' namespace */

#endif // !__render_h_

/* END OF 'render.h' FILE */
