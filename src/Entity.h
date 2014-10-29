//
//  Entity.h
//  Underneath
//
//  Created by Braeden Atlee on 10/1/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Entity__
#define __Underneath__Entity__

class Level;

#include "Level.h"
#include "Ui.h"
#include "Geometry.h"

class Entity {

public:

    Entity(char icon, Point2 startPos, Ui::color colorCode = Ui::COLOR_DEFAULT_ENTITY);

    virtual ~Entity();

    virtual bool tryToMove(Point2 p, Level* world);
    virtual bool update(int tick, Level* level);

    virtual char getIcon(Point2 p, int tick, Level* level);

    Point2 getPos() {
        return *pos;
    }

    int getViewDistance() {
        return viewDistance;
    }

    int getColorCode();


protected:
    char defaultIcon = '?';
    Point2* pos;
    Point2* lastPos;
    int colorCode;
    int viewDistance = 12;
    bool updateIcon = true;
};





#endif /* defined(__Underneath__Entity__) */
