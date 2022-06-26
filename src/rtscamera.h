#pragma once

#include <glm/matrix.hpp>
#include "camera.h"

class RTSCamera : public Camera
{
    private:
        glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 cameraDirection;

        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraRight;
        glm::vec3 cameraUp;

        glm::mat4 view;

        float radius = 5.0f;
        float camX = 0.0f, camZ = 0.0f;
        double time = 0.0f;
        glm::mat4 P;

        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

        bool blockRotation = false;
        float heightCap = 3.0f;
        float pitchCapLower = -70.0f, pitchCapUpper = -30.0f;
        bool useHeightCap = true;

    public:
        RTSCamera();
        RTSCamera(glm::vec3 startPosition);
        void CameraUpdate(double deltaTime) {};
        void MoveCamera(
            float frontSpeedMove,
            float rightSpeedMove, 
            float pitch, 
            float speed, 
            float roll, 
            float deltaTime
            ) override;

        void SetCameraRotationBlock(bool block) override;
        void SetCameraHeightCap(bool toggle, float cap) override;
        void ZoomCamera(float fov) override;
        

        glm::mat4 GetV() const override;
        glm::mat4 GetP() const override;
};