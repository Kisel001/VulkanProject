//
// commondf.glsl
//
//      Copyright (C) CGSG of PML30. All rights reserved.
//
// Base defines for shader programs.
//



#ifndef _COMDF_GLSL_INC_ // Protection against repeated inclusions
#define _COMDF_GLSL_INC_



//=========================================
//
//  Declaring Variables with UBO Data Store
//
//=========================================


/*
 * CAMERA & MATRIX
 */

// Matrix and camera data UBO representation type 
// UPDATE: Once per camera/frame change.     
layout(std140, binding = 0) uniform CAMERA_BUF
{
  mat4 MatrVP;               // Premultipled view and projection matrixes
  mat4 MatrV;                // Maxtrix view
  mat4 MatrP;                // Matrix projection
  vec4 CamLoc4;              // Camera location
  vec4 CamRight4;            // Camera right direction
  vec4 CamUp4;               // Camera up direction
  vec4 CamDir4;              // Camera forward direction
  vec4 CamAt4;               // Cam at direction
  vec4 FrameWHProjSizeDist;  // Camera frame size, proj. size and dist
  vec4 LightDir4;            // Light direction source
  vec4 LightColor4;          // Light color
  mat4 MatrShadow;           // Shadow map matrix
};

/* Macroses for fast UBO camera and matrix data access  */    
#define CamLoc CamLoc4.xyz
#define CamRight CamRight4.xyz
#define CamUp CamUp4.xyz
#define CamDir CamDir4.xyz
#define CamAt CamAt4.xyz
#define FrameW FrameWHProjSizeDist.x
#define FrameH FrameWHProjSizeDist.y
#define ProjSize FrameWHProjSizeDist.z
#define ProjDist FrameWHProjSizeDist.w
#define LightDir LightDir4.xyz
#define LightColor LightColor4.xyz


/*
 * SYNC
 */

// Synchronization data UBO representation type
// UPDATE: Every frame.          
layout(std140, binding = 1) uniform SYNC_BUF
{
  vec4 TimeDTGlobalTimeDT;    // Time parameters
  vec4 IsWireFrameIsPause[4]; // WireFrame, pause, FPS, ... flags
};
                                              
/* Macroses for fast UBO synchronization data access  */
#define Time            TimeDTGlobalTimeDT.x
#define DeltaTime       TimeDTGlobalTimeDT.y
#define GlobalTime      TimeDTGlobalTimeDT.z
#define GlobalDeltaTime TimeDTGlobalTimeDT.w
#define IsWireFrame     bool(IsWireFrameIsPause[0])
#define IsPause         bool(IsWireFrameIsPause[1])
#define FPS             IsWireFrameIsPause.z


/*
 * MATERIAL
 */

// Material data UBO representation type
// UPDATE: At material creation       
layout(std140, binding = 2) uniform MTL_BUF
{
  vec4 Ka4;             // Ambient time parameters
  vec4 KdTrans;         // Diffuse coefficient and transpanency
  vec4 KsPh;            // Specular coefficient and Phong power value
  bool TextureFlags[8]; // Material texture flags
};
                                               
/* Macroses for fast UBO material data access  */
//#define Ka Ka4.xyz
//#define Kd KdTrans.xyz 
#define MtlKa Ka4.xyz
#define MtlKd KdTrans.xyz
//#define Trans KdTrans.w
#define MtlTrans KdTrans.w
#define MtlKsPh KsPh
//define Ks KsPh.xyz  
#define MtlKs KsPh.xyz
//#define Ph KsPh.w 
#define MtlPh KsPh.w   
#define IsTexture0 bool(TextureFlags[0])     
#define IsTexture1 bool(TextureFlags[1])
#define IsTexture2 bool(TextureFlags[2])
#define IsTexture3 bool(TextureFlags[3])

/*
 * PRIMITIVE
 */

