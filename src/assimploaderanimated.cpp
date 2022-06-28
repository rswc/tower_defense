#include "assimploaderanimated.h"

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
glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from)
{
    glm::mat4 to;
    //the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
    to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
    to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
    to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
    to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
    return to;
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
            std::cout<<"Loading mesh"<<std::endl;

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
                newMesh.boneIDs.push_back(newMesh.vertices_animated[i].m_BoneIDs);
                newMesh.weights.push_back(newMesh.vertices_animated[i].m_Weights);
            }

            meshes.push_back(newMesh);
        }
    }
    //Get mesh assuming there is only one mesh in the scene
   // aiMesh* mesh = scene->mMeshes[0]; 
}

std::vector<AnimatedMesh> AnimatedAssimpLoader::getMeshes(){
    return meshes;
}

void AnimatedAssimpLoader::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
{
    for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
    {
        int boneID = -1;
        std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
        if (m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end())
        {
            BoneInfo newBoneInfo;
            newBoneInfo.id = m_BoneCounter;
            newBoneInfo.offset = ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
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