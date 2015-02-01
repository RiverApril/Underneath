//
//  Math.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/18/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Math.h"
#include "Global.h"

namespace Math {

    double distanceSquared(double ax, double ay, double bx, double by) {
        double dx = abs(ax - bx);
        double dy = abs(ay - by);

        return dx*dx + dy*dy;
    }

    double distanceSquared(double x, double y) {
        double dx = abs(x);
        double dy = abs(y);

        return dx*dx + dy*dy;
    }

    double randomRange(double min, double max){
        if(min > max){
            return randomRange(max, min);
        }else{
            return ((rand()/(double)RAND_MAX)*(max-min))+min;
        }
    }

    int roundToInt(double a){
        return (int)round(a);
    }

}
