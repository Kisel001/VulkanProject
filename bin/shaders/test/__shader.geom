//
// geom.glsl
//
//      Copyright (C) CGSG of PML30. All rights reserved.
//
// Geom shader of Sky pack shader.  
//
#version 460

layout(points) in;
layout(triangle_strip, max_vertices = 3) out;

void main( void )
{
  float z = 0;
 
  gl_Position = vec4(-1, 1, z, 1);
  EmitVertex();

  gl_Position = vec4(-1, -1, z, 1);
  EmitVertex();

  gl_Position = vec4(1, 1, z, 1);
  EmitVertex();               

  EndPrimitive();
}