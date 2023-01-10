#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;
uniform float u_sqrtnmbrOfParticles;
uniform sampler2D u_TexturePos;

void main()
{
    
    float ID = gl_InstanceID;
    ivec2 particleIndex = ivec2(mod(ID, u_sqrtnmbrOfParticles),floor(ID / u_sqrtnmbrOfParticles));
    vec4 texValue = texelFetch(u_TexturePos,particleIndex,0);

    mat4 tranMatrix = mat4(1.0f);
    tranMatrix[3][0] = texValue[0];
    tranMatrix[3][1] = texValue[1];
    tranMatrix[3][2] = texValue[2];
    tranMatrix[3][3] = 1.0f;

    
    

    gl_Position =  u_MVP * tranMatrix * position; 
    v_TexCoord = texCoord;
    
    
};