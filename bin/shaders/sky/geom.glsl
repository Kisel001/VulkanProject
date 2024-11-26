//
// geom.glsl
//
//      Copyright (C) CGSG of PML30. All rights reserved.
//
// Geom shader of Sky pack shader.  
//

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

void main( void )
{
  float z = 0.9999;
 
  gl_Position = vec4(-1, 1, z, 1);
  EmitVertex();

  gl_Position = vec4(-1, -1, z, 1);
  EmitVertex();

  gl_Position = vec4(1, 1, z, 1);
  EmitVertex();
 
  gl_Position = vec4(1, -1, z, 1);
  EmitVertex();

  EndPrimitive();
}