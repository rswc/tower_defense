#pragma once
#include <map>
#include <glm/glm.hpp>
#include <string>

namespace GlobalConfig{
    //Window
       extern int windowWidth;
        extern int windowHeight;

    //Mob 
        extern float mobTimePerFrame;
        extern float mobAnimationSpeed;
        extern std::string gridMapFilepath;

        extern int mobGroupSize;
        extern float mobGroupSpawnWaitPeriod;
        extern float mobSpawnTime;

    //Animations
        extern float globalAnimationSpeedMultiplier;

    //Settings
        extern float mouseSensitivity;
        extern float cameraHeightCap;
        extern float cameraMoveSpeed;
        extern float cameraStartX;
        extern float cameraStartZ;
        extern float cameraStartAngle;
        extern glm::vec3 cameraConstraintLU;
        extern glm::vec3 cameraConstraintRU;
        extern glm::vec3 cameraConstraintLD;
        extern glm::vec3 cameraConstraintRD;


    //Functions
        void loadConfigFromFile(std::string filepath);
        void printWholeConfig();


};