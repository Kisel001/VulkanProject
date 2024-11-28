/*************************************************************
 * Copyright (C) 2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : markers.h
 * PURPOSE     : Animation pattern on Vulkan API project.
 *               Markers class declaration module.
 * PROGRAMMER  : CGSG'Sr'2023.
 *               IP5.
 * LAST UPDATE : 26.11.2024.
 * NOTE        : Module namespace 'pivk'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __markers_h_
#define __markers_h_

//#include "res.h"
#include "shader.h"
//#include "../vulkan/vlk_core.h"

/* Project namespace */
namespace pivk
{
  /* Forward reference */
  class render;

  /* Shader manager */
  class marker_manager
  {
  protected:
    // Reference to render
    render &RndRef;

  private:
    // Spheres store stock (Center/Radius,Color)
    std::vector<std::tuple<vec4, vec4>> Spheres;

    // Cylinder store stock (P0/Radius,P1/Radius,Color)
    std::vector<std::tuple<vec4, vec4, vec4>> Cylinders;

    // Sphere/cylinder shaders
    shader *Shds[2];

    // Sphere/cylinder pipelines and layout
    VkPipeline Pipelines[2] {};
    VkPipelineLayout PipelineLayouts[2] {};

    vlk_buf *VertexBuffer;                     // Vertex buffer
    //VkBuffer VertexBuffer {};                // Vertex buffer (dummy)
    //VmaAllocation VertexBufferAllocation {}; // VMA vertex buffer allocation info (dummy)

  public:
    /* Class constructor.
    * ARGUMENTS:
    *   - render instance reference:
    *       render &Rnd;
    */
    marker_manager( render &Rnd ) : RndRef(Rnd)
    {
    } /* End of 'marker_manager' function */

      /* Class destructor */
    ~marker_manager( void )
    {
      Close();
    } /* End of '~marker_manager' function */

      /* Markers initialization funtion.
      * ARGUMENTS: None.
      * RETURNS: None.
      */
    VOID Init( VOID );

    /* Markers deinitialization funtion.
    * ARGUMENTS: None.
    * RETURNS: None.
    */
    VOID Close( VOID );

    /* Draw all markers function.
    * ARGUMENTS: None.
    * RETURNS: None.
    */
    VOID DrawAllMarkers( VOID );

    /* Draw sphere marker function.
    * ARGUMENTS:
    *   - sphere center point:
    *       const vec3 &C;
    *   - sphere radius:
    *       FLT R;
    *   - sphere color:
    *       const vec4 &Color;
    * RETURNS: None.
    */
    VOID DrawSphere( const vec3 &C, FLT R, const vec4 &Color )
    {
      Spheres.push_back({{C, R}, Color});
    } /* End of 'DrawSphere' function */

      /* Draw cylinder marker function.
      * ARGUMENTS:
      *   - cylinder first base center point:
      *       const vec3 &P0;
      *   - cylinder first base radius:
      *       FLT R0;
      *   - cylinder second base center point:
      *       const vec3 &P1;
      *   - cylinder second base radius:
      *       FLT R1;
      *   - cylinder color:
      *       const vec4 &Color;
      * RETURNS: None.
      */
    VOID DrawCylinder( const vec3 &P0, FLT R0, const vec3 &P1, FLT R1, const vec4 &Color )
    {
      Cylinders.push_back({{P0, R0}, {P1, R1}, Color});
    } /* End of 'DrawCylinder' function */
  }; /* End of 'shader_manager' class */
} /* end of 'pivk' namespace */

#endif /* __markers_h_ */

/* END OF 'markers.h' FILE */
