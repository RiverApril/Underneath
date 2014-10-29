//
//  Geometry.h
//  Underneath
//
//  Created by Braeden Atlee on 10/5/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Geometry__
#define __Underneath__Geometry__

#include "Global.h"

struct Vector2;

struct Point2 {
    int x,y;

    Point2(int x, int y);
    Point2() : Point2(0, 0){}
    bool operator==(const Point2 other);
    bool operator!=(const Point2 other);
    void set(const Point2 *other);

     Point2 operator+(Point2 other);
     Point2 operator-(Point2 other);
     Point2 operator*(Point2 other);
     Point2 operator/(Point2 other);

    Point2* operator+=(Point2 other);
    Point2* operator-=(Point2 other);
    Point2* operator*=(Point2 other);
    Point2* operator/=(Point2 other);

     Point2 operator+(int other);
     Point2 operator-(int other);
     Point2 operator*(int other);
    Point2 operator/(int other);

    Vector2 operator+(double other);
    Vector2 operator-(double other);
    Vector2 operator*(double other);
    Vector2 operator/(double other);

    void set(int i) {
        this->x = i;
        this->y = i;
    }
    std::string toString();
};

struct Vector2 {
    double x,y;

    Vector2(double x, double y);
    bool operator==(const Vector2 &other);
    bool operator!=(const Vector2 &other);
    void set(const Vector2 *other);
    void set(const Point2 *other);
    Point2 truncate();

    Vector2 operator+(double other);
    Vector2 operator-(double other);
    Vector2 operator*(double other);
    Vector2 operator/(double other);

    std::string toString();
};

extern Point2 Point2Zero;
extern Point2 Point2Left;
extern Point2 Point2Right;
extern Point2 Point2Up;
extern Point2 Point2Down;


#endif /* defined(__Underneath__Geometry__) */
