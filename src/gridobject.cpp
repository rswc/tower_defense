#include "gridobject.h"

#include "shaderprogram.h"
#include "camera.h"

#include "resources.h"
#include "BaseMesh.h"
#include "gameobject.h"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

void GridObject::Draw(const Camera& camera) const {
    //Activate the shader
    spTerrain->use();

	glUniformMatrix4fv(spTerrain->u("P"), 1, false, glm::value_ptr(camera.GetP()));
	glUniformMatrix4fv(spTerrain->u("V"), 1, false, glm::value_ptr(camera.GetV()));
	glUniformMatrix4fv(spTerrain->u("M"), 1, false, glm::value_ptr(GetTransformMatrix()));

	glUniform3f(spTerrain->u("ambientColor"), 0.2f, 0.2f, 0.2f);
	glUniform3f(spTerrain->u("diffuseColor"), 0.5f, 0.5f, 0.5f);
	glUniform4f(spTerrain->u("lightDir"), 0.2f, -1.0f, 0.0f, 0.0f);

	auto pLight = camera.GetLights()[0];
	glUniform3fv(spTerrain->u("lights[0].position"), 1, glm::value_ptr(pLight.position));
	glUniform3fv(spTerrain->u("lights[0].ambient"), 1, glm::value_ptr(pLight.ambient));
	glUniform3fv(spTerrain->u("lights[0].diffuse"), 1, glm::value_ptr(pLight.diffuse));
	// specular is unused in the terrain shader
	glUniform1f(spTerrain->u("lights[0].A"), pLight.A);
	glUniform1f(spTerrain->u("lights[0].B"), pLight.B);
	glUniform1f(spTerrain->u("lights[0].C"), pLight.C);



	glEnableVertexAttribArray(spTerrain->a("vertex"));
	glVertexAttribPointer(spTerrain->a("vertex"), 4, GL_FLOAT, false, 0, m_mesh.vertices.data()); 

	glEnableVertexAttribArray(spTerrain->a("normal"));
	glVertexAttribPointer(spTerrain->a("normal"), 4, GL_FLOAT, false, 0, m_mesh.normals.data()); 

	glEnableVertexAttribArray(spTerrain->a("texParams"));
	glVertexAttribPointer(spTerrain->a("texParams"), 3, GL_FLOAT, false, 0, m_mesh.textures.data());

	glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_2D, Resources::ta_Terrain.id);
	glUniform1i(spTerrain->u("texMain"), 0);

	glDrawArrays(GL_TRIANGLES, 0, m_mesh.vertices.size());

    //Disable vertex attribute array
	glDisableVertexAttribArray(spTerrain->a("vertex"));
	glDisableVertexAttribArray(spTerrain->a("normal"));
	glDisableVertexAttribArray(spTerrain->a("texParams"));
}

void GridObject::Update(double deltaTime){}

Grid& GridObject::GetLogical() {
	return m_grid;
}

Plane GridObject::GetMousePickPlane() const {
	return m_plane.ToWorldSpace(GetTransformMatrix());
}

Grid::GridPosition GridObject::WorldToGrid(glm::vec3 position) const {
	auto origin = GetPosition();

	auto gp = Grid::GridPosition(
		(position.x - origin.x) / (GameGrid::halfRowScale * 2),
		(position.z - origin.z) / (GameGrid::halfColScale * 2)
	);

	return m_grid.isInsideGrid(gp) ? gp : Grid::GridPosition(-1, -1 );
}
