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
    tex = Resources::GetModelTexture(Resources::MOBOBJECT_TEXTURE);
    // texSpecular = Resources::GetModelTexture(Resources::MOBOBJECT_TEXTURE_SPECULAR);
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
    
    animator->UpdateAnimation(deltaTime * animSpeed);

    // std::cerr << "Current Pos: " << currentPos.segment << " " << currentPos.timeLeft << std::endl;
    currentPos = advanceInTime(currentPos, deltaTime);
    // std::cerr << "Next Pos: " << currentPos.segment << " " << currentPos.timeLeft << std::endl;
    GamePosition nextGamePos = translateToGamePosition(currentPos); 
    // std::cerr << "Game Pos: " << nextGamePos.x << " " << nextGamePos.y << " " << nextGamePos.z << std::endl;
    SetPosition(nextGamePos);

    //Rotate(AI_MATH_PI * deltaTime, glm::vec3(0, 0, 1));

    if (finished()) {
        // restart();
        deactivate();
    }
}

// Truncates to last + 1 segment
MobObject::MobPosition MobObject::advanceInTime(MobObject::MobPosition curr, float time) {
    int pathSegments = getPathSegments();

    MobPosition next;
    float segmentsFrac = curr.timeLeft + time / timePerSegment;
    next.timeLeft = segmentsFrac - floor(segmentsFrac);
    next.segment = std::min(curr.segment + (int) floor(segmentsFrac), pathSegments);
    return next;
}

MobObject::GamePosition MobObject::getModelHitCoordinates(float afterTime = 0.0f) {
    MobPosition futurePosition = (afterTime < EPS ? currentPos : advanceInTime(currentPos, afterTime));
    GamePosition futureInGame = translateToGamePosition(futurePosition);
    futureInGame.z += modelHeight;
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