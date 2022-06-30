#include "mobobject.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

#include "shaderprogram.h"
#include "camera.h"
#include "assimploader.h"
#include "utility.h"

#include "Animator.h"
#include "resources.h"


MobObject::MobObject(GameGrid *gameGridPtr, int id) {
    gameGrid = gameGridPtr;
    m_id = id;
    tex = Resources::GetModelTexture(Resources::MOB_TEXTURE);
    texSpecular = Resources::GetModelTexture(Resources::MOB_TEXTURE_SPECULAR);
    mesh = Resources::GetMobAnimatedMesh(0);
    animator = Resources::GetNewMobAnimator();
    std::cout << "animator loaded? " << std::endl;

    // Rotate(-AI_MATH_PI/2, glm::vec3(1.0f, 0.0f, 0.0f));
    // Rotate(30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    SetScale(glm::vec3(0.1, 0.1, 0.1));
    auto pos = GetPosition();
    pos += glm::vec3(0, 1.f, 0.f);
    SetPosition(pos);
}

void MobObject::Draw(const Camera& camera) const {
    if (!active) return;

    spAnimated->use();

	glUniformMatrix4fv(spAnimated->u("P"), 1, false, glm::value_ptr(camera.GetP()));
	glUniformMatrix4fv(spAnimated->u("V"), 1, false, glm::value_ptr(camera.GetV()));
	glUniformMatrix4fv(spAnimated->u("M"), 1, false, glm::value_ptr(GetTransformMatrix()));

    std::vector<glm::mat4> &transforms = *animator->GetFinalBoneMatrices();
    for (int i = 0; i < transforms.size(); ++i){
        std::string finalBones = "finalBonesMatrices[" + std::to_string(i) + "]";
	    glUniformMatrix4fv(spAnimated->u(finalBones.c_str()), 1, false, glm::value_ptr(transforms[i]));
    }

    glUniform3fv(spAnimated->u("cameraPos"), 1, glm::value_ptr(camera.GetPosition()));

    glUniform3f(spAnimated->u("dirAmbientColor"), 0.2f, 0.2f, 0.2f);
	glUniform3f(spAnimated->u("dirDiffuseColor"), 0.5f, 0.5f, 0.5f);
	glUniform4f(spAnimated->u("lightDir"), 0.2f, -1.0f, 0.0f, 0.0f);

    auto pLights = camera.GetLights();
    for (int i = 0; i < pLights.size(); ++i) {
        std::string attrPrefix = "lights[" + std::to_string(i) + "].";

        auto& pLight = pLights[i];

        glUniform3fv(spAnimated->u((attrPrefix + "position").c_str()), 1, glm::value_ptr(pLight.position));
        glUniform3fv(spAnimated->u((attrPrefix + "ambient").c_str()), 1, glm::value_ptr(pLight.ambient));
        glUniform3fv(spAnimated->u((attrPrefix + "diffuse").c_str()), 1, glm::value_ptr(pLight.diffuse));
        glUniform3fv(spAnimated->u((attrPrefix + "specular").c_str()), 1, glm::value_ptr(pLight.specular));
        glUniform1f(spAnimated->u((attrPrefix + "A").c_str()), pLight.A);
        glUniform1f(spAnimated->u((attrPrefix + "B").c_str()), pLight.B);
        glUniform1f(spAnimated->u((attrPrefix + "C").c_str()), pLight.C);
    }

	glEnableVertexAttribArray(spAnimated->a("pos"));
	glVertexAttribPointer(spAnimated->a("pos"), 3, GL_FLOAT, false, 0, mesh->vertices.data()); 
        
	glEnableVertexAttribArray(spAnimated->a("tex"));
	glVertexAttribPointer(spAnimated->a("tex"), 2, GL_FLOAT, false, 0, mesh->textures.data()); 

	glEnableVertexAttribArray(spAnimated->a("norm"));
	glVertexAttribPointer(spAnimated->a("norm"), 3, GL_FLOAT, false, 0, mesh->normals.data()); 

    glEnableVertexAttribArray(spAnimated->a("boneIds"));
    //glVertexAttribIPointer(spAnimated->a("boneIds"), 4, GL_BYTE, 0, meshes[0].boneIDs.data());
    glVertexAttribPointer(spAnimated->a("boneIds"), 4, GL_FLOAT, false, 0, mesh->boneIDs.data());
    //GL_UNSIGNED_SHORT
    glEnableVertexAttribArray(spAnimated->a("weights"));
    //glVertexAttribIPointer(spAnimated->a("weights"), 4, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));
    glVertexAttribPointer(spAnimated->a("weights"), 4, GL_FLOAT, false, 0, mesh->weights.data());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glUniform1i(spAnimated->u("tex"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texSpecular);
    glUniform1i(spAnimated->u("texSpecular"), 1);

    glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, mesh->indices.data());
    
    //Disable vertex attribute array
	glDisableVertexAttribArray(spAnimated->a("pos"));
	glDisableVertexAttribArray(spAnimated->a("tex"));
	glDisableVertexAttribArray(spAnimated->a("norm"));   
    glDisableVertexAttribArray(spAnimated->a("boneIds"));
    glDisableVertexAttribArray(spAnimated->a("weights"));
}

