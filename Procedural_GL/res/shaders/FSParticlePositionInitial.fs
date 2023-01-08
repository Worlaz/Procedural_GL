#version 460 core
        
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

       


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
    color = vec4((2.0f * random(v_TexCoord) -1.0f) * 50.0f,(2.0f * random(v_TexCoord * 0.5f) -1.0f) *50.0f,( 2*random(v_TexCoord * 0.3f) -1.0f)*50f,1.0f);
    //color = vec4(0.0,-1.0,0.0,1);
    //color = vec4(60.0,0.0,0.0,1.0);
};