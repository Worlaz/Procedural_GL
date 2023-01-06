#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;
uniform sampler2D u_TexturePos;

void main()
{
    
    float ID = gl_InstanceID;
    ivec2 particleIndex = ivec2(mod(ID, 100),floor(ID / 100));
    vec4 texValue = texelFetch(u_TexturePos,particleIndex,0);

    //vec4 grad = textureGrad(u_TexturePos,particleIndex,vec2(0.0),vec2(0.0));

    mat4 tranMatrix = mat4(1.0);
   tranMatrix[3] = texValue;
   tranMatrix[3][0] = tranMatrix[3][0] *5;
   tranMatrix[3][1] = tranMatrix[3][1] * 5;
   tranMatrix[3][2] = 0;
   tranMatrix[3][3] = 1;

   
   //tranMatrix[3][0] = grad[0] * 5;
   //tranMatrix[3][1] = 0;
   //tranMatrix[3][2] = 0;
   //tranMatrix[3][3] = 1;


    //tranMatrix[3][0] = textureLod(u_TexturePos,vec2(0.5,0.5),0.0); //vec2(gl_InstanceID)
    //tranMatrix[3][0] = tex2Dlod(u_TexturePos,vec4(0.5,0.5,0,0));
    //tranMatrix[3][1] = gl_InstanceID * 2;
    //tranMatrix[3][0] = gl_InstanceID;
    //tranMatrix[3][1] = texture(u_TexturePos,vec2(gl_InstanceID));
    //tranMatrix[3][2] = texture(u_TexturePos,vec2(gl_InstanceID));

    gl_Position =  u_MVP * tranMatrix * position; 
    //vec4 modPos = vec4(position[0] + gl_InstanceID*2, position[1],position[2], 1.0);
    //gl_Position = u_MVP *modPos;
    v_TexCoord = texCoord;
    
    
};