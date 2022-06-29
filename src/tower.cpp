#include "tower.h"

#include <iostream>

void Tower::Update(double deltaTime) {
    if (init)
    {
        init = false;
        std::cout << "Tower is alive!" << std::endl;
    }
    
}

