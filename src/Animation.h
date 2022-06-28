#pragma once

#include "assimploaderanimated.h"
#include "Bone.h"

#include <functional>

#include <vector>
#include <map>
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>

struct AssimpNodeData
{
    glm::mat4 transformation;
    std::string name;
    int childrenCount;
    std::vector<AssimpNodeData> children;
};

glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from);

class Animation
{
    private:
        float m_Duration;
        int m_TicksPerSecond;
        std::vector<Bone> m_Bones;
        AssimpNodeData m_RootNode;
        std::map<std::string, BoneInfo> m_BoneInfoMap;
    public:
        Animation() = default;

        Animation(const std::string& animationPath, AnimatedAssimpLoader* model)
        {
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
            assert(scene && scene->mRootNode);
            auto animation = scene->mAnimations[0];
            m_Duration = animation->mDuration;
            m_TicksPerSecond = animation->mTicksPerSecond;
            ReadHeirarchyData(m_RootNode, scene->mRootNode);
            ReadMissingBones(animation, *model);
            // std::cout<<"Animation duration: "<<m_Duration<<std::endl;
            // std::cout<<"Animation ticks per second: "<<m_TicksPerSecond<<std::endl;
            // std::cout<<"Root node name: "<<m_RootNode.name<<std::endl;
            // std::cout<<"Root node children count: "<<m_RootNode.childrenCount<<std::endl;
            // std::cout<<"Root node children vector size: "<<m_RootNode.children.size()<<std::endl;
            // for(auto p : m_BoneInfoMap){
            //     std::cout<<p.first<<std::endl;
            //     std::cout<<"m_BoneInfoMap matrix"<<std::endl;
            //     // for(int i = 0; i < 4; i++)
            //     // {
            //     //     for(int j = 0; j < 4; j++)
            //     //     {
            //     //         std::cout<<p.second.offset[i][j]<<" ";
            //     //     }
            //     //     std::cout<<std::endl;
            //     // }
            // }
         
      

        }

        ~Animation()
        {
        }

        Bone* FindBone(const std::string& name)
        {
            auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
                [&](const Bone& Bone)
                {
                    return Bone.GetBoneName() == name;
                }
            );
            if (iter == m_Bones.end()) return nullptr;
            else return &(*iter);
        }

        
        inline float GetTicksPerSecond() { return m_TicksPerSecond; }

        inline float GetDuration() { return m_Duration;}

        inline const AssimpNodeData& GetRootNode() { return m_RootNode; }

        inline const std::map<std::string,BoneInfo>& GetBoneIDMap() 
        { 
            return m_BoneInfoMap;
        }

    private:
        void ReadMissingBones(const aiAnimation* animation, AnimatedAssimpLoader& model)
        {
            int size = animation->mNumChannels;

            auto& boneInfoMap = model.GetBoneInfoMap();//getting m_BoneInfoMap from Model class
            int& boneCount = model.GetBoneCount(); //getting the m_BoneCounter from Model class

            //reading channels(bones engaged in an animation and their keyframes)
            for (int i = 0; i < size; i++)
            {
                auto channel = animation->mChannels[i];
                std::string boneName = channel->mNodeName.data;

                if (boneInfoMap.find(boneName) == boneInfoMap.end())
                {
                    boneInfoMap[boneName].id = boneCount;
                    boneCount++;
                }
                m_Bones.push_back(Bone(channel->mNodeName.data,
                    boneInfoMap[channel->mNodeName.data].id, channel));
            }

            m_BoneInfoMap = boneInfoMap;
        }

        void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src)
        {
            assert(src);

            dest.name = src->mName.data;
            dest.transformation = ConvertMatrixToGLMFormat(src->mTransformation);
            //dest.transformation = glm::mat4(1.0f);
            dest.childrenCount = src->mNumChildren;

            // std::cout<<"Node name: "<<dest.name<<std::endl;
            // std::cout<<"Node children count: "<<dest.childrenCount<<std::endl;
            // std::cout<<"Transformation matrices"<<std::endl;
            // for(int i = 0; i < 4; i++)
            // {
            //     for(int j = 0; j < 4; j++)
            //     {
            //         std::cout<<dest.transformation[i][j]<<" ";
            //     }
            //     std::cout<<std::endl;
            // }
            
            // std::cout<<std::endl;
            // std::cout<<std::endl;

            for (int i = 0; i < src->mNumChildren; i++)
            {
                AssimpNodeData newData;
                ReadHeirarchyData(newData, src->mChildren[i]);
                dest.children.push_back(newData);
            }

            
        }

};