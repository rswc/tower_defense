#include "sillyanimatedobject.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "sillymaterial.h"
#include "shaderprogram.h"
#include "camera.h"
#include "OBJLoaderBly.h"


SillyAnimatedObject::SillyAnimatedObject() {
    // use resource manager?
    // material = std::make_shared<SillyMaterial>();
    std::cerr<<"Silly Animated Object Constructor Called"<<std::endl;
    

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

}

void SillyAnimatedObject::Draw(const Camera& camera) const {

    //TODO: make Transform class to handle this?
    glm::mat4 M = glm::mat4(1.0f);
	M = glm::rotate(M, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::rotate(M, 30.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    spConstant->use(); //Aktywuj program cieniujący

    //TODO: move to Material class??
	glUniformMatrix4fv(spConstant->u("P"), 1, false, glm::value_ptr(camera.GetP()));
	glUniformMatrix4fv(spConstant->u("V"), 1, false, glm::value_ptr(camera.GetV()));
	glUniformMatrix4fv(spConstant->u("M"), 1, false, glm::value_ptr(M));


    //TODO: pass to Material class?? make separate Mesh class for holding attributes??
	glEnableVertexAttribArray(spConstant->a("vertex"));
	glVertexAttribPointer(spConstant->a("vertex"), 4, GL_FLOAT, false, 0, vertices);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	glDisableVertexAttribArray(spConstant->a("vertex"));

}

