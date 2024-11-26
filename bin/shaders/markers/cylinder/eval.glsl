/*************************************************************
 * Copyright (C) 2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : ctrl.glsl
 * PURPOSE     : Animation pattern on Vulkan API project.
 *               Markers shader group.
 *               Cylinder tessellation evaluation shader.
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

// Tessellation parameters
layout(quads, /* fractional_even_spacing */ equal_spacing) in;

// Output data for fragment shader
layout(location = 0) out ES_OUT
{
  vec3 DrawNormal; // Vertex normal
} es_out;

/* The main shader function */
void main( void )
{
  float Radius0 = PR0.w, Radius1 = PR1.w;
  vec3
    P0 = PR0.xyz,
    P1 = PR1.xyz,
    Dir = P1 - P0,
    P, N;
  float DirLen = length(Dir);

  Dir /= DirLen;
  if (gl_TessCoord.y == 1)
    P = P1, N = Dir;
  else if (gl_TessCoord.y == 0)
    P = P0, N = -Dir;
  else
  {
    vec3
      U = cross(Dir, vec3(1, 0, 0)) + cross(Dir, vec3(0, 1, 0)),
      R = normalize(cross(Dir, U));
    U = cross(R, Dir);

    float
      pi = acos(-1),
      phi = gl_TessCoord.x * 2 * pi,
      sp = sin(phi), cp = cos(phi),
      radius = mix(Radius0, Radius1, gl_TessCoord.y >= 0.5);

    P = mix(P0, P1, float(gl_TessCoord.y >= 0.5)) + (R * cp + U * sp) * radius;

    if (gl_TessCoord.y > 0.70)
      N = Dir;
    else if (gl_TessCoord.y < 0.30)
      N = -Dir;
    else
      N = Dir * (Radius0 - Radius1) + (R * cp + U * sp) * DirLen;
  }
  es_out.DrawNormal = mat3(MatrWInv) * normalize(N);
  gl_Position = MatrWVP * vec4(P, 1);
} /* End of 'main' function */

/* END OF 'eval.glsl' FILE */
