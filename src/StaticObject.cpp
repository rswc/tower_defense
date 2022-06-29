#include "StaticObject.h"
#include <glm/gtc/type_ptr.hpp>
#include "shaderprogram.h"
#include "resources.h"

StaticObject::StaticObject(
    const std::string res_model,
    const std::string res_texture,
    const std::string res_specular
){
    std::cout<<"StaticObject is being created"<<std::endl;
    tex = Resources::GetModelTexture(Resources::GATEOBJECT_TEXTURE);
    texSpecular = Resources::GetModelTexture(Resources::GATEOBJECT_TEXTURE_SPECULAR);
    mesh = Resources::GetAssimpModelMesh(Resources::GATEOBJECT_MODEL, 0);
    std::cout<<"StaticObject created"<<std::endl;
}

void StaticObject::Draw(const Camera& camera) const {
    spLambertTextured->use();

	glUniformMatrix4fv(spLambertTextured->u("P"), 1, false, glm::value_ptr(camera.GetP()));
	glUniformMatrix4fv(spLambertTextured->u("V"), 1, false, glm::value_ptr(camera.GetV()));
	glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(GetTransformMatrix()));

	glEnableVertexAttribArray(spLambertTextured->a("vertex"));
	glVertexAttribPointer(spLambertTextured->a("vertex"), 3, GL_FLOAT, false, 0, mesh->vertices.data()); 
        
	glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
	glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, mesh->textures.data()); 

	glEnableVertexAttribArray(spLambertTextured->a("normal"));
	glVertexAttribPointer(spLambertTextured->a("normal"), 3, GL_FLOAT, false, 0, mesh->normals.data()); 

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glUniform1i(spLambertTextured->u("tex"), 0);

    glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, mesh->indices.data());
    
    //Disable vertex attribute array
	glDisableVertexAttribArray(spLambertTextured->a("vertex"));
	glDisableVertexAttribArray(spLambertTextured->a("texCoord"));
	glDisableVertexAttribArray(spLambertTextured->a("normal"));   
}
