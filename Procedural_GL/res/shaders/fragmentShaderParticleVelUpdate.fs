#version 460 core
        
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform float u_DeltaTime;
//uniform sampler2D u_TextureVelocity;
       


float random(vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

void main()
{
    

    //vec4 velocity = texture(u_TextureVelocity,v_TexCoord) - vec4(0.1f,0.0f,0.0f,0.0f)*u_DeltaTime;// + texture(u_TextureVelocity,v_TexCoord) * (u_DeltaTime);
    color = vec4(0.0f,0.1f * u_DeltaTime,0.0f,1.0f);
     //color = vec4(clamp(velocity[0],-1.0f,1.0f),clamp(velocity[1],-1.0f,1.0f),clamp(velocity[2],-1.0f,5.0f),1);
    //color = velocity;
    //color = vec4(0.0f,0.0001f,0.0f,1.0f) * u_DeltaTime;
   
    
    //color = vec4(texture(u_TextureVelocity,v_TexCoord))*u_DeltaTime;
     //color = vec4(0.2,0.0,0.0,1.0);
};