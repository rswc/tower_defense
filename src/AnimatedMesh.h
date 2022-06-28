#pragma once
#include <vector>
#include <glm/glm.hpp>

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
class AnimatedMesh{
    public: 
        std::vector<glm::vec4> vertices;
        std::vector<glm::vec4> normals;
        std::vector<glm::vec2> textures;
        std::vector<unsigned int> indices;
        std::vector<Vertex> vertices_animated;

};