//
//  Math.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/18/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Math.h"

namespace Math {

    int distanceSquared(int ax, int ay, int bx, int by) {
        int dx = abs(ax - bx);
        int dy = abs(ay - by);

        return dx*dx + dy*dy;
    }

    int distanceSquared(int x, int y) {
        int dx = abs(x);
        int dy = abs(y);

        return dx*dx + dy*dy;
    }

}
