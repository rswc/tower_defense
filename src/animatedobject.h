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

    public: 
        void Draw(const Camera& camera) const override;
        void Update(double deltaTime) override;
        void loadModel(std::string filename) override;
        AnimatedObject();
        ~AnimatedObject();

};