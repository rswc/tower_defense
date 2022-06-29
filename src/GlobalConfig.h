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

    //Functions
        void loadConfigFromFile(std::string filepath);
        void printWholeConfig();


};