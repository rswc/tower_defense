#include "Animator.h"
#include <iostream> 

Animator::Animator(Animation* animation) : m_CurrentAnimation(animation)
{
    m_CurrentTime = 0.0f;
    //m_CurrentAnimation = currentAnimation;
    m_FinalBoneMatrices.reserve(100);

    for (int i = 0; i < 100; i++)
        m_FinalBoneMatrices.push_back(glm::mat4(1.0f));

    std::cout<<"Animator: animation root node"<<m_CurrentAnimation->GetRootNode().name<<std::endl;
    std::cout<<"Animator: created"<<std::endl;
}  
void Animator::UpdateAnimation(float dt)
{
    if (m_CurrentAnimation)
    {
        m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
        m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
        CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
    }
}
void Animator::PlayAnimation(Animation* pAnimation)
{
    m_CurrentAnimation = pAnimation;
    m_CurrentTime = 0.0f;
}
std::vector<glm::mat4>* Animator::GetFinalBoneMatrices() 
{ 
    return &m_FinalBoneMatrices;  
}
void Animator::CalculateBoneTransform(const AssimpNodeData* node, const glm::mat4 &parentTransform)
{
    std::string nodeName = node->name;
    glm::mat4 nodeTransform = node->transformation;

    Bone* Bone = m_CurrentAnimation->FindBone(nodeName);

    if (Bone)
    {
        Bone->Update(m_CurrentTime);
        nodeTransform = Bone->GetLocalTransform();
    }

    glm::mat4 globalTransformation = parentTransform * nodeTransform;

    std::map<std::string, BoneInfo> boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        int index = boneInfoMap[nodeName].id;
        glm::mat4 offset = boneInfoMap[nodeName].offset;
        m_FinalBoneMatrices[index] = globalTransformation * offset;
    }

    for (int i = 0; i < node->childrenCount; i++)
        CalculateBoneTransform(&node->children[i], globalTransformation);
}