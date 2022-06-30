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

	auto pLights = camera.GetLights();
    for (int i = 0; i < pLights.size(); ++i) {
        std::string attrPrefix = "lights[" + std::to_string(i) + "].";

        auto& pLight = pLights[i];

        glUniform3fv(spTerrain->u((attrPrefix + "position").c_str()), 1, glm::value_ptr(pLight.position));
        glUniform3fv(spTerrain->u((attrPrefix + "ambient").c_str()), 1, glm::value_ptr(pLight.ambient));
        glUniform3fv(spTerrain->u((attrPrefix + "diffuse").c_str()), 1, glm::value_ptr(pLight.diffuse));
        glUniform3fv(spTerrain->u((attrPrefix + "specular").c_str()), 1, glm::value_ptr(pLight.specular));
        glUniform1f(spTerrain->u((attrPrefix + "A").c_str()), pLight.A);
        glUniform1f(spTerrain->u((attrPrefix + "B").c_str()), pLight.B);
        glUniform1f(spTerrain->u((attrPrefix + "C").c_str()), pLight.C);
    }



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

glm::vec3 GridObject::GridToWorld(Grid::GridPosition position) const {
	auto p = GetPosition() + glm::vec3(
		position.row * GameGrid::halfRowScale * 2 + GameGrid::halfRowScale,
		0.f,
		position.col * GameGrid::halfColScale * 2 + GameGrid::halfRowScale
	);

	return GetTransformMatrix() * glm::vec4(p, 1.0);
}
