//
// frag.glsl
//
//      Copyright (C) CGSG of PML30. All rights reserved.
//
// Frag shader of Sky pack shader.  
//


void main( void )
{
  float Wp, Hp;

  Wp = Hp = ProjSize;

  if (FrameW >= FrameH)
    Wp *= FrameW / FrameH;
  else
    Hp *= FrameH / FrameW;

  float
    xp = gl_FragCoord.x * Wp / FrameW - Wp / 2,
    yp = gl_FragCoord.y * Hp / FrameH - Hp / 2;

  vec3 D = normalize(CamDir * ProjDist + CamRight * xp + CamUp * yp);

  vec2
    ts = vec2(atan(D.x, D.z) / (2 * acos(-1)),
              -acos(D.y) / acos(-1));
  vec4
    tc = texture(SkyTex, vec2(ts.x, 1 - ts.y));
 
  OutColor = vec4(tc.r, tc.g, tc.b, 1);
  OutPosId = vec4(0);
  OutNormalIsShade = vec4(0, 0, 1, 0);
  OutKa = vec4(0);
  OutKdTrans = vec4(0);
  OutKsPh = vec4(0);

}