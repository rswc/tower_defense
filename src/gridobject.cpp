#include "gridobject.h"

#include "shaderprogram.h"
#include "camera.h"

#include "BaseMesh.h"
#include "gameobject.h"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

void GridObject::Draw(const Camera& camera) const {
    //Activate the shader
    spWireframe->use();

    //TODO: move to Material class??
	glUniformMatrix4fv(spWireframe->u("P"), 1, false, glm::value_ptr(camera.GetP()));
	glUniformMatrix4fv(spWireframe->u("V"), 1, false, glm::value_ptr(camera.GetV()));
	glUniformMatrix4fv(spWireframe->u("M"), 1, false, glm::value_ptr(GetTransformMatrix()));


    //TODO: pass to Material class?? make separate Mesh class for holding attributes??
	glEnableVertexAttribArray(spWireframe->a("vertex"));
	glVertexAttribPointer(spWireframe->a("vertex"), 4, GL_FLOAT, false, 0, m_mesh.vertices.data()); 

	glEnableVertexAttribArray(spWireframe->a("normal"));
	glVertexAttribPointer(spWireframe->a("normal"), 4, GL_FLOAT, false, 0, m_mesh.normals.data()); 

	glDrawArrays(GL_TRIANGLES, 0, m_mesh.vertices.size());

    //Disable vertex attribute array
	glDisableVertexAttribArray(spWireframe->a("vertex"));
	glDisableVertexAttribArray(spWireframe->a("normal"));

}

void GridObject::Update(double deltaTime){}