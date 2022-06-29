#pragma once
#include "AnimatedMesh.h"

#include <assimp/scene.h>
#include <vector>
#include <map>


class AnimatedAssimpLoader
{
    private:
        std::vector<AnimatedMesh> meshes;

    public:
        std::vector<AnimatedMesh> getMeshes();
        void loadModel(std::string filename);
        void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
        //From tutorial Model class
        std::map<std::string, BoneInfo> m_BoneInfoMap; 
        int m_BoneCounter = 0;
        std::map<std::string, BoneInfo>& GetBoneInfoMap() { return m_BoneInfoMap; }
        int GetBoneCount() { return m_BoneCounter; }   
};