#include "gridobject.h"

#include "shaderprogram.h"
#include "camera.h"

#include "BaseMesh.h"
#include "gameobject.h"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

void GridObject::Draw(const Camera& camera) const {
    //Activate the shader
    spLambert->use();

    //TODO: move to Material class??
	glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(camera.GetP()));
	glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(camera.GetV()));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(GetTransformMatrix()));


    //TODO: pass to Material class?? make separate Mesh class for holding attributes??
	glEnableVertexAttribArray(spLambert->a("vertex"));
	glVertexAttribPointer(spLambert->a("vertex"), 4, GL_FLOAT, false, 0, m_mesh.vertices.data()); 

	glEnableVertexAttribArray(spLambert->a("normal"));
	glVertexAttribPointer(spLambert->a("normal"), 4, GL_FLOAT, false, 0, m_mesh.normals.data()); 

	glDrawArrays(GL_TRIANGLES, 0, m_mesh.vertices.size());

    //Disable vertex attribute array
	glDisableVertexAttribArray(spLambert->a("vertex"));
	glDisableVertexAttribArray(spLambert->a("normal"));

}

void GridObject::Update(double deltaTime){}