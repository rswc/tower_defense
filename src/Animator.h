#pragma once
#include "Animation.h"

//Animator class which is used to animate the model
//Requires a model to be loaded and an animation to be loaded
class Animator
{	 
    private:
        std::vector<glm::mat4> m_FinalBoneMatrices;
        Animation* m_CurrentAnimation;
        float m_CurrentTime;
    public:
        Animator(Animation* animation);
        void UpdateAnimation(float dt);
        void PlayAnimation(Animation* pAnimation);
        void CalculateBoneTransform(const AssimpNodeData* node, const glm::mat4 &parentTransform);
        std::vector<glm::mat4>* GetFinalBoneMatrices();
};