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

namespace Geometry {

typedef float vector;

struct Point2 {
    int x,y;

    Point2(int x, int y);
    Point2() : Point2(0, 0){}
    bool operator==(const Point2 &other);
    bool operator!=(const Point2 &other);
    void set(const Point2 *other);
    void set(int i) {
        this->x = i;
        this->y = i;
    }
    std::string toString();
};

struct Vector2 {
    vector x,y;

    Vector2(vector x, vector y);
    bool operator==(const Vector2 &other);
    bool operator!=(const Vector2 &other);
    void set(const Vector2 *other);
    void set(const Point2 *other);
    Point2 truncate();
    std::string toString();
};

extern const Point2* Point2Zero;
}

#endif /* defined(__Underneath__Geometry__) */
