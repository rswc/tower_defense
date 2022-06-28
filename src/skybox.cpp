#include "skybox.h"

#include "shaderprogram.h"
#include "resources.h"

#include <glm/gtc/type_ptr.hpp>

void Skybox::Draw(const Camera& camera) const {
    spSkybox->use();
    glDepthFunc(GL_LEQUAL);

    glUniformMatrix4fv(spSkybox->u("P"), 1, false, glm::value_ptr(camera.GetP()));
	glUniformMatrix4fv(spSkybox->u("V"), 1, false, glm::value_ptr(glm::mat4(glm::mat3(camera.GetV()))));

	glEnableVertexAttribArray(spSkybox->a("vertex"));
	glVertexAttribPointer(spSkybox->a("vertex"), 3, GL_FLOAT, false, 0, mesh); 

	glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_2D, Resources::cmp_Skybox.id);
	glUniform1i(spSkybox->u("skybox"), 0);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDisableVertexAttribArray(spSkybox->a("vertex"));
    glDepthFunc(GL_LESS);
}

GameObject::DrawOrderPosition Skybox::GetOrder(const Camera& camera) const {
    return { LAYER_SKYBOX, 0.0f };
}

