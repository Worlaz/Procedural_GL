#version 460 core
        
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform float u_DeltaTime;



uniform sampler2D u_TexturePosition;
uniform sampler2D u_TextureVelocity;
       

void main()
{
    
   
    vec4 position = texture(u_TexturePosition,v_TexCoord);
    
    
    vec4 velocity =  texture(u_TextureVelocity, v_TexCoord);
    vec4 newPos = position + velocity * u_DeltaTime;
    newPos[3] = 1.0f;
    
    
     vec4 positionClamped = clamp(newPos,vec4(-200.0f),vec4(200.0f));

     color = positionClamped;
    
    
    
    
   
};