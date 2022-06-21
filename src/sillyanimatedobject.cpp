

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>


#include "sillyanimatedobject.h"
#include "shaderprogram.h"
#include "camera.h"
#include "lodepng.h"
#include "OBJLoaderBly.h"



GLuint readTexture(const char* filename) { //Deklaracja globalna
 	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image; //Alokuj wektor do wczytania obrazka
	unsigned width, height; //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);
	//Import do pamięci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return tex;
}

SillyAnimatedObject::SillyAnimatedObject() {
    // use resource manager?
    // material = std::make_shared<SillyMaterial>();
    std::cerr<<"Silly Animated Object Constructor Called"<<std::endl;

    tex = readTexture("assets\\pizzabox.png");
    

    //LOAD OBJ FROM FILE 
    objl::Loader loader;
    loader.LoadFile("assets\\pizzabox.obj"); //object must exist in bin/assets folder
    std::vector<objl::Vertex> vertex_vector = loader.LoadedMeshes[0].Vertices;
    std::cerr<<loader.LoadedMeshes[0].MeshName<<std::endl;
    std::cerr<<"Silly Animated Object Created"<<std::endl;


    for(int i = 0; i < 204; i++)
    {
        vertices[i][0] = vertex_vector[i].Position.X;
        vertices[i][1] = vertex_vector[i].Position.Y;
        vertices[i][2] = vertex_vector[i].Position.Z;
        vertices[i][3] = 1.0f;
    }
    for(int i = 0; i < 204; i++)
    {
        normals[i][0] = vertex_vector[i].Normal.X;
        normals[i][1] = vertex_vector[i].Normal.Y;
        normals[i][2] = vertex_vector[i].Normal.Z;
        normals[i][3] = 1.0f;
    }
    for(int i = 0; i < 556; i++)
    {
        texCoords[i][0] = vertex_vector[i].TextureCoordinate.X;
        texCoords[i][1] = vertex_vector[i].TextureCoordinate.Y;
    }

}
void SillyAnimatedObject::Update(double deltaTime) {

    //Rotate continously
    if(angleY > 360.0f)
        angleY = 0.0f;
    angleY = angleY + 2.0f * (float)deltaTime;
}

void SillyAnimatedObject::Draw(const Camera& camera) const {

    //TODO: make Transform class to handle this?
    glm::mat4 M = glm::mat4(1.0f);
	M = glm::rotate(M, angleY, glm::vec3(0.0f, 1.0f, 0.0f));
	// M = glm::rotate(M, 30.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    spTextured->use(); //Aktywuj program cieniujący

    //TODO: move to Material class??
	glUniformMatrix4fv(spTextured->u("P"), 1, false, glm::value_ptr(camera.GetP()));
	glUniformMatrix4fv(spTextured->u("V"), 1, false, glm::value_ptr(camera.GetV()));
	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M));


    //TODO: pass to Material class?? make separate Mesh class for holding attributes??
	glEnableVertexAttribArray(spTextured->a("vertex"));
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, vertices);

    // glEnableVertexAttribArray(spTextured->a("normal"));
	// glVertexAttribPointer(spTextured->a("normal"), 4, GL_FLOAT, false, 0, normals);

    glEnableVertexAttribArray(spTextured->a("texCoord"));
	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, texCoords);
    glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(spTextured->u("tex"), 0);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	glDisableVertexAttribArray(spTextured->a("vertex"));
	//glDisableVertexAttribArray(spTextured->a("normal"));
	glDisableVertexAttribArray(spTextured->a("texCoord"));

}

