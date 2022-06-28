/*
Credits:
    Math in RTSCamera class is inspired by learnopengl tutorial. 
    Source: https://learnopengl.com/Getting-started/Camera
*/
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
        glm::vec3(0.0f, 0.0f, -1.0f), 
        glm::vec3(0.0f, 0.0f, 1.0f));
}

void RTSCamera::MoveCamera(
    float frontSpeedMove, 
    float rightSpeedMove, 
    float pitch, 
    float yaw, 
    float roll, 
    float deltaTime
    ){
    if (blockRotation == false){
        //RTS mode on
        if (useHeightCap){
            //Cast forward vector onto the xz plane
            cameraFront.y = 0;
            cameraFront = glm::normalize(cameraFront);

            if (pitch > pitchCapUpper)
                pitch = pitchCapUpper;
            if (pitch < pitchCapLower)
                pitch = pitchCapLower;
        }
    
        cameraPos += deltaTime * frontSpeedMove * cameraFront;
        cameraPos += deltaTime * rightSpeedMove * glm::normalize(glm::cross(cameraFront, cameraUp));
        
            //Mouse camera rotation
            glm::vec3 direction;
            direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            direction.y = sin(glm::radians(pitch));
            direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            cameraFront = glm::normalize(direction);
        
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }
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
    heightCap = cap;
    if(toggle){
        useHeightCap = !useHeightCap;
        cameraPos.y = heightCap;
    }
}

void RTSCamera::ZoomCamera(float fov){
    std::cerr<<"Zoom: "<<fov<<std::endl;
    P = glm::perspective(glm::radians(fov), 1.0f, 0.01f, 100.0f);
}
