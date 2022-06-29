#pragma once
#include "assimploaderanimated.h"
#include "Bone.h"

#include <vector>
#include <map>
#include <glm/glm.hpp>


struct AssimpNodeData
{
    glm::mat4 transformation;
    std::string name;
    int childrenCount;
    std::vector<AssimpNodeData> children;
};

class Animation
{
    private:
        float m_Duration;
        int m_TicksPerSecond;
        std::vector<Bone> m_Bones;
        AssimpNodeData m_RootNode;
        std::map<std::string, BoneInfo> m_BoneInfoMap;

        void ReadMissingBones(const aiAnimation* animation, AnimatedAssimpLoader& model);
        void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);

    public:
        Animation() = default;
        Animation(const aiScene* scene, AnimatedAssimpLoader* model);
         ~Animation(){}
        Bone* FindBone(const std::string& name);
        inline float GetTicksPerSecond() { return m_TicksPerSecond; }
        inline float GetDuration() { return m_Duration;}
        inline const AssimpNodeData& GetRootNode() { return m_RootNode; }
        inline const std::map<std::string,BoneInfo>& GetBoneIDMap() { return m_BoneInfoMap;}
};