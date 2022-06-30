#include "assimploaderanimated.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>
#include "utility.h"

void SetVertexBoneDataToDefault(Vertex& vertex)
{
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        vertex.m_BoneIDs[i] = -1;
        vertex.m_Weights[i] = 0.0f;
    }
}
void SetVertexBoneData(Vertex& vertex, int boneID, float weight)
{
    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
    {
        if (vertex.m_BoneIDs[i] < 0)
        {
            vertex.m_Weights[i] = weight;
            vertex.m_BoneIDs[i] = boneID;
            break;
        }
    }
}

void AnimatedAssimpLoader::loadModel(std::string filename){
    Assimp::Importer importer;

    //aiProcess_triangulate -> make triangles
    //aiProcess_FlipUVs -> flip y coordinate on textures 
    //[opengl format is different than everywhere else] v = 1 - v 
    // aiProcess_GenSmoothNormals -> generate smooth normals
    // pointer is automatically deleted when importer is destroyed
    const aiScene* scene = importer.ReadFile(filename.c_str(), 
    aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals); 

    std::cout<<importer.GetErrorString()<<std::endl;

    if (scene->HasMeshes()){
        for(int k = 0; k< scene->mNumMeshes; k++)
        {
            aiMesh* mesh = scene->mMeshes[k]; //Get specific mesh
            AnimatedMesh newMesh;
            std::cout<<"Loading animated mesh from " << filename << std::endl;

            for(int i = 0; i< mesh->mNumVertices; i++){ 
                Vertex vertex;

                SetVertexBoneDataToDefault(vertex);

                aiVector3D vec = mesh->mVertices[i];
                vertex.Position = glm::vec3(vec.x, vec.y, vec.z);

                aiVector3D normal = mesh->mNormals[i];
                vertex.Normal = glm::vec3(normal.x, normal.y, normal.z);

                if (mesh->mTextureCoords[0])
                    {
                        glm::vec2 vec;
                        vec.x = mesh->mTextureCoords[0][i].x;
                        vec.y = mesh->mTextureCoords[0][i].y;
                        vertex.TexCoords = vec;
                    }
                else
                    vertex.TexCoords = glm::vec2(0.0f, 0.0f);
                newMesh.vertices_animated.push_back(vertex);
            }

            ExtractBoneWeightForVertices(newMesh.vertices_animated, mesh, scene);

            std::cout<<"ExtractingFaces"<<std::endl;
            //Faces - mesh triangles
            for(int i = 0; i < mesh->mNumFaces; i++){
                aiFace& face = mesh->mFaces[i];

                for(int j = 0; j< face.mNumIndices; j++){
                    newMesh.indices.push_back(face.mIndices[j]);
                }
            }

            for (int i = 0; i < newMesh.vertices_animated.size(); i++)
            {
                newMesh.vertices.push_back(newMesh.vertices_animated[i].Position);
                newMesh.normals.push_back(newMesh.vertices_animated[i].Normal);  
                newMesh.textures.push_back(newMesh.vertices_animated[i].TexCoords);

                glm::vec4 bone;
                for (int j = 0; j < MAX_BONE_INFLUENCE; j++)
                {
                    bone[j] = (float)newMesh.vertices_animated[i].m_BoneIDs[j];
                }
                newMesh.boneIDs.push_back(bone);

                glm::vec4 weight;
                for (int j = 0; j < MAX_BONE_INFLUENCE; j++)
                {
                    weight[j] = newMesh.vertices_animated[i].m_Weights[j];
                }
                newMesh.weights.push_back(weight);
            }

            meshes.push_back(newMesh);
        }
    }
}

std::vector<AnimatedMesh> AnimatedAssimpLoader::getMeshes(){
    return meshes;
}

void AnimatedAssimpLoader::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
{
    std::cout<<"Extracting bone weights"<<std::endl;
    for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
    {
        int boneID = -1;
        std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();

        //std::cout<<"Bone name: "<<boneName<<std::endl;
        if (m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end())
        {
            BoneInfo newBoneInfo;
            newBoneInfo.id = m_BoneCounter;
            newBoneInfo.offset = glm::mat4(1.0f);
            aiMatrix4x4t<ai_real> mat4 = (aiMatrix4x4t<float> &&) mesh->mBones[boneIndex]->mOffsetMatrix;
            newBoneInfo.offset[0][0] = mat4.a1;
            newBoneInfo.offset[0][1] = mat4.b1;
            newBoneInfo.offset[0][2] = mat4.c1;
            newBoneInfo.offset[0][3] = mat4.d1;
            newBoneInfo.offset[1][0] = mat4.a2;
            newBoneInfo.offset[1][1] = mat4.b2;
            newBoneInfo.offset[1][2] = mat4.c2;
            newBoneInfo.offset[1][3] = mat4.d2;
            newBoneInfo.offset[2][0] = mat4.a3;
            newBoneInfo.offset[2][1] = mat4.b3;
            newBoneInfo.offset[2][2] = mat4.c3;
            newBoneInfo.offset[2][3] = mat4.d3;
            newBoneInfo.offset[3][0] = mat4.a4;
            newBoneInfo.offset[3][1] = mat4.b4;
            newBoneInfo.offset[3][2] = mat4.c4;
            newBoneInfo.offset[3][3] = mat4.d4;
            m_BoneInfoMap[boneName] = newBoneInfo;
            boneID = m_BoneCounter;
            m_BoneCounter++;
        }
        else
        {
            boneID = m_BoneInfoMap[boneName].id;
        }
        assert(boneID != -1);
        auto weights = mesh->mBones[boneIndex]->mWeights;
        int numWeights = mesh->mBones[boneIndex]->mNumWeights;

        for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
        {
            int vertexId = weights[weightIndex].mVertexId;
            float weight = weights[weightIndex].mWeight;
            assert(vertexId <= vertices.size());
            SetVertexBoneData(vertices[vertexId], boneID, weight);
        }
    }
}