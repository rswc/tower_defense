#include "StaticObject.h"
#include <glm/gtc/type_ptr.hpp>
#include "shaderprogram.h"
#include "resources.h"
#include "lodepng.h"

GLuint readTexture(const char* filename) {	
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

StaticObject::StaticObject(
    const std::string res_model,
    const std::string res_texture,
    const std::string res_specular
){
    // std::cout<<"StaticObject is being created"<<std::endl;
    // tex = Resources::GetModelTexture(Resources::GATEOBJECT_TEXTURE);
    // texSpecular = Resources::GetModelTexture(Resources::GATEOBJECT_TEXTURE_SPECULAR);
    // mesh = Resources::GetAssimpModelMesh(Resources::GATEOBJECT_MODEL,0);
    // std::cout<<"StaticObject created"<<std::endl;
    texture = readTexture(res_texture.c_str());
    textSpecular = readTexture(res_specular.c_str());

    auto loader = AssimpLoader();
    loader.loadModel(res_model.c_str());
    meshes = loader.getMeshes();
}

StaticObject::~StaticObject() {
    glDeleteTextures(1, &texture);
    glDeleteTextures(1, &textSpecular);
    glDeleteTextures(1, &textDiffuse);
}

void StaticObject::Draw(const Camera& camera) const {
    //Activate the shader
    spLambertTextured->use();

    //TODO: move to Material class??
	glUniformMatrix4fv(spLambertTextured->u("P"), 1, false, glm::value_ptr(camera.GetP()));
	glUniformMatrix4fv(spLambertTextured->u("V"), 1, false, glm::value_ptr(camera.GetV()));
	glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(GetTransformMatrix()));


    //TODO: pass to Material class?? make separate Mesh class for holding attributes??
	glEnableVertexAttribArray(spLambertTextured->a("vertex"));
	glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, meshes[0].vertices.data()); 

        
	glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
	glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, meshes[0].textures.data()); 

       
	glEnableVertexAttribArray(spLambertTextured->a("normal"));
	glVertexAttribPointer(spLambertTextured->a("normal"), 4, GL_FLOAT, false, 0, meshes[0].normals.data()); 


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(spLambertTextured->u("tex"), 0);

    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, textSpecular);
    // glUniform1i(spTexturedSpecular->u("textureMap1"), 1);

    glDrawElements(GL_TRIANGLES, meshes[0].indices.size(), GL_UNSIGNED_INT, meshes[0].indices.data());

    //Disable vertex attribute array
	glDisableVertexAttribArray(spLambertTextured->a("vertex"));
	glDisableVertexAttribArray(spLambertTextured->a("texCoord"));
	glDisableVertexAttribArray(spLambertTextured->a("normal"));

}
// void StaticObject::Draw(const Camera& camera) const {
//     spLambertTextured->use();

// 	glUniformMatrix4fv(spLambertTextured->u("P"), 1, false, glm::value_ptr(camera.GetP()));
// 	glUniformMatrix4fv(spLambertTextured->u("V"), 1, false, glm::value_ptr(camera.GetV()));
// 	glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(GetTransformMatrix()));

// 	glEnableVertexAttribArray(spLambertTextured->a("vertex"));
// 	glVertexAttribPointer(spLambertTextured->a("vertex"), 3, GL_FLOAT, false, 0, mesh->vertices.data()); 
        
// 	glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
// 	glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, mesh->textures.data()); 

// 	glEnableVertexAttribArray(spLambertTextured->a("normal"));
// 	glVertexAttribPointer(spLambertTextured->a("normal"), 3, GL_FLOAT, false, 0, mesh->normals.data()); 

//     glActiveTexture(GL_TEXTURE0);
//     glBindTexture(GL_TEXTURE_2D, tex);
//     glUniform1i(spLambertTextured->u("tex"), 0);

//     glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, mesh->indices.data());
    
//     //Disable vertex attribute array
// 	glDisableVertexAttribArray(spLambertTextured->a("vertex"));
// 	glDisableVertexAttribArray(spLambertTextured->a("texCoord"));
// 	glDisableVertexAttribArray(spLambertTextured->a("normal"));   
// }

