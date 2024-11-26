/*************************************************************
 * Copyright (C) 2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : ctrl.glsl
 * PURPOSE     : Animation pattern on Vulkan API project.
 *               Markers shader group.
 *               Cylinder fragment shader.
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

// Output color attribute
layout(location = 0) out vec4 OutColor;

// Input data from tessellation
layout(location = 0) in ES_IN
{
  vec3 DrawNormal; // Interpolated normal
} es_in;

/* The main shader function */
void main( void )
{
  float nl = max(0.2, dot(normalize(es_in.DrawNormal), normalize(-vec3(CamDir))));
  OutColor = vec4(Color.rgb * nl, 1);
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */
