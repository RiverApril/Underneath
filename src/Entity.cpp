//
//  Entity.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/1/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Entity.h"
#include "Geometry.h"
#include "Math.h"
#include "Ui.h"
#include "Global.h"

Entity::Entity(std::string icon, char iconAlt, const Point2 startPos, Ui::color colorCode) {
    this->defaultIcon = icon;
    this->defaultIconAlt = iconAlt;
    this->colorCode = colorCode;

    pos = new Point2(0, 0);
    lastPos = new Point2(0, 0);
    pos->set(&startPos);
    lastPos->set(&startPos);
}

Entity::~Entity() {
    delete pos;
    delete lastPos;
}

bool Entity::tryToMove(Point2 p, Level* level) {
    if(!level->tileAt(*pos+p)->isSolid()) {
        pos->x += p.x;
        pos->y += p.y;
        return true;
    }
    return false;
}

bool Entity::update(int tick, Level* level) {

    bool u = false;

    if(pos != lastPos || updateIcon) {
        if(level->inRange(*lastPos)) {
            level->displayEntityGrid[lastPos->x][lastPos->y] = nullptr;
        }
        if(level->inRange(*pos)) {
            level->displayEntityGrid[pos->x][pos->y] = this;
        }
        u = true;
        lastPos->set(pos);

        updateIcon = false;
    }

    return u;
}

const std::string Entity::getIcon(Point2 p, int tick, Level* level) {
    return defaultIcon;
}

const char Entity::getIconAlt(Point2 p, int tick, Level* level) {
    return defaultIconAlt;
}

const int Entity::getColorCode() {
    return colorCode;
}
