#include "Bone.h"

glm::vec3 GetGLMVec(const aiVector3D& vec) 
{ 
    return glm::vec3(vec.x, vec.y, vec.z); 
}

glm::quat GetGLMQuat1(const aiQuaternion& pOrientation)
{
    return glm::quat(pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z);
}