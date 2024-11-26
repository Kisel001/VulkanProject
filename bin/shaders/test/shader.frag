//
// frag.glsl
//
//      Copyright (C) CGSG of PML30. All rights reserved.
//
// Frag shader of Triangle pack shader.  
//
#version 460

layout(std140, set = 0, binding = 0) uniform CAMERA_BUF
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
layout(std140, set = 0, binding = 1) uniform SYNC_BUF
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

struct Material
{
  vec4 Ka4;              // Ambient time parameters
  vec4 KdTrans;          // Diffuse coefficient and transpanency
  vec4 KsPh;             // Specular coefficient and Phong power value
  bvec4 TextureFlags;    // Material texture flags
};

// Material data UBO representation type
// UPDATE: At material creation       
layout(std140, set = 0, binding = 2) buffer BUF_MTL
{
  Material Materials[];
};

/*
 * PUSH CONSTANTS
 */

// Push constants 
layout(push_constant, std140) uniform DRAW_BUF
{
  mat4 MatrixW; // World matrix
  ivec4 Ids;   // Ids
};
              
#define MtlId Ids[0]
                                 
/* Macroses for fast UBO material data access  */
//#define Ka Ka4.xyz
//#define Kd KdTrans.xyz 
#define MtlKa Materials[MtlId].Ka4.xyz
#define MtlKd Materials[MtlId].KdTrans.xyz
//#define Trans KdTrans.w
#define MtlTrans Materials[MtlId].KdTrans.w
#define MtlKsPh Materials[MtlId].KsPh
//define Ks KsPh.xyz  
#define MtlKs Materials[MtlId].KsPh.xyz
//#define Ph KsPh.w 
#define MtlPh Materials[MtlId].KsPh.w   
#define IsTexture0 bool(Materials[MtlId].TextureFlags[0])     
#define IsTexture1 bool(Materials[MtlId].TextureFlags[1])
#define IsTexture2 bool(Materials[MtlId].TextureFlags[2])
#define IsTexture3 bool(Materials[MtlId].TextureFlags[3])

/*
 * PRIMITIVE
 */

struct Primitive
{                                                       
  ivec4 AddonIArray;   // Special integer parameters for shader
  vec4 AddonFArray;    // Special float parameters for shader
  vec4 AddonVArray[4]; // Special vector 4d parameters for shader
};

                           		
// Primitive draw parameters data UBO representation type
// UPDATE: before primitive drawing      
layout(std140, set = 0, binding = 3) buffer PRIM_BUF
{                
  Primitive PrimBufs[];
};

#define PrimId Ids[1]
                                               
/* Macroses for fast UBO Primitive data access  */
//#define MatrInv  MatrixWInv
//#define MatrWInv MatrixWInv
#define MatrW    MatrixW
//#define MatrWVP  MatrixWVP
#define AddonI   PrimBufs[PrimId].AddonIArray
#define AddonF   PrimBufs[PrimId].AddonFArray
#define AddonV   PrimBufs[PrimId].AddonVArray

//layout(std140, set = 0, binding = 4) sampl
layout(set = 1, binding = 0) uniform sampler2D Tex;


/*
in vec4 DrawColor;     
in vec2 DrawTexCoord;
in vec3 DrawNormal;
in vec3 DrawPosition;    
*/

layout(location = 0) in VS_OUT
{
  vec4 DrawColor;   
  vec2 DrawTexCoord;
  vec3 DrawNormal;
  vec3 DrawPosition; 
};                           

layout(location = 0) out vec4 OutColor;
//layout(location = 1) out vec4 OutPosId;

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
}
