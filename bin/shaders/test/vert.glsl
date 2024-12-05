//
// vert.glsl
//
//      Copyright (C) CGSG of PML30. All rights reserved.
//
// Vert shader of Triangle pack shader.  
//
#version 460

#include <commondf.h>

// Entry to shader data
layout(location = 0) in vec3 InPosition; // Position
layout(location = 1) in vec2 InTexCoord; // Texture coordinates
layout(location = 2) in vec3 InNormal;   // Normal          

// Out from shader data
layout(location = 0) out VS_OUT
{
  vec4 DrawColor;    // Color
  vec2 DrawTexCoord; // Texture coordinates
  vec3 DrawNormal;   // Normal
  vec3 DrawPosition; // Position
};

/* Entry shader function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{                                   
  gl_Position = MatrVP * MatrW * vec4(InPosition, 1);
  DrawColor = vec4(MtlKd, 1);
  DrawNormal = mat3(inverse(transpose(MatrW))) * InNormal; 
  DrawPosition = vec3(MatrW * vec4(InPosition, 1));
  DrawTexCoord = InTexCoord; 
} /* End of 'main' function */

// END OF 'vert.glsl' FILE

