#include "mobobject.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

#include "shaderprogram.h"
#include "camera.h"
#include "assimploader.h"
#include "utility.h"

#include "resources.h"

MobObject::MobObject(GameGrid *gameGridPtr, int id) {
    gameGrid = gameGridPtr;
    m_id = id;
    tex = Resources::GetModelTexture(Resources::MOBOBJECT_TEXTURE);
    texSpecular = Resources::GetModelTexture(Resources::MOBOBJECT_TEXTURE_SPECULAR);
    mesh = Resources::GetAssimpModelMesh(Resources::MOBOBJECT_MODEL, 0);

    Rotate(-AI_MATH_PI/2, glm::vec3(1.0f, 0.0f, 0.0f));
    // Rotate(30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    SetScale(glm::vec3(0.05, 0.05f, 0.05f));
    auto pos = GetPosition();
    pos += glm::vec3(0, 1.f, 0.f);
    SetPosition(pos);
}

void MobObject::Draw(const Camera& camera) const {
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

void MobObject::Update(double deltaTime) {
    if (!active) return;

    // std::cerr << "Current Pos: " << currentPos.segment << " " << currentPos.timeLeft << std::endl;
    currentPos = advanceInTime(currentPos, deltaTime);
    // std::cerr << "Next Pos: " << currentPos.segment << " " << currentPos.timeLeft << std::endl;
    GamePosition nextGamePos = translateToGamePosition(currentPos); 
    // std::cerr << "Game Pos: " << nextGamePos.x << " " << nextGamePos.y << " " << nextGamePos.z << std::endl;
    SetPosition(nextGamePos);

    Rotate(AI_MATH_PI * deltaTime, glm::vec3(0, 0, 1));

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