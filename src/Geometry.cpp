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

//Point2:

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


Point2 Point2::operator-() {
    return Point2(-this->x, -this->y);
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


string Point2::toString() {
    return to_string(x)+", "+to_string(y);
}

void Point2::save(Point2 p, vector<unsigned char>* data){
    FileUtility::saveInt(data, p.x);
    FileUtility::saveInt(data, p.y);
}

Point2 Point2::load(unsigned char* data, int* position){
    return Point2(FileUtility::loadInt(data, position), FileUtility::loadInt(data, position));
}

//Point3:

Point3::Point3(int x, int y, int z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

bool Point3::operator==(const Point3 other) {
    return this->x == other.x && this->y == other.y && this->z == other.z;
}

bool Point3::operator!=(const Point3 other) {
    return !(*this == other);
}

void Point3::set(const Point3 other) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
}


Point3 Point3::operator+( Point3 other) {
    return Point3(this->x+other.x, this->y+other.y, this->z+other.z);
}

Point3 Point3::operator-( Point3 other) {
    return Point3(this->x-other.x, this->y-other.y, this->z-other.z);
}

Point3 Point3::operator*( Point3 other) {
    return Point3(this->x*other.x, this->y*other.y, this->z*other.z);
}

Point3 Point3::operator/( Point3 other) {
    return Point3(this->x/other.x, this->y/other.y, this->z/other.z);
}


Point3* Point3::operator+=(Point3 other) {
    this->x+=other.x;
    this->y+=other.y;
    this->z+=other.z;
    return this;
}

Point3* Point3::operator-=(Point3 other) {
    this->x-=other.x;
    this->y-=other.y;
    this->z-=other.z;
    return this;
}

Point3* Point3::operator*=( Point3 other) {
    this->x*=other.x;
    this->y*=other.y;
    this->z*=other.z;
    return this;
}

Point3* Point3::operator/=( Point3 other) {
    this->x/=other.x;
    this->y/=other.y;
    this->z/=other.z;
    return this;
}


Point3 Point3::operator+(int other) {
    return Point3(this->x+other, this->y+other, this->z+other);
}

Point3 Point3::operator-( int other) {
    return Point3(this->x-other, this->y-other, this->z-other);
}

Point3 Point3::operator*( int other) {
    return Point3(this->x*other, this->y*other, this->z*other);
}

Point3 Point3::operator/( int other) {
    return Point3(this->x/other, this->y/other, this->z/other);
}


Point3 Point3::operator-() {
    return Point3(-this->x, -this->y, -this->z);
}


string Point3::toString() {
    return to_string(x)+", "+to_string(y)+", "+to_string(z);
}

void Point3::save(vector<unsigned char>* data){
    FileUtility::saveInt(data, x);
    FileUtility::saveInt(data, y);
    FileUtility::saveInt(data, z);
}

Point3 Point3::load(unsigned char* data, int* position){
    return Point3(FileUtility::loadInt(data, position), FileUtility::loadInt(data, position), FileUtility::loadInt(data, position));
}

//Vector2:


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

Point2 Vector2::round() {
    return Point2((int)::round(this->x), (int)::round(this->y));
}

Point2 Vector2::ceil() {
    return Point2((int)::ceil(this->x), (int)::ceil(this->y));
}

Point2 Vector2::floor() {
    return Point2((int)::floor(this->x), (int)::floor(this->y));
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
    return Math::distanceSquared(a.x, a.y, b.x, b.y);
}



string Vector2::toString() {
    return to_string(x)+", "+to_string(y);
}
