#version 460 core
        
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform float u_DeltaTime;

//uniform vec4 u_Color;
uniform sampler2D u_TexturePosition;
uniform sampler2D u_TextureVelocity;
       


float random(vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

void main()
{
    //vec4 texColor = texture(u_Texture,v_TexCoord);
    //color = vec4(1.0,0.0,0.0,1.0);

    //vec4 texColor = texture(u_Texture,v_TexCoord);
    //color = vec4(vec3(random(v_TexCoord)) + texture(u_TexturePosition,v_TexCoord),1.0) ;
    //color = vec4(vec3(random(v_TexCoord)),1.0);
    //color = vec4(random(v_TexCoord),random(v_TexCoord * 0.5),random(v_TexCoord * 0.3),1.0);

    vec4 position = texture(u_TexturePosition,v_TexCoord);
    vec4 velocity =  texture(u_TextureVelocity, v_TexCoord);
    vec4 newPos =vec4(position[0] + velocity[0]* u_DeltaTime,position[1] + velocity[1]* u_DeltaTime,position[2] + velocity[2]* u_DeltaTime,1.0);
    //color = vec4(velocity[0],velocity[1],velocity[2],1.0);
    color = texture(u_TexturePosition,v_TexCoord) + (texture(u_TextureVelocity, v_TexCoord))* (u_DeltaTime / 100);
    

     //color = vec4(0.2,0.0,0.0,1.0);
};