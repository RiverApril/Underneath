//
//  Math.h
//  Underneath
//
//  Created by Braeden Atlee on 10/18/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Math__
#define __Underneath__Math__

namespace Math {

constexpr const double pi = 3.14159;

int distanceSquared(int ax, int ay, int bx, int by);
int distanceSquared(int x, int y);

template <typename T> const T abs(const T a) {
    return (a<0)?-a:a;
}

template <typename T> const T min(const T a, const T b) {
    return (a<b)?a:b;
}

template <typename T> const T max(const T a, const T b) {
    return (a>b)?a:b;
}
}

#endif /* defined(__Underneath__Math__) */
