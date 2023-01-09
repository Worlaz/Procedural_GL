#version 460 core
        
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform float u_DeltaTime;



uniform sampler2D u_TexturePosition;
uniform sampler2D u_TextureVelocity;
       


float random(vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

void main()
{
    

    vec4 position = texture(u_TexturePosition,v_TexCoord);
    vec4 velocity =  texture(u_TextureVelocity, v_TexCoord);
    vec4 newPos = position + velocity* (u_DeltaTime/100);
     vec4 positionClamped = clamp(newPos,vec4(-100.0f),vec4(100.0f));
   color = positionClamped;
};