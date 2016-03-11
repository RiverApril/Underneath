//
//  Geometry.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/6/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Geometry.hpp"
#include "Math.hpp"
#include "Utility.hpp"
#include "Global.hpp"


Point2 Point2Zero = Point2(0, 0);
Point2 Point2Left = Point2(-1, 0);
Point2 Point2Right = Point2(1, 0);
Point2 Point2Up = Point2(0, -1);
Point2 Point2Down = Point2(0, 1);
Point2 Point2One = Point2(1, 1);
Point2 Point2Neg1 = Point2(-1, -1);

Vector2 Vector2Zero = Vector2(0.0, 0.0);
Vector2 Vector2One = Vector2(1, 1);
Vector2 Vector2OneHalf = Vector2(.5, .5);

Vector3 Vector3Zero = Vector3(0, 0, 0);

//Point2:

Point2::Point2(int x, int y) : x(x), y(y){}

bool Point2::operator==(const Point2 other) {
    return this->x == other.x && this->y == other.y;
}


bool Point2::operator>(const Point2 other) {
    return this->x > other.x && this->y > other.y;
}

bool Point2::operator<(const Point2 other) {
    return this->x < other.x && this->y < other.y;
}

bool Point2::operator<=(const Point2 other) {
    return this->x <= other.x && this->y <= other.y;
}

bool Point2::operator>=(const Point2 other) {
    return this->x >= other.x && this->y >= other.y;
}


bool Point2::operator!=(const Point2 other) {
    return !(*this == other);
}

void Point2::set(const Point2 other) {
    this->x = other.x;
    this->y = other.y;
}

void Point2::add(int x, int y) {
    this->x += x;
    this->y += y;
}

Point2 Point2::abs(){
    return Point2(x < 0 ? -x : x, y < 0 ? -y : y);
}

Point2 Point2::operator+(Point2 other) {
    return Point2(this->x + other.x, this->y + other.y);
}

Point2 Point2::operator-(Point2 other) {
    return Point2(this->x - other.x, this->y - other.y);
}

Point2 Point2::operator*(Point2 other) {
    return Point2(this->x * other.x, this->y * other.y);
}

Point2 Point2::operator/(Point2 other) {
    return Point2(this->x / other.x, this->y / other.y);
}

Point2 Point2::operator%(Point2 other) {
    return Point2(this->x % other.x, this->y % other.y);
}

Point2* Point2::operator+=(Point2 other) {
    this->x += other.x;
    this->y += other.y;
    return this;
}

Point2* Point2::operator-=(Point2 other) {
    this->x -= other.x;
    this->y -= other.y;
    return this;
}

Point2* Point2::operator*=(Point2 other) {
    this->x *= other.x;
    this->y *= other.y;
    return this;
}

Point2* Point2::operator/=(Point2 other) {
    this->x /= other.x;
    this->y /= other.y;
    return this;
}

Point2 Point2::operator+(int other) {
    return Point2(this->x + other, this->y + other);
}

Point2 Point2::operator-(int other) {
    return Point2(this->x - other, this->y - other);
}

Point2 Point2::operator*(int other) {
    return Point2(this->x*other, this->y * other);
}

Point2 Point2::operator/(int other) {
    return Point2(this->x / other, this->y / other);
}

Point2 Point2::operator%(int other) {
    return Point2(this->x % other, this->y % other);
}

Point2 Point2::operator-() {
    return Point2(-this->x, -this->y);
}

Vector2 Point2::operator+(double other) {
    return Vector2(this->x + other, this->y + other);
}

Vector2 Point2::operator-(double other) {
    return Vector2(this->x - other, this->y - other);
}

Vector2 Point2::operator*(double other) {
    return Vector2(this->x*other, this->y * other);
}

Vector2 Point2::operator/(double other) {
    return Vector2(this->x / other, this->y / other);
}

string Point2::toString() {
    return to_string(x) + ", " + to_string(y);
}

void Point2::save(vector<unsigned char>* data) {
    Utility::saveInt(data, x);
    Utility::saveInt(data, y);
}

Point2::Point2(vector<unsigned char>* data, int* position) {
    x = Utility::loadInt(data, position);
    y = Utility::loadInt(data, position);
}

//Point3:

Point3::Point3(int x, int y, int z) : x(x), y(y), z(z){}

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

Point3 Point3::operator+(Point3 other) {
    return Point3(this->x + other.x, this->y + other.y, this->z + other.z);
}

Point3 Point3::operator-(Point3 other) {
    return Point3(this->x - other.x, this->y - other.y, this->z - other.z);
}

Point3 Point3::operator*(Point3 other) {
    return Point3(this->x * other.x, this->y * other.y, this->z * other.z);
}

Point3 Point3::operator/(Point3 other) {
    return Point3(this->x / other.x, this->y / other.y, this->z / other.z);
}

Point3* Point3::operator+=(Point3 other) {
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    return this;
}

