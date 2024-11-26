//
// frag.glsl
//
//      Copyright (C) CGSG of PML30. All rights reserved.
//
// Frag shader of Default pack shader.
//

#line 8

layout(binding = 0) uniform sampler2D Tex;

in vec4 DrawColor;     
in vec2 DrawTexCoord;
in vec3 DrawNormal;
in vec3 DrawPosition;    
 
void main( void )
{
  OutColor = vec4(1, 1, 1, 1);
  OutPosId = vec4(DrawPosition, 0);
  OutNormalIsShade = vec4(normalize(DrawNormal), 1);
  OutKa = vec4(MtlKa, 0);
  vec3 color = min(vec3(0.5), MtlKa);
  
  OutColor = vec4(DrawNormal, 1);

  vec3 KdDif = MtlKd;
  float alpha = MtlTrans;
  if (IsTexture0)
  {
    vec4 tc = texture(Tex, vec2(DrawTexCoord.x, 1 - DrawTexCoord.y));
    KdDif = tc.rgb;
    alpha = tc.a;
    if (alpha < 0.8)
      discard;
  }              

  //if (MtlTrans < 0.8)
  //  discard;

  OutKdTrans = vec4(KdDif, alpha);
  OutKsPh = MtlKsPh; // vec4(MtlKs, MtlPh);
  //OutKd = MtlKd;

  //bool IsShade = false;

  //if (OutNormalIsShade.w)
 //   IsShade = true;

  //if (IsShade == true)
  //{
  vec3 _color = Shade(OutPosId.xyz, OutNormalIsShade.xyz, OutKa.xyz, OutKdTrans.xyz, OutKsPh.xyz, OutKsPh.w);
  OutColor = vec4(_color, alpha);
  //}
}

// 
