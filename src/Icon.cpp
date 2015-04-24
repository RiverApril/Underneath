//
//  Icon.cpp
//  Underneath
//
//  Created by Braeden Atlee on 4/15/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Icon.h"
#include "Random.h"

char RandomIcon::getChar(unsigned long tick, Point2 pos, Level* lvl){
    return i[Random::randomFromPoint(pos)%i.size()];
}

