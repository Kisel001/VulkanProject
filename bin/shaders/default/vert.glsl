//
// vert.glsl
//
//      Copyright (C) CGSG of PML30. All rights reserved.
//
// Vert shader of Default pack shader.
//
   
layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;
                                    
out vec4 DrawColor;   
out vec2 DrawTexCoord;
out vec3 DrawNormal;
out vec3 DrawPosition; 

void main( void )
{
  gl_Position = MatrWVP * vec4(InPosition, 1);
  DrawColor = InColor;
  DrawNormal = mat3(MatrWInv) * InNormal;  
  DrawPosition = vec3(MatrW * vec4(InPosition, 1));
  DrawTexCoord = InTexCoord;                       
}
