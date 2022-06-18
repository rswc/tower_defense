#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <iostream>
#include "BaseMesh.h"

//Loader class utilizing assimp to load models and store them in a vector of BaseMesh objects
//Can be used as Singleton
class AssimpLoader{
    private:
        std::vector<BaseMesh> meshes;
    public:
        void loadModel(std::string filename);
        std::vector<BaseMesh> getMeshes();
        void clear();
};