Point3* Point3::operator-=(Point3 other) {
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    return this;
}

Point3* Point3::operator*=(Point3 other) {
    this->x *= other.x;
    this->y *= other.y;
    this->z *= other.z;
    return this;
}

Point3* Point3::operator/=(Point3 other) {
    this->x /= other.x;
    this->y /= other.y;
    this->z /= other.z;
    return this;
}

Point3 Point3::operator+(int other) {
    return Point3(this->x + other, this->y + other, this->z + other);
}

Point3 Point3::operator-(int other) {
    return Point3(this->x - other, this->y - other, this->z - other);
}

Point3 Point3::operator*(int other) {
    return Point3(this->x*other, this->y*other, this->z * other);
}

Point3 Point3::operator/(int other) {
    return Point3(this->x / other, this->y / other, this->z / other);
}

Point3 Point3::operator-() {
    return Point3(-this->x, -this->y, -this->z);
}

string Point3::toString() {
    return to_string(x) + ", " + to_string(y) + ", " + to_string(z);
}

void Point3::save(vector<unsigned char>* data) {
    Utility::saveInt(data, x);
    Utility::saveInt(data, y);
    Utility::saveInt(data, z);
}

Point3::Point3(vector<unsigned char>* data, int* position) {
    x = Utility::loadInt(data, position);
    y = Utility::loadInt(data, position);
    z = Utility::loadInt(data, position);
}

//Vector2:

Vector2::Vector2(double x, double y) : x(x), y(y){}

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

double Vector2::angleRad(){
    return atan2(y, x);
}

double Vector2::randomRange(){
    return ::randomRange(x, y);
}

Point2 Vector2::roundAwayFrom0() {
    return Point2(::roundAwayFrom0(this->x), ::roundAwayFrom0(this->y));
}

Point2 Vector2::roundToward0() {
    return Point2(::roundToward0(this->x), ::roundToward0(this->y));
}

Point2 Vector2::truncate() {
    return Point2((int) (this->x), (int) (this->y));
}

Point2 Vector2::round() {
    return Point2((int) ::round(this->x), (int) ::round(this->y));
}

Point2 Vector2::ceil() {
    return Point2((int) ::ceil(this->x), (int) ::ceil(this->y));
}

Point2 Vector2::floor() {
    return Point2((int) ::floor(this->x), (int) ::floor(this->y));
}

Vector2 Vector2::normalize() {
    double m = sqrt(x*x+y*y);
    return Vector2(x/m, y/m);
}

Vector2 Vector2::operator+(double other) {
    return Vector2(this->x + other, this->y + other);
}

Vector2 Vector2::operator-(double other) {
    return Vector2(this->x - other, this->y - other);
}

Vector2 Vector2::operator*(double other) {
    return Vector2(this->x*other, this->y * other);
}

Vector2 Vector2::operator/(double other) {
    return Vector2(this->x / other, this->y / other);
}

Vector2 Vector2::operator+(Point2 other){
	return Vector2(this->x + other.x, this->y + other.y);
}

Vector2 Vector2::operator-(Point2 other){
    return Vector2(this->x - other.x, this->y - other.y);
}

Vector2 Vector2::operator*(Point2 other){
    return Vector2(this->x * other.x, this->y * other.y);
}

Vector2 Vector2::operator/(Point2 other){
    return Vector2(this->x / other.x, this->y / other.y);
}

Vector2 Vector2::operator+(Vector2 other){
    return Vector2(this->x + other.x, this->y + other.y);
}

Vector2 Vector2::operator-(Vector2 other){
    return Vector2(this->x - other.x, this->y - other.y);
}

Vector2 Vector2::operator*(Vector2 other){
    return Vector2(this->x * other.x, this->y * other.y);
}

Vector2 Vector2::operator/(Vector2 other){
    return Vector2(this->x / other.x, this->y / other.y);
}


Vector2 Vector2::operator-(){
    return Vector2(-this->x, -this->y);
}


Vector2* Vector2::operator+=(Vector2 other){
    this->x += other.x;
    this->y += other.y;
    return this;
}

Vector2* Vector2::operator-=(Vector2 other){
    this->x -= other.x;
    this->y -= other.y;
    return this;
}

Vector2* Vector2::operator*=(Vector2 other){
    this->x *= other.x;
    this->y *= other.y;
    return this;
}

Vector2* Vector2::operator/=(Vector2 other){
    this->x /= other.x;
    this->y /= other.y;
    return this;
}


Vector2* Vector2::operator+=(double other){
    this->x += other;
    this->y += other;
    return this;
}

Vector2* Vector2::operator-=(double other){
    this->x -= other;
    this->y -= other;
    return this;
}

Vector2* Vector2::operator*=(double other){
    this->x *= other;
    this->y *= other;
    return this;
}

Vector2* Vector2::operator/=(double other){
    this->x /= other;
    this->y /= other;
    return this;
}

