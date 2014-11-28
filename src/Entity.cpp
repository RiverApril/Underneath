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
#include "ItemEntity.h"

#include "Level.h"
#include "Math.h"
#include "Utility.h"
#include "Global.h"

Entity::Entity() : Entity(' ', Point2Zero, Ui::C_WHITE){

}

Entity::Entity(char icon, Point2 startPos, Ui::color colorCode) {
    this->defaultIcon = icon;
    this->fgColorCode = colorCode;
    this->bgColorCode = Ui::C_BLACK;

    pos = startPos;
    lastPos = startPos;
}

Entity::~Entity() {

}

bool Entity::tryToMoveAbsalute(Point2 p, Level* level) {
    if(!level->tileAt(p)->isSolid()) {
        bool block = false;
        for(Entity* e : level->entityList){
            if(e->uniqueId == uniqueId){
                continue;
            }
            if(e->pos == p){

                if(e->isSolid()){
                    block = true;
                    break;
                }
            }
        }
        if(!block){
            pos = p;
            return true;
        }
    }
    return false;
}
bool Entity::tryToMoveRelative(Point2 p, Level* level) {
    /*if(p.x != 0 && p.y != 0){
        bool xx = tryToMoveRelative(Point2(p.x, 0), level);
        bool yy = tryToMoveRelative(Point2(0, p.y), level);
        return xx || yy;
    }*/
    return tryToMoveAbsalute(pos+p, level);
}

bool Entity::update(int tick, Level* level) {

    bool u = false;
    

    if(pos != lastPos || updateIcon) {
        
        //setAndUnsetDisplayEntity(level);

        u = true;
        lastPos = pos;

        updateIcon = false;
    }

    return u;
}

char Entity::getIcon(Point2 p, int tick, Level* level) {
    return defaultIcon;
}

Ui::color Entity::getFgColorCode() {
    return fgColorCode;
}

Ui::color Entity::getBgColorCode() {
    return bgColorCode;
}


Entity* Entity::clone(Entity* oldE, Entity* newE){

    if(newE == nullptr){
        newE = new Entity();
    }

    newE->defaultIcon = oldE->defaultIcon;
    newE->pos = oldE->pos;
    newE->lastPos = oldE->lastPos;
    newE->fgColorCode = oldE->fgColorCode;
    newE->bgColorCode = oldE->bgColorCode;
    newE->updateIcon = oldE->updateIcon;
    newE->solid = oldE->solid;

    return newE;
}

void Entity::save(string* data){
    FileUtility::saveInt(data, getEntityTypeId());


    FileUtility::saveInt(data, uniqueId);
    
    FileUtility::saveChar(data, defaultIcon);
    
    Point2::save(pos, data);
    Point2::save(lastPos, data);
    
    FileUtility::saveChar(data, fgColorCode);
    FileUtility::saveChar(data, bgColorCode);
    FileUtility::saveBool(data, solid);
}

int Entity::getEntityTypeId(){
    return ENTITY_TYPE_ENTITY;
}

void Entity::load(char* data, int* position){
    //Entity::loadNew() loads the entityId

    uniqueId = FileUtility::loadInt(data, position);

    defaultIcon = FileUtility::loadChar(data, position);

    pos = Point2::load(data, position);
    lastPos = Point2::load(data, position);

    fgColorCode = FileUtility::loadChar(data, position);
    bgColorCode = FileUtility::loadChar(data, position);
    solid = FileUtility::loadBool(data, position);

    updateIcon = true;
}

Entity* Entity::loadNew(char* data, int* position){
    Entity* e;

    int type = FileUtility::loadInt(data, position);

    switch (type) {
        case ENTITY_TYPE_ENTITY:
            e = new Entity();
            break;
        case ENTITY_TYPE_ALIVE:
            e = new Alive();
            break;
        case ENTITY_TYPE_AIENTITY:
            e = new AiEntity();
            break;
        case ENTITY_TYPE_PLAYER:
            e = new Player();
            break;
        case ENTITY_TYPE_ITEMENTITY:
            e = new ItemEntity();
            break;

        default:
            throw FileUtility::ExceptionLoad("Entity type unknown: "+to_string(type));
            return nullptr;
            break;
    }
    e->load(data, position);

    debug("Loaded Entity: "+e->getName()+"("+to_string(e->getEntityTypeId())+")"+", Pos: "+e->pos.toString());

    return e;
}



