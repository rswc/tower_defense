#include "GlobalConfig.h"
#include <iostream>
#include <fstream>

//Functions
namespace GlobalConfig{
        //Window
        int windowWidth;
        int windowHeight;

    //Mob 
        float mobTimePerFrame;
        float mobAnimationSpeed;
        std::string gridMapFilepath;

        int mobGroupSize;
        float mobGroupSpawnWaitPeriod;
        float mobSpawnTime;

    //Animations
        float globalAnimationSpeedMultiplier;

    //Parse config.txt file into maps of proper variable types
    //Examples of use:
    // windowWidth float 600.0 -> windowWidth = 600.0f
    // mobGroupSize int 600 ->  mobGroupSize = 600
    // gridMapFilepath string grid.txt -> gridMapFilepath = "grid.txt"
    // #comment -> print out comment
    void loadConfigFromFile(std::string filepath){
        std::ifstream file(filepath);
        std::string line;
        while(std::getline(file, line)){
            if(line[0] == '#'){
                std::cout << line << std::endl;
            }
            else{
                std::string key = line.substr(0, line.find(' '));
                std::string value = line.substr(line.find(' ') + 1);
                if(key == "windowWidth"){
                    windowWidth = std::stof(value);
                }
                else if(key == "windowHeight"){
                    windowHeight = std::stof(value);
                }
                else if(key == "mobTimePerFrame"){
                    mobTimePerFrame = std::stof(value);
                }
                else if(key == "mobAnimationSpeed"){
                    mobAnimationSpeed = std::stof(value);
                }
                else if(key == "gridMapFilepath"){
                    gridMapFilepath = value;
                }
                else if(key == "mobGroupSize"){
                    mobGroupSize = std::stoi(value);
                }
                else if(key == "mobGroupSpawnWaitPeriod"){
                    mobGroupSpawnWaitPeriod = std::stof(value);
                }
                else if(key == "mobSpawnTime"){
                    mobSpawnTime = std::stof(value);
                }
                else if(key == "globalAnimationSpeedMultiplier"){
                    globalAnimationSpeedMultiplier = std::stof(value);
                }
                else{
                    std::cout << "Error: Unknown key in config file: " << key << std::endl;
                }
            }
        }
    }

    void printWholeConfig(){
        std::cout << "windowWidth: " << windowWidth << std::endl;
        std::cout << "windowHeight: " << windowHeight << std::endl;
        std::cout << "mobTimePerFrame: " << mobTimePerFrame << std::endl;
        std::cout << "mobAnimationSpeed: " << mobAnimationSpeed << std::endl;
        std::cout << "gridMapFilepath: " << gridMapFilepath << std::endl;
        std::cout << "mobGroupSize: " << mobGroupSize << std::endl;
        std::cout << "mobGroupSpawnWaitPeriod: " << mobGroupSpawnWaitPeriod << std::endl;
        std::cout << "mobSpawnTime: " << mobSpawnTime << std::endl;
        std::cout << "globalAnimationSpeedMultiplier: " << globalAnimationSpeedMultiplier << std::endl;
    }
}