/*
void MobObject::OldDraw(const Camera& camera) const {
    if (!active) return;

    //Activate the shader
    spLambertTextured->use();

    //TODO: move to Material class??
	glUniformMatrix4fv(spLambertTextured->u("P"), 1, false, glm::value_ptr(camera.GetP()));
	glUniformMatrix4fv(spLambertTextured->u("V"), 1, false, glm::value_ptr(camera.GetV()));
	glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(GetTransformMatrix()));


    //TODO: pass to Material class?? make separate Mesh class for holding attributes??
	glEnableVertexAttribArray(spLambertTextured->a("vertex"));
	glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, mesh->vertices.data()); 

        
	glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
	glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, mesh->textures.data()); 

       
	glEnableVertexAttribArray(spLambertTextured->a("normal"));
	glVertexAttribPointer(spLambertTextured->a("normal"), 4, GL_FLOAT, false, 0, mesh->normals.data()); 


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glUniform1i(spLambertTextured->u("tex"), 0);

    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, textSpecular);
    // glUniform1i(spTexturedSpecular->u("textureMap1"), 1);

    glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, mesh->indices.data());

    //Disable vertex attribute array
	glDisableVertexAttribArray(spLambertTextured->a("vertex"));
	glDisableVertexAttribArray(spLambertTextured->a("texCoord"));
	glDisableVertexAttribArray(spLambertTextured->a("normal"));

}
*/


void MobObject::Update(double deltaTime) {
    if (!active) return;
    else if (died()) {
        deactivate();
        return;
    }
    else if (willDie()) {
        timeToDie -= deltaTime;
    }
    
    animator->UpdateAnimation(deltaTime * animSpeed);

    // std::cerr << "Current Pos: " << currentPos.segment << " " << currentPos.timeLeft << std::endl;
    MobPosition nextPos = advanceInTime(currentPos, deltaTime);
    // std::cerr << "Next Pos: " << currentPos.segment << " " << currentPos.timeLeft << std::endl;
    GamePosition nextGamePos = translateToGamePosition(nextPos); 

    float rot_y = rotY - AI_MATH_PI;
    auto heading = glm::vec3(sin(rot_y), 0.f, cos(rot_y));
    auto diff = GetPosition() - nextGamePos;
    

    float dot = diff.x * heading.x + diff.z * heading.z;
    float det = diff.x * heading.z - diff.z * heading.x;

    float angle = atan2(det, dot);

    // std::cout << "Rotated: " << rot_y << ", heading: " << heading.x << ", " << heading.z; 
    // std::cout << " " << diff.x << " " << diff.z << ", rotateBY: " << angle << std::endl;

    if (fabs(angle) > EPS && fabs(angle - AI_MATH_PI) > EPS) CumulativeRotation(angle, glm::vec3(0, 1, 0));

    // std::cerr << "Game Pos: " << nextGamePos.x << " " << nextGamePos.y << " " << nextGamePos.z << std::endl;
    currentPos = nextPos;
    SetPosition(nextGamePos);


    if (finished()) {
        // restart();
        deactivate();
    }
}

// Truncates to last + 1 segment
MobObject::MobPosition MobObject::advanceInTime(MobObject::MobPosition curr, float time) const {
    int pathSegments = getPathSegments();

    MobPosition next;
    float segmentsFrac = curr.timeLeft + time / timePerSegment;
    next.timeLeft = segmentsFrac - floor(segmentsFrac);
    next.segment = std::min(curr.segment + (int) floor(segmentsFrac), pathSegments);
    return next;
}

glm::vec3 MobObject::getModelHitCoordinates(float afterTime = 0.0f) {
    MobPosition futurePosition = (afterTime < EPS ? currentPos : advanceInTime(currentPos, afterTime));
    GamePosition futureInGame = translateToGamePosition(futurePosition);
    futureInGame.y += modelHeight;
    return futureInGame;
}

// Assume: pos is valid in terms of path length
// -> timeLeft c [0, 1], segment c [0, pathSegments]
MobObject::GamePosition MobObject::translateToGamePosition(MobObject::MobPosition pos) {
    PathIndex pindex = translateToPathIndex(pos.segment);
    GamePosition bezierCurvePoint = quadraticBezierCurve(

        gameGrid->gamePathNode(pindex - 1),
        gameGrid->gamePathNode(pindex),
        gameGrid->gamePathNode(pindex + 1),
        pos.timeLeft
    );
    return bezierCurvePoint;
}