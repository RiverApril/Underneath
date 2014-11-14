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

Entity::Entity(string name, char icon, Point2 startPos, Ui::color colorCode) {
    this->defaultIcon = icon;
    this->fgColorCode = colorCode;
    this->bgColorCode = Ui::C_BLACK;
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

bool Entity::tryToMoveAbsalute(Point2 p, shared_ptr<Level> level) {
    if(!level->tileAt(p)->isSolid()) {
        bool block = false;
        int i;
        forVector(level->entityList, i){
            if(level->entityList[i]->uniqueId == uniqueId){
                continue;
            }
            if(level->entityList[i]->getPos() == p){

                if(level->entityList[i]->isSolid()){
                    block = true;
                    break;
                }
            }
        }
        if(!block){
            pos->x = p.x;
            pos->y = p.y;
            return true;
        }
    }
    return false;
}
bool Entity::tryToMoveRelative(Point2 p, shared_ptr<Level> level) {
    return tryToMoveAbsalute(*pos+p, level);
}

bool Entity::update(int tick, shared_ptr<Level> level) {

    bool u = false;
    

    if(pos != lastPos || updateIcon) {
        if(level->inRange(*lastPos)) {
            if(level->getDisplayEntity(*lastPos) != nullptr && level->getDisplayEntity(*lastPos)->uniqueId == uniqueId){
            	level->setDisplayEntity(*lastPos, nullptr);
            }
        }
        if(level->inRange(*pos)) {
            level->setDisplayEntity(*pos, shared_from_this());
        }
        u = true;
        lastPos->set(*pos);

        updateIcon = false;
    }

    return u;
}

char Entity::getIcon(Point2 p, int tick, shared_ptr<Level> level) {
    return defaultIcon;
}

int Entity::getFgColorCode() {
    return fgColorCode;
}

int Entity::getBgColorCode() {
    return bgColorCode;
}


shared_ptr<Entity> Entity::clone(shared_ptr<Entity> oldE, shared_ptr<Entity> newE){

    if(newE == nullptr){
        newE = shared_ptr<Entity>(new Entity());
    }

    newE->defaultIcon = oldE->defaultIcon;
    newE->name = oldE->name;
    newE->pos->set(*oldE->pos);
    newE->lastPos->set(*oldE->lastPos);
    newE->fgColorCode = oldE->fgColorCode;
    newE->bgColorCode = oldE->bgColorCode;
    newE->updateIcon = oldE->updateIcon;
    newE->solid = oldE->solid;

    return newE;
}

void Entity::save(string* data){
    Utility::saveInt(data, getEntityTypeId());

    Utility::saveInt(data, uniqueId);
    
    Utility::saveChar(data, defaultIcon);
    Utility::saveString(data, name);
    pos->save(data);
    lastPos->save(data);
    Utility::saveInt(data, fgColorCode);
    Utility::saveInt(data, bgColorCode);
    Utility::saveBool(data, solid);
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
    fgColorCode = Utility::loadInt(data, position);
    bgColorCode = Utility::loadInt(data, position);
    updateIcon = true;
    solid = Utility::loadBool(data, position);
}

shared_ptr<Entity> Entity::loadNew(char* data, int* position){
    shared_ptr<Entity> e;

    int type = Utility::loadInt(data, position);

    if(type == ENTITY_TYPE_ENTITY){
        e = shared_ptr<Entity>(new Entity());
    }else if(type == ENTITY_TYPE_ALIVE){
        e = shared_ptr<Alive>(new Alive());
    }else if(type == ENTITY_TYPE_AIENTITY){
        e = shared_ptr<AiEntity>(new AiEntity());
    }else if(type == ENTITY_TYPE_PLAYER){
        e = shared_ptr<Player>(new Player());
    }
    e->load(data, position);

    return e;
}



