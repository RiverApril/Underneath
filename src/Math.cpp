//
//  Math.cpp
//  Underneath
//
//  Created by Emily Atlee on 10/18/14.
//  Copyright (c) 2014 Emily Atlee. All rights reserved.
//

#include "Math.hpp"
#include "Global.hpp"

double distanceSquared(double ax, double ay, double bx, double by) {
    double dx = abs(ax - bx);
    double dy = abs(ay - by);

    return dx * dx + dy*dy;
}

double distanceSquared(double x, double y) {
    double dx = abs(x);
    double dy = abs(y);

    return dx * dx + dy*dy;
}

double randomRange(double min, double max, int div) {
    if (min > max) {
        return randomRange(max, min);
    } else {
        return (((rand()%div) / (double)div)*(max - min))+min;
    }
}

int roundToInt(double a) {
    return (int) round(a);
}

int roundAwayFrom0(double a) {
    return (int) (a > 0 ? ceil(a) : floor(a));
}

int roundToward0(double a) {
    return (int) (a > 0 ? floor(a) : ceil(a));
}

bool aproxEqual(double a, double b, double epsilon){
    return abs(a - b) < epsilon;
}
