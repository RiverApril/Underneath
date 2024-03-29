//
//  Geometry.hpp
//  Underneath
//
//  Created by Emily Atlee on 10/5/14.
//  Copyright (c) 2014 Emily Atlee. All rights reserved.
//

#ifndef __Underneath__Geometry__
#define __Underneath__Geometry__

#include "Global.hpp"

struct Vector2;

struct Point2 {
    int x, y;

    Point2(int x, int y);

    Point2() : Point2(0, 0) {
    }
    bool operator==(const Point2 other);
    bool operator!=(const Point2 other);
    bool operator>(const Point2 other);
    bool operator<(const Point2 other);
    bool operator>=(const Point2 other);
    bool operator<=(const Point2 other);

    Point2 xOnly() {
        return Point2(x, 0);
    }

    Point2 yOnly() {
        return Point2(0, y);
    }

    bool inRange(int xMin, int yMin, int xMax, int yMax) {
        return x >= xMin && y >= yMin && x <= xMax && y <= yMax;
    }

    bool inRange(Point2 min, Point2 max) {
        return x >= min.x && y >= min.y && x <= max.x && y <= max.y;
    }

    void set(const Point2 other);

    void add(int x, int y);

    Point2 abs();

    Point2 operator+(Point2 other);
    Point2 operator-(Point2 other);
    Point2 operator*(Point2 other);
    Point2 operator/(Point2 other);
    Point2 operator%(Point2 other);

    Point2 operator-();

    Point2* operator+=(Point2 other);
    Point2* operator-=(Point2 other);
    Point2* operator*=(Point2 other);
    Point2* operator/=(Point2 other);

    Point2 operator+(int other);
    Point2 operator-(int other);
    Point2 operator*(int other);
    Point2 operator/(int other);
    Point2 operator%(int other);

    Vector2 operator+(double other);
    Vector2 operator-(double other);
    Vector2 operator*(double other);
    Vector2 operator/(double other);

    void save(vector<unsigned char>* data);

    Point2(vector<unsigned char>* data, int* position);

    void set(int i) {
        this->x = i;
        this->y = i;
    }

    double xPlusY() {
        return x + y;
    }

    string toString();
};

struct Point3 {
    int x, y, z;

    Point3(int x, int y, int z);

    Point3() : Point3(0, 0, 0) {
    }
    bool operator==(const Point3 other);
    bool operator!=(const Point3 other);



    void set(const Point3 other);

    Point3 operator+(Point3 other);
    Point3 operator-(Point3 other);
    Point3 operator*(Point3 other);
    Point3 operator/(Point3 other);

    Point3 operator-();

    Point3* operator+=(Point3 other);
    Point3* operator-=(Point3 other);
    Point3* operator*=(Point3 other);
    Point3* operator/=(Point3 other);

    Point3 operator+(int other);
    Point3 operator-(int other);
    Point3 operator*(int other);
    Point3 operator/(int other);

    void save(vector<unsigned char>* data);

    Point3(vector<unsigned char>* data, int* position);

    void set(int i) {
        this->x = i;
        this->y = i;
    }
    string toString();
};

struct Vector2 {
    double x, y;

    Vector2(double x, double y);

    Vector2() : Vector2(0.0, 0.0) {}
    
    Vector2(Point2 p) : Vector2(p.x, p.y) {}

    bool operator==(const Vector2 other);
    bool operator!=(const Vector2 other);
    void set(const Vector2 other);
    void set(const Point2 other);
    double randomRange();
    Point2 roundAwayFrom0();
    Point2 roundToward0();
    Point2 truncate();
    Point2 round();
    Point2 ceil();
    Point2 floor();
    double angleRad();

    Vector2 normalize();

    Vector2 operator+(double other);
    Vector2 operator-(double other);
    Vector2 operator*(double other);
    Vector2 operator/(double other);

    Vector2 operator+(Point2 other);
    Vector2 operator-(Point2 other);
    Vector2 operator*(Point2 other);
    Vector2 operator/(Point2 other);

    Vector2 operator+(Vector2 other);
    Vector2 operator-(Vector2 other);
    Vector2 operator*(Vector2 other);
    Vector2 operator/(Vector2 other);

    Vector2 operator-();

    Vector2* operator+=(Vector2 other);
    Vector2* operator-=(Vector2 other);
    Vector2* operator*=(Vector2 other);
    Vector2* operator/=(Vector2 other);

    Vector2* operator+=(double other);
    Vector2* operator-=(double other);
    Vector2* operator*=(double other);
    Vector2* operator/=(double other);

    void save(vector<unsigned char>* data);

    Vector2(vector<unsigned char>* data, int* position);

    string toString();
};



struct Vector3 {
    double x, y, z;

    Vector3(double x, double y, double z);

    Vector3() : Vector3(0, 0, 0) {}

    Vector3(Vector2 p, double z) : Vector3(p.x, p.y, z) {}

    Vector3(Point2 p, double z) : Vector3(p.x, p.y, z) {}

    bool operator==(const Vector3 other);
    bool operator!=(const Vector3 other);
    void set(const Vector3 other);
    void set(const Point3 other);
    Point3 roundAwayFrom0();
    Point3 roundToward0();
    Point3 truncate();
    Point3 round();
    Point3 ceil();
    Point3 floor();
    Vector2 xy();

    Vector3 operator+(double other);
    Vector3 operator-(double other);
    Vector3 operator*(double other);
    Vector3 operator/(double other);

    Vector3 operator+(Point3 other);
    Vector3 operator-(Point3 other);
    Vector3 operator*(Point3 other);
    Vector3 operator/(Point3 other);

    Vector3 operator+(Vector3 other);
    Vector3 operator-(Vector3 other);
    Vector3 operator*(Vector3 other);
    Vector3 operator/(Vector3 other);

    Vector3 operator-();

    Vector3* operator+=(Vector3 other);
    Vector3* operator-=(Vector3 other);
    Vector3* operator*=(Vector3 other);
    Vector3* operator/=(Vector3 other);

    Vector3* operator+=(double other);
    Vector3* operator-=(double other);
    Vector3* operator*=(double other);
    Vector3* operator/=(double other);

    void save(vector<unsigned char>* data);

    Vector3(vector<unsigned char>* data, int* position);
    
    string toString();
};


double distanceSquared(Point2 a, Point2 b);

extern Point2 Point2Zero;
extern Point2 Point2Left;
extern Point2 Point2Right;
extern Point2 Point2Up;
extern Point2 Point2Down;
extern Point2 Point2Neg1;
extern Point2 Point2One;

extern Vector2 Vector2Zero;
extern Vector2 Vector2One;
extern Vector2 Vector2OneHalf;

extern Vector3 Vector3Zero;


#endif /* defined(__Underneath__Geometry__) */
