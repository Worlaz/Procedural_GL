#version 460 core
        
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

//uniform vec4 u_Color;
//uniform sampler2D u_TexturePos;
       
void main()
{
    //vec4 texColor = texture(u_Texture,v_TexCoord);
    //color = texColor;

    
    //float radius = length(f);
    //color = vec4(sin(radius*v_TexCoord[0])/2 + 0.5);

    //a gray scale particle
    //vec2 f =v_TexCoord * 2 - vec2(0.5);
    //color = vec4(1 - distance(f,vec2(0.5,0.5)));

    //vec4 texColor = texture(u_TexturePos,v_TexCoord);
    //vec4 texColor = texture(u_TexturePos,v_TexCoord);
    color = vec4(1.0);
};