#pragma once
#include "assimpobject.h"
#include "assimploaderanimated.h"
#include "Animator.h"

class AnimatedObject : public AssimpObject
{
    private: 
        std::vector<AnimatedMesh> meshes;
        //Animation danceAnimation;
        Animator* animator;
        Animation* danceAnimation;
        float animationSpeed = 1.0f;
        glm::vec3 modelScale = glm::vec3(1.0f);
        void startSetup();

    public: 
        void Draw(const Camera& camera) const override;
        void Update(double deltaTime) override;
        void loadModel(std::string filename) override;
        AnimatedObject();
        AnimatedObject(glm::vec3 targetModelScale, float targetAnimationSpeed);
        ~AnimatedObject();
};