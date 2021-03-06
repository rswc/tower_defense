#version 430 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 tex;
layout(location = 3) in vec4 boneIds; 
layout(location = 4) in vec4 weights;
	
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec3 cameraPos;
	
const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];
	
out vec2 texCoords;
out vec3 iNormal;
out vec3 iPosition;
out vec3 iViewDir;
	
void main()
{
    ivec4 intBones = ivec4(boneIds);
    vec4 totalPosition = vec4(0.0);
    vec3 totalNormal = vec3(0.0);

    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(intBones[i] == -1) 
            continue;
        if(intBones[i] >= MAX_BONES) 
        {
            totalPosition = vec4(pos,1.0);
            break;
        }
        vec4 localPosition = finalBonesMatrices[intBones[i]] * vec4(pos,1.0);
        totalPosition += localPosition * weights[i];
        vec3 localNormal = mat3(finalBonesMatrices[intBones[i]]) * norm * weights[i];
        totalNormal += localNormal;
    }

    vec4 worldPosition = M * totalPosition;

    iPosition = worldPosition.xyz;
    iNormal = (M * vec4(totalNormal, 0.0)).xyz;
    iViewDir = cameraPos - worldPosition.xyz;
		
    gl_Position =  P * V * worldPosition;

    texCoords = tex;

}