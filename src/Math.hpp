//
//  Math.hpp
//  Underneath
//
//  Created by Braeden Atlee on 10/18/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Math__
#define __Underneath__Math__

#define PI 3.14159265358979
#define TAU (2*PI)
#define Deg1InRad (TAU/360)

double distanceSquared(double ax, double ay, double bx, double by);
double distanceSquared(double x, double y);

double randomRange(double min, double max);

/*template <typename T> const T abs(const T a) {
    return (a < 0) ? -a : a;
}

template <typename T> const T min(const T a, const T b) {
    return (a < b) ? a : b;
}

template <typename T> const T max(const T a, const T b) {
    return (a > b) ? a : b;
}*/

int roundToInt(double a);

int roundAwayFrom0(double a);
int roundToward0(double a);

bool aproxEqual(double a, double b, double epsilon = 0.00001);


#endif /* defined(__Underneath__Math__) */
