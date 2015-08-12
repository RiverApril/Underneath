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

    unsigned char predefinedRandomGrid[64][64];

    double randDouble(double min, double max) {
        return min + ((rand() / (double) RAND_MAX) * (max - min));
    }

    int randomFromPoint(Point2 seed){
        return predefinedRandomGrid[seed.x][seed.y];
    }

    void setup(){
        for(int i=0;i<64;i++){
            for(int j=0;j<64;j++){
                predefinedRandomGrid[i][j] = rand() % 256;
            }
        }
    }



}
