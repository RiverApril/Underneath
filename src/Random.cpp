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

    double randDouble(double min, double max) {
        return min + ((rand() / (double) RAND_MAX) * (max - min));
    }

    int randomFromPoint(Point2 seed){
        int pair = ((seed.x*seed.x) + (3*seed.x) + (2*seed.x*seed.y) + seed.y + (seed.y*seed.y))/2;
        return ((pair)*1103515245+12345) % 2147483648;
    }



}
