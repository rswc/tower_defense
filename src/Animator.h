#pragma once

#include "Animation.h"

class Animator
{	
    		
private:
    std::vector<glm::mat4> m_FinalBoneMatrices;
    Animation* m_CurrentAnimation;
    float m_CurrentTime;
    float m_DeltaTime;	

public:
    Animator(Animation* animation) 
    : m_CurrentAnimation(animation)
    {
        m_CurrentTime = 0.0;
        //m_CurrentAnimation = currentAnimation;

        m_FinalBoneMatrices.reserve(100);

        for (int i = 0; i < 100; i++)
            m_FinalBoneMatrices.push_back(glm::mat4(1.0f));

        std::cout<<"Animator: animation root node"<<m_CurrentAnimation->GetRootNode().name<<std::endl;
        std::cout<<"Animator: created"<<std::endl;
    }
	
    void UpdateAnimation(float dt)
    {
        m_DeltaTime = dt;
        if (m_CurrentAnimation)
        {
            m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
            m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
            CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
        }
    }
	
    void PlayAnimation(Animation* pAnimation)
    {
        m_CurrentAnimation = pAnimation;
        m_CurrentTime = 0.0f;
    }
	
    void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
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

        // std::cout<<"Animator: node name: "<<nodeName<<std::endl;
        // std::cout<<"Transformation matrices"<<std::endl;
        // for(int i = 0; i < 4; i++)
        // {
        //     for(int j = 0; j < 4; j++)
        //     {
        //         std::cout<<globalTransformation[i][j]<<" ";
        //     }
        //     std::cout<<std::endl;
        // }
	
        auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
        if (boneInfoMap.find(nodeName) != boneInfoMap.end())
        {
            int index = boneInfoMap[nodeName].id;

            glm::mat4 offset = boneInfoMap[nodeName].offset;

            m_FinalBoneMatrices[index] = globalTransformation * offset;

            // if(index == 5 || index == 20){
            //     std::cout<<"Animator: bone index: "<<index<<std::endl;
            //     std::cout<<"Final matrix"<<std::endl;
            //     for(int i = 0; i < 4; i++)
            //     {
            //         for(int j = 0; j < 4; j++)
            //         {
            //             std::cout<<m_FinalBoneMatrices[index][i][j]<<" ";
            //         }
            //         std::cout<<std::endl;
            //     }
            // }
        }
	
        for (int i = 0; i < node->childrenCount; i++)
            CalculateBoneTransform(&node->children[i], globalTransformation);
    }
	
    std::vector<glm::mat4> GetFinalBoneMatrices() 
    { 
        return m_FinalBoneMatrices;  
    }

};