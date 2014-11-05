//
//  Entity.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/1/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Entity.h"
#include "Alive.h"
#include "AiEntity.h"
#include "Player.h"

#include "Level.h"
#include "Math.h"
#include "Utility.h"
#include "Global.h"

Entity::Entity() : Entity("", ' ', Point2Zero, Ui::C_WHITE){

}

Entity::Entity(std::string name, char icon, Point2 startPos, Ui::color colorCode) {
    this->defaultIcon = icon;
    this->colorCode = colorCode;
    this->name = name;

    pos = new Point2(0, 0);
    lastPos = new Point2(0, 0);
    pos->set(startPos);
    lastPos->set(startPos);
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
            level->setDisplayEntity(*lastPos, nullptr);
        }
        if(level->inRange(*pos)) {
            level->setDisplayEntity(*pos, this);
        }
        u = true;
        lastPos->set(*pos);

        updateIcon = false;
    }

    return u;
}

char Entity::getIcon(Point2 p, int tick, Level* level) {
    return defaultIcon;
}

int Entity::getColorCode() {
    return colorCode;
}


Entity* Entity::clone(Entity* oldE, Entity* newE){

    if(newE == nullptr){
        newE = new Entity();
    }

    newE->defaultIcon = oldE->defaultIcon;
    newE->name = oldE->name;
    newE->pos->set(*oldE->pos);
    newE->lastPos->set(*oldE->lastPos);
    newE->colorCode = oldE->colorCode;
    newE->updateIcon = oldE->updateIcon;

    return newE;
}

void Entity::save(std::string* data){
    Utility::saveInt(data, getEntityTypeId());

    Utility::saveInt(data, uniqueId);
    
    Utility::saveChar(data, defaultIcon);
    Utility::saveString(data, name);
    pos->save(data);
    lastPos->save(data);
    Utility::saveInt(data, colorCode);
}

int Entity::getEntityTypeId(){
    return ENTITY_TYPE_ENTITY;
}

void Entity::load(char* data, int* position){
    uniqueId = Utility::loadInt(data, position);

    defaultIcon = Utility::loadChar(data, position);
    name = Utility::loadString(data, position);
    pos->set(Point2::load(data, position));
    lastPos->set(Point2::load(data, position));
    colorCode = Utility::loadInt(data, position);
}

Entity* Entity::loadNew(char* data, int* position){
    Entity* e;

    int type = Utility::loadInt(data, position);

    if(type == ENTITY_TYPE_ENTITY){
        e = new Entity();
    }else if(type == ENTITY_TYPE_ALIVE){
        e = new Alive();
    }else if(type == ENTITY_TYPE_AIENTITY){
        e = new AiEntity();
    }else if(type == ENTITY_TYPE_PLAYER){
        e = new Player();
    }
    e->load(data, position);

    return e;
}