void Vector2::save(vector<unsigned char>* data) {
    Utility::saveDouble(data, x);
    Utility::saveDouble(data, y);
}

Vector2::Vector2(vector<unsigned char>* data, int* position) {
    x = Utility::loadDouble(data, position);
    y = Utility::loadDouble(data, position);
}

//Vector3:



Vector3::Vector3(double x, double y, double z) : x(x), y(y), z(z){}

bool Vector3::operator==(Vector3 other) {
    return this->x == other.x && this->y == other.y && this->z == other.z;
}

bool Vector3::operator!=(Vector3 other) {
    return !(*this == other);
}

void Vector3::set(Vector3 other) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
}

void Vector3::set(Point3 other) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
}

Point3 Vector3::roundAwayFrom0() {
    return Point3(::roundAwayFrom0(this->x), ::roundAwayFrom0(this->y), ::roundAwayFrom0(this->z));
}

Point3 Vector3::roundToward0() {
    return Point3(::roundToward0(this->x), ::roundToward0(this->y), ::roundAwayFrom0(this->z));
}

Point3 Vector3::truncate() {
    return Point3((int) (this->x), (int) (this->y), (int) (this->z));
}

Point3 Vector3::round() {
    return Point3((int) ::round(this->x), (int) ::round(this->y), (int) ::round(this->z));
}

Point3 Vector3::ceil() {
    return Point3((int) ::ceil(this->x), (int) ::ceil(this->y), (int) ::ceil(this->z));
}

Point3 Vector3::floor() {
    return Point3((int) ::floor(this->x), (int) ::floor(this->y), (int) ::floor(this->z));
}

Vector2 Vector3::xy() {
    return Vector2(x, y);
}

Vector3 Vector3::operator+(double other) {
    return Vector3(this->x + other, this->y + other, this->z + other);
}

Vector3 Vector3::operator-(double other) {
    return Vector3(this->x - other, this->y - other, this->z - other);
}

Vector3 Vector3::operator*(double other) {
    return Vector3(this->x*other, this->y * other, this->z * other);
}

Vector3 Vector3::operator/(double other) {
    return Vector3(this->x / other, this->y / other, this->z / other);
}

Vector3 Vector3::operator+(Point3 other){
    return Vector3(this->x + other.x, this->y + other.y, this->z + other.y);
}

Vector3 Vector3::operator-(Point3 other){
    return Vector3(this->x - other.x, this->y - other.y, this->z - other.z);
}

Vector3 Vector3::operator*(Point3 other){
    return Vector3(this->x * other.x, this->y * other.y, this->z * other.z);
}

Vector3 Vector3::operator/(Point3 other){
    return Vector3(this->x / other.x, this->y / other.y, this->z / other.z);
}

Vector3 Vector3::operator+(Vector3 other){
    return Vector3(this->x + other.x, this->y + other.y, this->z + other.z);
}

Vector3 Vector3::operator-(Vector3 other){
    return Vector3(this->x - other.x, this->y - other.y, this->z - other.z);
}

Vector3 Vector3::operator*(Vector3 other){
    return Vector3(this->x * other.x, this->y * other.y, this->z * other.z);
}

Vector3 Vector3::operator/(Vector3 other){
    return Vector3(this->x / other.x, this->y / other.y, this->z / other.z);
}


Vector3 Vector3::operator-(){
    return Vector3(-this->x, -this->y, -this->z);
}


Vector3* Vector3::operator+=(Vector3 other){
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    return this;
}

Vector3* Vector3::operator-=(Vector3 other){
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    return this;
}

Vector3* Vector3::operator*=(Vector3 other){
    this->x *= other.x;
    this->y *= other.y;
    this->z *= other.z;
    return this;
}

Vector3* Vector3::operator/=(Vector3 other){
    this->x /= other.x;
    this->y /= other.y;
    this->z /= other.z;
    return this;
}


Vector3* Vector3::operator+=(double other){
    this->x += other;
    this->y += other;
    this->z += other;
    return this;
}

Vector3* Vector3::operator-=(double other){
    this->x -= other;
    this->y -= other;
    this->z -= other;
    return this;
}

Vector3* Vector3::operator*=(double other){
    this->x *= other;
    this->y *= other;
    this->z *= other;
    return this;
}

Vector3* Vector3::operator/=(double other){
    this->x /= other;
    this->y /= other;
    this->z /= other;
    return this;
}

void Vector3::save(vector<unsigned char>* data) {
    Utility::saveDouble(data, x);
    Utility::saveDouble(data, y);
    Utility::saveDouble(data, z);
}

Vector3::Vector3(vector<unsigned char>* data, int* position) {
    x = Utility::loadDouble(data, position);
    y = Utility::loadDouble(data, position);
    z = Utility::loadDouble(data, position);
}


double distanceSquared(Point2 a, Point2 b) {
    return distanceSquared(a.x, a.y, b.x, b.y);
}

string Vector2::toString() {
    return to_string(x) + ", " + to_string(y);
}
