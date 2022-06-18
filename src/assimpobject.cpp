#include "assimpobject.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

#include "sillymaterial.h"
#include "shaderprogram.h"
#include "camera.h"


AssimpObject::AssimpObject() {
    Rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    Rotate(30.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    texture = readTexture("assets/wall.png");
    loadModel("assets/wall.fbx");
}

AssimpObject::~AssimpObject() {
    glDeleteTextures(1, &texture);
}

void AssimpObject::Draw(const Camera& camera) const {
    //Activate the shader
    spLambertTextured->use();

    //TODO: move to Material class??
	glUniformMatrix4fv(spLambertTextured->u("P"), 1, false, glm::value_ptr(camera.GetP()));
	glUniformMatrix4fv(spLambertTextured->u("V"), 1, false, glm::value_ptr(camera.GetV()));
	glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(GetTransformMatrix()));


    //TODO: pass to Material class?? make separate Mesh class for holding attributes??
	glEnableVertexAttribArray(spLambertTextured->a("vertex"));
	glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, objectMesh.vertices.data()); 

        
	glEnableVertexAttribArray(spLambertTextured->a("texCoords"));
	glVertexAttribPointer(spLambertTextured->a("texCoords"), 2, GL_FLOAT, false, 0, objectMesh.textures.data()); 

       
	glEnableVertexAttribArray(spLambertTextured->a("normal"));
	glVertexAttribPointer(spLambertTextured->a("normal"), 4, GL_FLOAT, false, 0, objectMesh.normals.data()); 


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(spLambertTextured->u("tex"), 0);

    glDrawElements(GL_TRIANGLES, objectMesh.indices.size(), GL_UNSIGNED_INT, objectMesh.indices.data());

    //Disable vertex attribute array
	glDisableVertexAttribArray(spLambertTextured->a("vertex"));
	glDisableVertexAttribArray(spLambertTextured->a("texCoords"));
	glDisableVertexAttribArray(spLambertTextured->a("normal"));

}

void AssimpObject::Update(double deltaTime) {
    //pass
}


void AssimpObject::loadModel(std::string filename) {

    Assimp::Importer importer;

    //aiProcess_triangulate -> make triangles
    //aiProcess_FlipUVs -> flip y coordinate on textures 
    //[opengl format is different than everywhere else] v = 1 - v 
    // aiProcess_GenSmoothNormals -> generate smooth normals
    // pointer is automatically deleted when importer is destroyed
    const aiScene* scene = importer.ReadFile(filename.c_str(), 
    aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals); 

    std::cout<<importer.GetErrorString()<<std::endl;

    // if (scene->HasMeshes()){
    //     for(int i = 0; i< scene->mNumMeshes; i++)
    //     {
    //         aiMesh* mesh = scene->mMeshes[i]; //Get specific mesh
    //     }
    // }

    //Get mesh assuming there is only one mesh in the scene
    aiMesh* mesh = scene->mMeshes[0]; 
    std::cout<<"Loading mesh"<<std::endl;
    for(int i = 0; i< mesh->mNumVertices; i++){
        aiVector3D vertex = mesh->mVertices[i];
        //std::cout<<vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
        objectMesh.vertices.push_back(glm::vec4(vertex.x, vertex.y, vertex.z, 1.0f));

        aiVector3D normal = mesh->mNormals[i];
        //std::cout<<normal.x << " " << normal.y << " " << normal.z << std::endl;
        objectMesh.normals.push_back(glm::vec4(normal.x, normal.y, normal.z, 0.0f));


        //Number of defined sets of texture coordinates (max 8)
        unsigned int number_of_sets = mesh->GetNumUVChannels();
        unsigned int dim_coord_tex = mesh->mNumUVComponents[0];

        //Get texture coordinates e.g if 3D then all x y z are defined
        aiVector3D texCoord = mesh->mTextureCoords[0][i]; 
        //std::cout<<texCoord.x << " " << texCoord.y << " " <<std::endl;
        //texCoords.push_back(glm::vec4(texCoord.x, texCoord.y, texCoord.z, 0.0f));
        objectMesh.textures.push_back(glm::vec2(texCoord.x, texCoord.y));
    }

    std::cout<<"ExtractingFaces"<<std::endl;
    //Faces - mesh triangles
    for(int i = 0; i < mesh->mNumFaces; i++){
        aiFace& face = mesh->mFaces[i];

        for(int j = 0; j< face.mNumIndices; j++){
            objectMesh.indices.push_back(face.mIndices[j]);
            //std::cout<<face.mIndices[j]<<" ";
        }
        //std::cout<<std::endl;
    }
}


GLuint AssimpObject::readTexture(const char* filename) {	
  GLuint tex;	
  glActiveTexture(GL_TEXTURE0); 	

  //Wczytanie do pamięci komputera
  std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
  unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
  //Wczytaj obrazek
  unsigned error = lodepng::decode(image, width, height, filename);
 
  //Import do pamięci karty graficznej
  glGenTextures(1,&tex); //Zainicjuj jeden uchwyt
  glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
  //Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
  glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
    GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());	

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	

  return tex;
} 