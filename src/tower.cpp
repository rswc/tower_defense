#include "tower.h"

#include "resources.h"
#include "shaderprogram.h"
#include "GlobalConfig.h"
#include "bulletmanager.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Tower::Tower(BulletManager * bman, MobManager * mman) {
    bulletman = bman;
    mobman = mman;

    tex = Resources::GetModelTexture(Resources::TOWER_TEXTURE);
    mesh = Resources::GetAssimpModelMesh(Resources::TOWER_MODEL, 0);
    
    refreshTime = GlobalConfig::towerRefreshTime;
    shootTime = GlobalConfig::towerShootTime;
    radius = GlobalConfig::towerRadius;
    towerHeight = GlobalConfig::towerModelHeight;

    Rotate(-AI_MATH_PI/2, glm::vec3(1.0f, 0.0f, 0.0f));
    // Rotate(30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    SetScale(glm::vec3(0.2, 0.2f, 0.2f));
}

void Tower::shoot() {
    if (!ready) return;

    for (MobObject * mob : mobman->mobs) {
        if (!mob->isActive()) continue;

        if (!mob->shootable(shootTime)) { 
            continue;
        }
        if (!inRange(mob->getModelHitCoordinates(shootTime))) { 
            continue;
        }

        std::cout << "Shooting that mob " << mob->id() << std::endl;
        mob->markDead(shootTime);
        timeLeft = refreshTime;
        ready = false;
        bulletman->shootBullet(getShootPoint(), mob->getModelHitCoordinates(shootTime), shootTime);
        return;
    }
}


void Tower::Draw(const Camera& camera) const {
    if (!active) return;

    //Activate the shader
    spTextured->use();

    //TODO: move to Material class??
	glUniformMatrix4fv(spTextured->u("P"), 1, false, glm::value_ptr(camera.GetP()));
	glUniformMatrix4fv(spTextured->u("V"), 1, false, glm::value_ptr(camera.GetV()));
	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(GetTransformMatrix()));

    glUniform3fv(spTextured->u("cameraPos"), 1, glm::value_ptr(camera.GetPosition()));

    glUniform3f(spTextured->u("dirAmbientColor"), 0.2f, 0.2f, 0.2f);
	glUniform3f(spTextured->u("dirDiffuseColor"), 0.5f, 0.5f, 0.5f);
	glUniform4f(spTextured->u("lightDir"), 0.2f, -1.0f, 0.0f, 0.0f);

    auto pLights = camera.GetLights();
    for (int i = 0; i < pLights.size(); ++i) {
        std::string attrPrefix = "lights[" + std::to_string(i) + "].";

        auto& pLight = pLights[i];

        glUniform3fv(spTextured->u((attrPrefix + "position").c_str()), 1, glm::value_ptr(pLight.position));
        glUniform3fv(spTextured->u((attrPrefix + "ambient").c_str()), 1, glm::value_ptr(pLight.ambient));
        glUniform3fv(spTextured->u((attrPrefix + "diffuse").c_str()), 1, glm::value_ptr(pLight.diffuse));
        glUniform3fv(spTextured->u((attrPrefix + "specular").c_str()), 1, glm::value_ptr(pLight.specular));
        glUniform1f(spTextured->u((attrPrefix + "A").c_str()), pLight.A);
        glUniform1f(spTextured->u((attrPrefix + "B").c_str()), pLight.B);
        glUniform1f(spTextured->u((attrPrefix + "C").c_str()), pLight.C);
    }


    //TODO: pass to Material class?? make separate Mesh class for holding attributes??
	glEnableVertexAttribArray(spTextured->a("vertex"));
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, mesh->vertices.data()); 

        
	glEnableVertexAttribArray(spTextured->a("texCoord"));
	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, mesh->textures.data()); 

       
	glEnableVertexAttribArray(spTextured->a("normal"));
	glVertexAttribPointer(spTextured->a("normal"), 4, GL_FLOAT, false, 0, mesh->normals.data()); 


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glUniform1i(spTextured->u("tex"), 0);

    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, textSpecular);
    // glUniform1i(spTexturedSpecular->u("textureMap1"), 1);

    glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, mesh->indices.data());

    //Disable vertex attribute array
	glDisableVertexAttribArray(spTextured->a("vertex"));
	glDisableVertexAttribArray(spTextured->a("texCoord"));
	glDisableVertexAttribArray(spTextured->a("normal"));

}