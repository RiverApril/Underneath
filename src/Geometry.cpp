//
//  Geometry.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/6/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Geometry.h"
#include "Math.h"
#include "Global.h"

namespace Geometry {

const Point2* Point2Zero = new Point2(0, 0);

Point2::Point2(int x, int y) {
    this->x = x;
    this->y = y;
}

bool Point2::operator==(const Point2 &other) {
    return this->x == other.x && this->y == other.y;
}

bool Point2::operator!=(const Point2 &other) {
    return !(*this == other);
}

void Point2::set(const Point2 *other) {
    this->x = other->x;
    this->y = other->y;
}

std::string Point2::toString() {
    return std::to_string(x)+", "+std::to_string(y);
}




Vector2::Vector2(vector x, vector y) {
    this->x = x;
    this->y = y;
}

bool Vector2::operator==(const Vector2 &other) {
    return this->x == other.x && this->y == other.y;
}

bool Vector2::operator!=(const Vector2 &other) {
    return !(*this == other);
}

void Vector2::set(const Vector2 *other) {
    this->x = other->x;
    this->y = other->y;
}

void Vector2::set(const Point2 *other) {
    this->x = other->x;
    this->y = other->y;
}

Point2 Vector2::truncate() {
    return Point2((int)(this->x), (int)(this->y));
}

std::string Vector2::toString() {
    return std::to_string(x)+", "+std::to_string(y);
}
}
