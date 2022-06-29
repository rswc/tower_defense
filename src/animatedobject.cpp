#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include "shaderprogram.h"
#include "camera.h"
#include "animatedobject.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

AnimatedObject::AnimatedObject(glm::vec3 targetModelScale, float targetAnimationSpeed)
{
    modelScale = targetModelScale;
    animationSpeed = targetAnimationSpeed;
    std::cerr<<"Animation Speed: "<<animationSpeed<<std::endl;
    startSetup();
}

AnimatedObject::AnimatedObject(){
    startSetup();
}

void AnimatedObject::startSetup(){
    
    std::string dir = "assets/";
    std::string modelName = "trex";
    std::string stexture =  dir + "/" + modelName + "/" + modelName +".png";
    std::string stextureSpecular =  dir + "/" + modelName + "/" + modelName +"_specular.png";
    std::string smodelPath = dir + "/" + modelName + "/" + modelName +".glb";
    
    texture = readTexture(stexture.c_str());
    textSpecular = readTexture(stextureSpecular.c_str());

    AnimatedAssimpLoader loader = AnimatedAssimpLoader();
    loader.loadModel(smodelPath);
    meshes = loader.getMeshes();

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(smodelPath, aiProcess_Triangulate);

    danceAnimation = new Animation(scene, &loader);
    animator = new Animator(danceAnimation);

    //Print max bone id and bone count
    std::cerr<<"Num of bones: "<<loader.GetBoneCount()<<std::endl;
    std::cerr<<"Animation speed: "<<animationSpeed<<std::endl;
}

AnimatedObject::~AnimatedObject(){
    glDeleteTextures(1, &texture);
    glDeleteTextures(1, &textSpecular);
    glDeleteTextures(1, &textDiffuse);
    delete animator;
    delete danceAnimation;
}

void AnimatedObject::Update(double deltaTime){
    animator->UpdateAnimation((float)deltaTime * animationSpeed);
}

void AnimatedObject::Draw(const Camera& camera) const {
    spAnimated->use();

	glUniformMatrix4fv(spAnimated->u("P"), 1, false, glm::value_ptr(camera.GetP()));
	glUniformMatrix4fv(spAnimated->u("V"), 1, false, glm::value_ptr(camera.GetV()));

    glm::mat4 M = GetTransformMatrix();
    M = glm::scale(M, modelScale);
	glUniformMatrix4fv(spAnimated->u("M"), 1, false, glm::value_ptr(M));

    std::vector<glm::mat4> &transforms = *animator->GetFinalBoneMatrices();
    for (int i = 0; i < transforms.size(); ++i){
        std::string finalBones = "finalBonesMatrices[" + std::to_string(i) + "]";
	    glUniformMatrix4fv(spAnimated->u(finalBones.c_str()), 1, false, glm::value_ptr(transforms[i]));
    }

    glUniform3fv(spAnimated->u("cameraPos"), 1, glm::value_ptr(camera.GetPosition()));

    glUniform3f(spAnimated->u("dirAmbientColor"), 0.2f, 0.2f, 0.2f);
	glUniform3f(spAnimated->u("dirDiffuseColor"), 0.5f, 0.5f, 0.5f);
	glUniform4f(spAnimated->u("lightDir"), 0.2f, -1.0f, 0.0f, 0.0f);

    auto pLight = camera.GetLights()[0];
	glUniform3fv(spAnimated->u("lights[0].position"), 1, glm::value_ptr(pLight.position));
	glUniform3fv(spAnimated->u("lights[0].ambient"), 1, glm::value_ptr(pLight.ambient));
	glUniform3fv(spAnimated->u("lights[0].diffuse"), 1, glm::value_ptr(pLight.diffuse));
	glUniform3fv(spAnimated->u("lights[0].specular"), 1, glm::value_ptr(pLight.specular));
	glUniform1f(spAnimated->u("lights[0].A"), pLight.A);
	glUniform1f(spAnimated->u("lights[0].B"), pLight.B);
	glUniform1f(spAnimated->u("lights[0].C"), pLight.C);


	glEnableVertexAttribArray(spAnimated->a("pos"));
	glVertexAttribPointer(spAnimated->a("pos"), 3, GL_FLOAT, false, 0, meshes[0].vertices.data()); 
        
	glEnableVertexAttribArray(spAnimated->a("tex"));
	glVertexAttribPointer(spAnimated->a("tex"), 2, GL_FLOAT, false, 0, meshes[0].textures.data()); 

       
	glEnableVertexAttribArray(spAnimated->a("norm"));
	glVertexAttribPointer(spAnimated->a("norm"), 3, GL_FLOAT, false, 0, meshes[0].normals.data()); 

    glEnableVertexAttribArray(spAnimated->a("boneIds"));
    //glVertexAttribIPointer(spAnimated->a("boneIds"), 4, GL_BYTE, 0, meshes[0].boneIDs.data());
    glVertexAttribPointer(spAnimated->a("boneIds"), 4, GL_FLOAT, false, 0, meshes[0].boneIDs.data());
    //GL_UNSIGNED_SHORT
    glEnableVertexAttribArray(spAnimated->a("weights"));
    //glVertexAttribIPointer(spAnimated->a("weights"), 4, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));
    glVertexAttribPointer(spAnimated->a("weights"), 4, GL_FLOAT, false, 0, meshes[0].weights.data());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(spAnimated->u("tex"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textSpecular);
    glUniform1i(spAnimated->u("texSpecular"), 1);

    // std::cout<<"Started drawing triangles"<<std::endl;
    glDrawElements(GL_TRIANGLES, meshes[0].indices.size(), GL_UNSIGNED_INT, meshes[0].indices.data());
    // std::cout<<"Finished drawing triangles"<<std::endl;
    
    //Disable vertex attribute array
	glDisableVertexAttribArray(spAnimated->a("pos"));
	glDisableVertexAttribArray(spAnimated->a("tex"));
	glDisableVertexAttribArray(spAnimated->a("norm"));   
    glDisableVertexAttribArray(spAnimated->a("boneIds"));
    glDisableVertexAttribArray(spAnimated->a("weights"));
}

void AnimatedObject::loadModel(std::string filename) {

}