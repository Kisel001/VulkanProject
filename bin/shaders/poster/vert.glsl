//
// vert.glsl
//
//      Copyright (C) CGSG of PML30. All rights reserved.
//
// Vert shader of Poster pack shader.  
//

out vec2 DrawPosition;

void main( void )
{                                                       
  vec2 p[4] =
    vec2[4](vec2(0, 0),
            vec2(0, MtlKd.y - 1),
            vec2(MtlKd.x - 1, 0),
            vec2(MtlKd.x - 1, MtlKd.y - 1));
  DrawPosition = p[gl_VertexID];
  gl_Position = MatrW * vec4(DrawPosition + MtlKa.xy, 0, 1);
}