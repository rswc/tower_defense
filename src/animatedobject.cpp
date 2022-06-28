#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include "shaderprogram.h"
#include "camera.h"
#include "animatedobject.h"

auto loader = AnimatedAssimpLoader();
Animation danceAnimation = Animation("assets/vampire.dae", &loader);
Animator animator = Animator(&danceAnimation);

AnimatedObject::AnimatedObject(){
    Rotate(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    Rotate(30.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    texture = readTexture("assets/Vampire_diffuse.png");
    textSpecular = readTexture("assets/Vampire_specular.png");


    loader.loadModel("assets/vampire.dae");
    meshes = loader.getMeshes();
}

AnimatedObject::~AnimatedObject(){
    glDeleteTextures(1, &texture);
    glDeleteTextures(1, &textSpecular);
    glDeleteTextures(1, &textDiffuse);
}

void AnimatedObject::Update(double deltaTime){
    animator.UpdateAnimation((float)deltaTime);
}

void AnimatedObject::Draw(const Camera& camera) const {
     //Activate the shader
    spAnimated->use();

    //TODO: move to Material class??
	glUniformMatrix4fv(spAnimated->u("P"), 1, false, glm::value_ptr(camera.GetP()));
	glUniformMatrix4fv(spAnimated->u("V"), 1, false, glm::value_ptr(camera.GetV()));
	glUniformMatrix4fv(spAnimated->u("M"), 1, false, glm::value_ptr(GetTransformMatrix()));

    auto transforms = animator.GetFinalBoneMatrices();
    for (int i = 0; i < transforms.size(); ++i){
        std::string finalBones = "finalBonesMatrices[" + std::to_string(i) + "]";
	    glUniformMatrix4fv(spAnimated->u(finalBones.c_str()), 1, false, glm::value_ptr(transforms[i]));
    }

    //TODO: pass to Material class?? make separate Mesh class for holding attributes??
	glEnableVertexAttribArray(spAnimated->a("pos"));
	glVertexAttribPointer(spAnimated->a("pos"), 4, GL_FLOAT, false, 0, meshes[0].vertices.data()); 
        
	glEnableVertexAttribArray(spAnimated->a("tex"));
	glVertexAttribPointer(spAnimated->a("tex"), 2, GL_FLOAT, false, 0, meshes[0].textures.data()); 

       
	glEnableVertexAttribArray(spAnimated->a("norm"));
	glVertexAttribPointer(spAnimated->a("norm"), 4, GL_FLOAT, false, 0, meshes[0].normals.data()); 

    glEnableVertexAttribArray(spAnimated->a("boneIds"));
    //glVertexAttribIPointer(spAnimated->a("boneIds"), 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));
    glVertexAttribPointer(spAnimated->a("boneIds"), 4, GL_INT, false, 0, meshes[0].boneIDs.data());
    
    glEnableVertexAttribArray(spAnimated->a("weights"));
    //glVertexAttribIPointer(spAnimated->a("weights"), 4, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));
    glVertexAttribPointer(spAnimated->a("weights"), 4, GL_FLOAT, false, 0, meshes[0].weights.data());


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(spAnimated->u("tex"), 0);

    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, textSpecular);
    // glUniform1i(spTexturedSpecular->u("textureMap1"), 1);

    glDrawElements(GL_TRIANGLES, meshes[0].indices.size(), GL_UNSIGNED_INT, meshes[0].indices.data());

    //Disable vertex attribute array
	glDisableVertexAttribArray(spAnimated->a("pos"));
	glDisableVertexAttribArray(spAnimated->a("tex"));
	glDisableVertexAttribArray(spAnimated->a("norm"));   
    glDisableVertexAttribArray(spAnimated->a("boneIds"));
    glDisableVertexAttribArray(spAnimated->a("weights"));
}

void AnimatedObject::loadModel(std::string filename) {

}