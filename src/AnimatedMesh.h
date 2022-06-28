#pragma once

#include <glm/glm.hpp>
#include <vector>


#define MAX_BONE_INFLUENCE 4

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
	 // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
  
};
struct BoneInfo
{
    /*id is index in finalBoneMatrices*/
    int id;
    /*offset matrix transforms vertex from model space to bone space*/
    glm::mat4 offset;

};

struct singleBone {
    int ids[MAX_BONE_INFLUENCE];
};

struct singleWeight {
    float weights[MAX_BONE_INFLUENCE];
};

class AnimatedMesh{
    public: 
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> textures;
        std::vector<unsigned int> indices;
        std::vector<glm::vec4> boneIDs;
        std::vector<glm::vec4> weights;
        std::vector<Vertex> vertices_animated;

};