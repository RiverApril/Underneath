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

    Entity(std::string icon, char iconAlt, const Geometry::Point2* startPos, Ui::color colorCode = Ui::COLOR_DEFAULT_ENTITY);

    virtual ~Entity();

    virtual bool tryToMove(int x, int y, Level* world);
    virtual bool update(int tick, Level* level);

    virtual const std::string getIcon(int x, int y, int tick, Level* level);
    virtual const char getIconAlt(int x, int y, int tick, Level* level);

    Geometry::Point2* const getPos() {
        return pos;
    }

    int const getViewDistance() {
        return viewDistance;
    }

    const int getColorCode();


protected:
    std::string defaultIcon = "?";
    char defaultIconAlt = '?';
    Geometry::Point2* pos;
    Geometry::Point2* lastPos;
    int colorCode;
    int viewDistance = 8;
    bool updateIcon = true;
};





#endif /* defined(__Underneath__Entity__) */
