//
//  Random.cpp
//  Underneath
//
//  Created by Braeden Atlee on 2/18/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Global.hpp"
#include "Random.hpp"
#include "Geometry.hpp"

namespace Random {

    unsigned char predefinedRandomGrid[PREDEFINED_RANDOM_GRID_SIZE][PREDEFINED_RANDOM_GRID_SIZE];

    double randDouble(double min, double max) {
        return (((rand()%10000) / 10000.0)*(max - min))+min;
    }

    int randomFromPoint(Point2 seed){
        return predefinedRandomGrid[seed.x % PREDEFINED_RANDOM_GRID_SIZE][seed.y % PREDEFINED_RANDOM_GRID_SIZE];
    }

    void setup(){
        for(int i=0;i<PREDEFINED_RANDOM_GRID_SIZE;i++){
            for(int j=0;j<PREDEFINED_RANDOM_GRID_SIZE;j++){
                predefinedRandomGrid[i][j] = rand() % 256;
            }
        }
    }



}
