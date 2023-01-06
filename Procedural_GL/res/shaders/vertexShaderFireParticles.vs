#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
    vec4 modifiedPos = vec4(position[0] + 2*gl_InstanceID,position[1],position[2],position[3]);
    gl_Position = u_MVP * modifiedPos; 
    v_TexCoord = texCoord;
    //gl_Position = position; 

    
};