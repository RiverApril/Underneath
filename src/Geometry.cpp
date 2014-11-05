//
//  Geometry.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/6/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Geometry.h"
#include "Math.h"
#include "Utility.h"
#include "Global.h"

Point2 Point2Zero = Point2(0, 0);
Point2 Point2Left = Point2(-1, 0);
Point2 Point2Right = Point2(1, 0);
Point2 Point2Up = Point2(0, -1);
Point2 Point2Down = Point2(0, 1);

Point2::Point2(int x, int y) {
    this->x = x;
    this->y = y;
}

bool Point2::operator==(const Point2 other) {
    return this->x == other.x && this->y == other.y;
}

bool Point2::operator!=(const Point2 other) {
    return !(*this == other);
}

void Point2::set(const Point2 other) {
    this->x = other.x;
    this->y = other.y;
}


Point2 Point2::operator+( Point2 other) {
    return Point2(this->x+other.x, this->y+other.y);
}

Point2 Point2::operator-( Point2 other) {
    return Point2(this->x-other.x, this->y-other.y);
}

Point2 Point2::operator*( Point2 other) {
    return Point2(this->x*other.x, this->y*other.y);
}

Point2 Point2::operator/( Point2 other) {
    return Point2(this->x/other.x, this->y/other.y);
}


Point2* Point2::operator+=(Point2 other) {
    this->x+=other.x;
    this->y+=other.y;
    return this;
}

Point2* Point2::operator-=(Point2 other) {
    this->x-=other.x;
    this->y-=other.y;
    return this;
}

Point2* Point2::operator*=( Point2 other) {
    this->x*=other.x;
    this->y*=other.y;
    return this;
}

Point2* Point2::operator/=( Point2 other) {
    this->x/=other.x;
    this->y/=other.y;
    return this;
}


Point2 Point2::operator+(int other) {
    return Point2(this->x+other, this->y+other);
}

Point2 Point2::operator-( int other) {
    return Point2(this->x-other, this->y-other);
}

Point2 Point2::operator*( int other) {
    return Point2(this->x*other, this->y*other);
}

Point2 Point2::operator/( int other) {
    return Point2(this->x/other, this->y/other);
}


Vector2 Point2::operator+(double other) {
    return Vector2(this->x+other, this->y+other);
}

Vector2 Point2::operator-( double other) {
    return Vector2(this->x-other, this->y-other);
}

Vector2 Point2::operator*( double other) {
    return Vector2(this->x*other, this->y*other);
}

Vector2 Point2::operator/( double other) {
    return Vector2(this->x/other, this->y/other);
}


std::string Point2::toString() {
    return std::to_string(x)+", "+std::to_string(y);
}

void Point2::save(std::string* data){
    Utility::saveInt(data, x);
    Utility::saveInt(data, y);
}

Point2 Point2::load(char* data, int* position){
    return Point2(Utility::loadInt(data, position), Utility::loadInt(data, position));
}



Vector2::Vector2(double x, double y) {
    this->x = x;
    this->y = y;
}

bool Vector2::operator==(Vector2 other) {
    return this->x == other.x && this->y == other.y;
}

bool Vector2::operator!=(Vector2 other) {
    return !(*this == other);
}

void Vector2::set(Vector2 other) {
    this->x = other.x;
    this->y = other.y;
}

void Vector2::set(Point2 other) {
    this->x = other.x;
    this->y = other.y;
}

Point2 Vector2::truncate() {
    return Point2((int)(this->x), (int)(this->y));
}




Vector2 Vector2::operator+(double other) {
    return Vector2(this->x+other, this->y+other);
}

Vector2 Vector2::operator-( double other) {
    return Vector2(this->x-other, this->y-other);
}

Vector2 Vector2::operator*( double other) {
    return Vector2(this->x*other, this->y*other);
}

Vector2 Vector2::operator/( double other) {
    return Vector2(this->x/other, this->y/other);
}

int distanceSquared(Point2 a, Point2 b){
    return distanceSquared(a.x, a.y, b.x, b.y);
}



std::string Vector2::toString() {
    return std::to_string(x)+", "+std::to_string(y);
}
