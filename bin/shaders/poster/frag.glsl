//
// frag.glsl
//
//      Copyright (C) CGSG of PML30. All rights reserved.
//
// Frag shader of Poster pack shader.  
//

in vec2 DrawPosition;

layout(binding = 0) uniform sampler2D Tex;

void main( void )
{                                           
  OutColor = texelFetch(Tex, ivec2(DrawPosition), 0);
  OutPosId = vec4(DrawPosition, 0, 0);
                      
  OutKdTrans = vec4(OutColor.rgb, MtlTrans);
  OutKsPh = MtlKsPh;
  OutKa = vec4(MtlKa, 0);
  OutNormalIsShade = vec4(normalize(vec3(1, 0, 0)), 0);
}