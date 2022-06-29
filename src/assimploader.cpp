#include "assimploader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void AssimpLoader::loadModel(std::string filename){
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
        for(int i = 0; i< scene->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[i]; //Get specific mesh
            BaseMesh newMesh;
            std::cout<<"Loading mesh"<<std::endl;

            for(int i = 0; i< mesh->mNumVertices; i++){ 
                aiVector3D vertex = mesh->mVertices[i];
                //std::cout<<vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
                newMesh.vertices.push_back(glm::vec4(vertex.x, vertex.y, vertex.z, 1.0f));

                aiVector3D normal = mesh->mNormals[i];
                //std::cout<<normal.x << " " << normal.y << " " << normal.z << std::endl;
                newMesh.normals.push_back(glm::vec4(normal.x, normal.y, normal.z, 0.0f));


                //Number of defined sets of texture coordinates (max 8)
                unsigned int number_of_sets = mesh->GetNumUVChannels();
                unsigned int dim_coord_tex = mesh->mNumUVComponents[0];

                //Get texture coordinates e.g if 3D then all x y z are defined
                aiVector3D texCoord = mesh->mTextureCoords[0][i]; 
                //std::cout<<texCoord.x << " " << texCoord.y << " " <<std::endl;
                //texCoords.push_back(glm::vec4(texCoord.x, texCoord.y, texCoord.z, 0.0f));
                newMesh.textures.push_back(glm::vec2(texCoord.x, texCoord.y));
            }

            std::cout<<"ExtractingFaces"<<std::endl;
            //Faces - mesh triangles
            for(int i = 0; i < mesh->mNumFaces; i++){
                aiFace& face = mesh->mFaces[i];

                for(int j = 0; j< face.mNumIndices; j++){
                    newMesh.indices.push_back(face.mIndices[j]);
                    //std::cout<<face.mIndices[j]<<" ";
                }
                //std::cout<<std::endl;
            }
            meshes.push_back(newMesh);
        }
    }
    //Get mesh assuming there is only one mesh in the scene
   // aiMesh* mesh = scene->mMeshes[0]; 
}

std::vector<BaseMesh> AssimpLoader::getMeshes(){
    return meshes;
}

void  AssimpLoader::clear(){
    meshes.clear();
}