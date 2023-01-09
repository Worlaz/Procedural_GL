#version 460 core
        
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform float u_DeltaTime;
uniform vec3 u_Acceleration;
uniform sampler2D u_TextureVelocity;
       


float random(vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

void main()
{
    

    vec4 velocity = texture(u_TextureVelocity,v_TexCoord) + vec4(u_Acceleration[0],u_Acceleration[1],u_Acceleration[2],0.0f)*u_DeltaTime/100;
    vec4 velocityClamped = clamp(velocity,vec4(-1.0f),vec4(1.0f));
    color = velocityClamped;
    
};