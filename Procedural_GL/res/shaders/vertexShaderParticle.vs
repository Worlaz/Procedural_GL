#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;
uniform sampler2D u_TexturePos;

void main()
{
    
    float ID = gl_InstanceID;
    ivec2 particleIndex = ivec2(mod(ID, 100.0f),floor(ID / 100.0f));
    vec4 texValue = texelFetch(u_TexturePos,particleIndex,0);

    //vec4 grad = textureGrad(u_TexturePos,particleIndex,vec2(0.0f),vec2(0.0)f);

    mat4 tranMatrix = mat4(1.0f);
    tranMatrix[3][0] = texValue[0];//  * 50;
    tranMatrix[3][1] = texValue[1];// * 50;
    tranMatrix[3][2] = texValue[2];// * 50;
    tranMatrix[3][3] = 1.0f;


    gl_Position =  u_MVP * tranMatrix * position; 
    //gl_Position = u_MVP * texValue;
    v_TexCoord = texCoord;
    
    
};