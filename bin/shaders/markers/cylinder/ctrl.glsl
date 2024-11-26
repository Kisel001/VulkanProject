/*************************************************************
 * Copyright (C) 2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : ctrl.glsl
 * PURPOSE     : Animation pattern on Vulkan API project.
 *               Markers shader group.
 *               Cylinder tessellation control shader.
 * PROGRAMMER  : CGSG'Jr'2024.
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 26.11.2024.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 460
#include <commondf.glsl>
#include "inc.glsl"

// Number of stream points in patch
layout(vertices = 1) out;

/* The main shader function */
void main( void )
{
  int n1 = 5, n2 = 64;

  gl_TessLevelOuter[0] = n1;
  gl_TessLevelOuter[1] = n2;
  gl_TessLevelOuter[2] = n1;
  gl_TessLevelOuter[3] = n2;

  gl_TessLevelInner[0] = n2;
  gl_TessLevelInner[1] = n1;
} /* End of 'main' function */

/* END OF 'ctrl.glsl' FILE */
