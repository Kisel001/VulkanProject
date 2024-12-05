//
// frag.glsl
//
//      Copyright (C) CGSG of PML30. All rights reserved.
//
// Fragment shader of Triangle pack shader.  
//
#version 460

#include <commondf.h>

// Entry data to shader
layout(location = 0) in VS_OUT
{
  vec4 DrawColor;    // Color
  vec2 DrawTexCoord; // Texture coordinates
  vec3 DrawNormal;   // Normal
  vec3 DrawPosition; // Position
};                           

// Out data from shader
layout(location = 0) out vec4 OutColor; // Color

//layout(location = 1) out vec4 OutPosId;

/* Entry shader function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{
  if (IsTexture0)
  {
    vec4 tc = texture(Tex, vec2(DrawTexCoord.x, 1 - DrawTexCoord.y));
    tc.a = 1;
    OutColor = tc;
  }
  else
    OutColor = vec4(DrawNormal, 1.f);
} /* End of 'main' function */

// END OF 'frag.glsl' FILE
