#include "rtscamera.h"
#include <GLFW/glfw3.h>
#include <iostream>

RTSCamera::RTSCamera(){
    RTSCamera(glm::vec3(0.0f, 0.0f, 3.0f));
}

RTSCamera::RTSCamera(glm::vec3 startPosition){
    cameraDirection = glm::normalize(cameraPos - cameraTarget);
    cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    cameraUp = glm::cross(cameraDirection, cameraRight);

    P = glm::perspective(glm::radians(50.0f), 1.0f, 0.01f, 100.0f);

    view = glm::lookAt(
        startPosition, 
        glm::vec3(0.0f, 0.0f, 0.0f), 
        glm::vec3(0.0f, 1.0f, 0.0f));
}

void RTSCamera::CameraUpdate(double deltaTime){
    //-------------ORBITING----------------
    // time += deltaTime;
    // if (time >= 2 * 3.14159)
    //     time = 0; 

    // camX = sin(time) * radius;
    // camZ = cos(time) * radius;

    // view = glm::lookAt(
    //     glm::vec3(camX, 0.0, camZ), 
    //     glm::vec3(0.0, 0.0, 0.0), 
    //     glm::vec3(0.0, 1.0, 0.0));
    //--------------------------------------
}

void RTSCamera::MoveCamera(
    float frontSpeedMove, 
    float rightSpeedMove, 
    float pitch, 
    float yaw, 
    float roll, 
    float deltaTime
    ){
    
    cameraPos += deltaTime * frontSpeedMove * cameraFront;
    cameraPos += deltaTime * rightSpeedMove * glm::normalize(glm::cross(cameraFront, cameraUp));
    
    if (blockRotation == false){
        //Mouse camera rotation
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);
    }

    if (useHeightCap)
        cameraPos.y = heightCap;

    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    std::cout<<"yaw"<<yaw<<"pitch"<<pitch<<std::endl;
}

glm::mat4 RTSCamera::GetP() const{
    return P;
}

glm::mat4 RTSCamera::GetV() const{
    return view;
}

void RTSCamera::SetCameraRotationBlock(bool block){
    blockRotation = block;
}

void RTSCamera::SetCameraHeightCap(bool toggle, float cap){
    if(toggle)
        useHeightCap = !useHeightCap;
    heightCap = cap;
}

void RTSCamera::ZoomCamera(float fov){
    P = glm::perspective(glm::radians(fov), 1.0f, 0.01f, 100.0f);
}
