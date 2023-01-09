#version 460 core
        
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

//uniform float u_DeltaTime;



uniform sampler2D u_TexturePosition;
//uniform sampler2D u_TextureVelocity;
       

void main()
{
    

    vec4 position = texture(u_TexturePosition,v_TexCoord);
    vec4 posClamped = clamp(position + position * 0.000001f,vec4(-100.0f),vec4(100.0f));
    color = posClamped;
    //vec4 velocity =  texture(u_TextureVelocity, v_TexCoord);
    //vec4 newPos = position + velocity* (u_DeltaTime/100);
    // vec4 positionClamped = clamp(newPos,vec4(-100.0f),vec4(100.0f));
   
};