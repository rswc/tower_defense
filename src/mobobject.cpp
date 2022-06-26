#include "mobobject.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

#include "shaderprogram.h"
#include "camera.h"
#include "assimploader.h"

MobObject::MobObject(GameGrid *gameGridPtr) : gameGrid(gameGridPtr) {
    Rotate(AI_MATH_PI/2, glm::vec3(1.0f, 0.0f, 0.0f));
    // Rotate(30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    auto pos = GetPosition();
    pos += glm::vec3(0, 1.f, 0.f);
    SetPosition(pos);

    texture = readTexture("assets/wall.png");
    textSpecular = readTexture("assets/wall_specular.png");

    auto loader = AssimpLoader();
    loader.loadModel("assets/wall.fbx");
    meshes = loader.getMeshes();
}

MobObject::~MobObject() {
    glDeleteTextures(1, &texture);
    glDeleteTextures(1, &textSpecular);
    glDeleteTextures(1, &textDiffuse);
}

void MobObject::Draw(const Camera& camera) const {
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

void MobObject::Update(double deltaTime) {

    static int i = 0;
    const float timePerSegment = 2.0f;
    static float timeInSegment = 0; 
    
    timeInSegment += deltaTime;
    while (timeInSegment > timePerSegment) {
        timeInSegment -= timePerSegment;
        i += 2;
        
        if (i + 2 >= gameGrid->gamePath.points.size()) {
            i = 0;
        }
    }

    
    // lerp:
    auto p1 = gameGrid->gamePath.points[i];
    auto p2 = gameGrid->gamePath.points[i + 1];
    auto p3 = gameGrid->gamePath.points[i + 2];
    
    // [0, 1)
    float normTimeInSegment = timeInSegment / timePerSegment;
    
    auto p12 = p2 * normTimeInSegment + p1 * (1 - normTimeInSegment);
    auto p23 = p3 * normTimeInSegment + p2 * (1 - normTimeInSegment);

    auto p123 = p23 * normTimeInSegment + p12 * (1 - normTimeInSegment);
    // std::cerr << "Setting Position to: " << p123.x << " " << p123.z << ", " << i << ", " << timeInSegment << std::endl;
    auto pos = p123 + glm::vec4(0, 0.5f, 0, 0);
    SetPosition(pos);
}

GLuint MobObject::readTexture(const char* filename) {	
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