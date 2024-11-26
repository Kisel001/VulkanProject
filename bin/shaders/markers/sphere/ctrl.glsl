/*************************************************************
 * Copyright (C) 2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : ctrl.glsl
 * PURPOSE     : Animation pattern on Vulkan API project.
 *               Markers shader group.
 *               Sphere tessellation control shader.
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
  float
    d = distance(CamLoc, vec3(MatrW * vec4(CenterR.xyz, 1))) - CenterR.w,
    a = 1, b = 18,
    minn = 5, maxn = 30,
    n = d > b ? minn : d < a ? maxn : (d - a) / (b - a) * (minn - maxn) + maxn;

  n = 30;

  gl_TessLevelOuter[0] = n;
  gl_TessLevelOuter[1] = n;
  gl_TessLevelOuter[2] = n;
  gl_TessLevelOuter[3] = n;
  gl_TessLevelInner[0] = n;
  gl_TessLevelInner[1] = n;
} /* End of 'main' function */

/* END OF 'ctrl.glsl' FILE */
