//
//  Icon.cpp
//  Underneath
//
//  Created by Emily Atlee on 4/15/15.
//  Copyright (c) 2015 Emily Atlee. All rights reserved.
//

#include "Icon.hpp"
#include "Random.hpp"

vector<Icon*> iconList;

char RandomIcon::getChar(unsigned long tick, Point2 pos, Level* lvl){
    return i[Random::randomFromPoint(pos)%i.size()];
}

char RandomColorIcon::getChar(unsigned long tick, Point2 pos, Level* lvl){
    return i[Random::randomFromPoint(pos)%i.size()];
}

Ui::Color RandomColorIcon::getFgColor(unsigned long tick, Point2 pos, Level* lvl){
    return fg[Random::randomFromPoint(pos)%fg.size()];
}

Ui::Color RandomColorIcon::getBgColor(unsigned long tick, Point2 pos, Level* lvl){
    return bg[Random::randomFromPoint(pos)%bg.size()];
}



char AnimatedIcon::getChar(unsigned long tick, Point2 pos, Level* lvl){
    return i[((tick/speed)+(randomStart?Random::randomFromPoint(pos):0))%i.size()];
}

char ColorAnimatedIcon::getChar(unsigned long tick, Point2 pos, Level* lvl){
    return i[((tick/speed)+(randomStart?Random::randomFromPoint(pos):0))%i.size()];
}