// Primitive draw parameters data UBO representation type
// UPDATE: before primitive drawing      
layout(std140, binding = 3) uniform PRIM_BUF
{
  mat4 MatrixWVP;      // Premultiplyed 'world' - 'view' - 'projection' matrixes
  mat4 MatrixW;        // World primitive matrix
  mat4 MatrixWInv;     // Inverse transposed world matrix (for normals)
  ivec4 AddonIArray;   // Special integer parameters for shader
  vec4 AddonFArray;    // Special float parameters for shader
  vec4 AddonVArray[4]; // Special vector 4d parameters for shader
};
                                               
/* Macroses for fast UBO Primitive data access  */
#define MatrInv  MatrixWInv
#define MatrWInv MatrixWInv
#define MatrW    MatrixW
#define MatrWVP  MatrixWVP
#define AddonI   AddonIArray
#define AddonF   AddonFArray
#define AddonV   AddonVArray

/*
 * Base definitions of frag shader
 */

#ifdef FRAGMENT_SHADER

layout(location = 0) out vec4 OutColor;
layout(location = 1) out vec4 OutPosId;
layout(location = 2) out vec4 OutNormalIsShade;
layout(location = 3) out vec4 OutKa;
layout(location = 4) out vec4 OutKdTrans;
layout(location = 5) out vec4 OutKsPh;

layout(binding = 0) uniform sampler2D InColor;
layout(binding = 1) uniform sampler2D InPosId;
layout(binding = 2) uniform sampler2D InNormalIsShade;
layout(binding = 3) uniform sampler2D InKa;
layout(binding = 4) uniform sampler2D InKdTrans;
layout(binding = 5) uniform sampler2D InKsPh;

layout(binding = 0) uniform sampler2D GBufs[6];
                                             
layout(binding = 8) uniform sampler2D SkyTex;

/* Shd func */
vec3 Shade( vec3 P, vec3 N, vec3 Ka, vec3 Kd, vec3 Ks, float Ph )
{
  vec3 V = normalize(P - CamLoc);
  vec3 L = (vec3(sin(Time) * 0.30 * 5 * 5, 8, 1));
  vec3 LDir = vec3(0, -1, 0);
  //L = CamLoc;
  L = vec3(5, 5, 5);
 
  L = L - P;
 
  float d = length(L);
  L /= d;
 
  float
    cc = 1,
    cl = 0.001,
    cq = 0.0001,
    att = min(1, 1 / (cc + cl * d + cq * d * d));
 
  float ld = dot(-L, LDir);
  float cut = cos(radians(47.0));
  if (ld < cut)
    att = 0;
  else
    att *= pow(1 - cut / ld, 0.47);
 
  att = 1;
 
  vec3 color = min(vec3(0.1), Ka);
 
  N = faceforward(N, V, N);
 
  color += Kd * max(0.5, dot(N, L));
  //Specular
  vec3 R;
  color += Ks * max(0, pow(dot((R = reflect(V, N)), L), Ph));
 
  color *= att;
 
  // Add sky sphere color
  vec2
    ts = vec2(Time * 0.030 + atan(R.x, R.z) / (2 * acos(-1)),
              -acos(R.y) / acos(-1));
  vec4
    skyc = texture(SkyTex, ts);
  //SKY TEXTURE OFF :: 
  //color += 0.47 * MtlKs * skyc.rgb;
 
  /*
  vec2 tv = pow(abs(sin(DrawPosition.xz * 0.30 + Time)), vec2(111));
  float t = tv.x * tv.y;
  color = mix(vec3(0, 1, 1), color, 1 - t);
  tv = pow(abs(sin(DrawPosition.xz * 0.30 + Time)), vec2(122115));
  t = tv.x + tv.y;
  color = mix(vec3(0, 1, 0), color, 1 - t);
  */
 
  return color;
}

#endif // FRAGMENT_SHADER


#endif // !__COMDF_GLSL_INC_ 



// END OF 'commondf.glsl' FILE
