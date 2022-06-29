/*
Credits:
    Math in RTSCamera class is inspired by learnopengl tutorial. 
    Source: https://learnopengl.com/Getting-started/Camera
*/
#include "rtscamera.h"
#include "GlobalConfig.h"
#include <GLFW/glfw3.h>
#include <iostream>

RTSCamera::RTSCamera(){
    RTSCamera(glm::vec3(0.0f, 0.0f, 3.0f));
    //Print camera constraints from GlobalConfig
    std::cout << "Camera Constraints: " << std::endl;
    std::cout << "Left-Up: " << GlobalConfig::cameraConstraintLU.x << ", " << GlobalConfig::cameraConstraintLU.y << std::endl;
}

RTSCamera::RTSCamera(glm::vec3 startPosition){
    cameraDirection = glm::normalize(cameraPos - cameraTarget);
    cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    cameraUp = glm::cross(cameraDirection, cameraRight);

    SetP(glm::perspective(
            glm::radians(50.0f), 
            (float)GlobalConfig::windowWidth/(float)GlobalConfig::windowHeight, 
            0.01f, 
            100.0f));

    view = glm::lookAt(
        startPosition, 
        glm::vec3(0.0f, 0.0f, -1.0f), 
        glm::vec3(0.0f, 0.0f, 1.0f));

    cameraPos = startPosition;
    SetPosition(startPosition);
}

void constrainCameraPosition(glm::vec3 &cameraPos){

    if(cameraPos.x < GlobalConfig::cameraConstraintLU.x){
        cameraPos.x = GlobalConfig::cameraConstraintLU.x;
        //std::cerr<<"Camera Constraint: Left-Up x"<<std::endl;
    }
    if(cameraPos.z < GlobalConfig::cameraConstraintLU.z){
        cameraPos.z = GlobalConfig::cameraConstraintLU.z;
        //std::cerr<<"Camera Constraint: Left-Up z"<<std::endl;
    }

    if(cameraPos.x > GlobalConfig::cameraConstraintRU.x){
        cameraPos.x = GlobalConfig::cameraConstraintRU.x;
        //std::cerr<<"Camera Constraint: Right-Up x"<<std::endl;
    }
    if(cameraPos.z < GlobalConfig::cameraConstraintRU.z){
        cameraPos.z = GlobalConfig::cameraConstraintRU.z;
        //std::cerr<<"Camera Constraint: Right-Up z"<<std::endl;
    }

    if(cameraPos.x < GlobalConfig::cameraConstraintLD.x){
        cameraPos.x = GlobalConfig::cameraConstraintLD.x;
        //std::cerr<<"Camera Constraint: Left-Down x"<<std::endl;
    }
    if(cameraPos.z > GlobalConfig::cameraConstraintLD.z){
        cameraPos.z = GlobalConfig::cameraConstraintLD.z;
        //std::cerr<<"Camera Constraint: Left-Down z"<<std::endl;
    }

    if(cameraPos.x > GlobalConfig::cameraConstraintRD.x){
        cameraPos.x = GlobalConfig::cameraConstraintRD.x;
        //std::cerr<<"Camera Constraint: Right-Down x"<<std::endl;
    }
    if(cameraPos.z > GlobalConfig::cameraConstraintRD.z){
        cameraPos.z = GlobalConfig::cameraConstraintRD.z;
        //std::cerr<<"Camera Constraint: Right-Down z"<<std::endl;
    }
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

        if(useHeightCap)
            constrainCameraPosition(cameraPos);

        // HACK: To make this 'workarodund' work with systems reliant on Transform
        SetPosition(cameraPos);
        
            //Mouse camera rotation
            glm::vec3 direction;
            direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            direction.y = sin(glm::radians(pitch));
            direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            cameraFront = glm::normalize(direction);
        
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }
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
        
        // HACK: To make this 'workarodund' work with systems reliant on Transform
        SetPosition(cameraPos);
    }
}

void RTSCamera::ZoomCamera(float fov){
        SetP(glm::perspective(
            glm::radians(fov), 
            (float)GlobalConfig::windowWidth/(float)GlobalConfig::windowHeight, 
            0.01f, 
            100.0f));
}
