/*************************************************************
 * Copyright (C) 2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : inc.glsl
 * PURPOSE     : Animation pattern on Vulkan API project.
 *               Markers shader group.
 *               Include data for cylinder marker shader.
 * PROGRAMMER  : CGSG'Jr'2024.
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 26.11.2024.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

/* Push constants data transfer structure for cylinders */
layout(std140, push_constant) uniform cylinder_marker
{
  // Matrixes
  mat4 MatrWVP;  // World-View-Project
  mat4 MatrW;    // World
  mat4 MatrWInv; // World inverted and transposed
  vec4 PR0;      // First base center point and radius
  vec4 PR1;      // Second base center point and radius
  vec4 Color;    // Cylinder color
}; /* End of 'cylinder_marker' structure */

/* END OF 'inc.glsl' FILE */
