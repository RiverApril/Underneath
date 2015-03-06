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

Entity::Entity(char icon, Point2 startPos, Ui::Color colorCode) {
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
    if(p == Point2Zero){
        return false;
    }
    return tryToMoveAbsalute(pos+p, level);
}

bool Entity::update(double deltaTime, double time, Level* level) {

    bool u = false;
    

    if(pos != lastPos || updateIcon) {
        
        //setAndUnsetDisplayEntity(level);

        u = true;
        lastPos = pos;

        updateIcon = false;
    }

    return u;
}

char Entity::getIcon(Point2 p, double time, Level* level) {
    return defaultIcon;
}

Ui::Color Entity::getFgColorCode() {
    return fgColorCode;
}

Ui::Color Entity::getBgColorCode() {
    return bgColorCode;
}


Entity* Entity::cloneUnsafe(Entity* oldE, Entity* newE){

    newE->defaultIcon = oldE->defaultIcon;
    newE->pos = oldE->pos;
    newE->lastPos = oldE->lastPos;
    newE->fgColorCode = oldE->fgColorCode;
    newE->bgColorCode = oldE->bgColorCode;
    newE->updateIcon = oldE->updateIcon;
    newE->solid = oldE->solid;

    return newE;
}

template<class Super, class Sub>
Sub* Entity::makeNewAndClone(Super* oldT){
    Sub* newT = new Sub();
    return Sub::cloneUnsafe(dynamic_cast<Sub*>(oldT), newT);
}

Entity* Entity::clone(Entity* oldE){

    int type = oldE->getEntityTypeId();

    debug("Clone Entity of type: "+to_string(type));

    switch (type) {
        case ENTITY_TYPE_ENTITY:
            return makeNewAndClone<Entity, Entity>(oldE);

        case ENTITY_TYPE_ALIVE:
            return makeNewAndClone<Entity, Alive>(oldE);

        case ENTITY_TYPE_AIENTITY:
            return makeNewAndClone<Entity, AiEntity>(oldE);

        case ENTITY_TYPE_PLAYER:
            return makeNewAndClone<Entity, Player>(oldE);

        case ENTITY_TYPE_ITEMENTITY:
            return makeNewAndClone<Entity, ItemEntity>(oldE);

        default:
            throw FileUtility::FileExceptionLoad("Entity type unknown: "+to_string(type));
            return nullptr;
            break;
    }
    
    
    
}

void Entity::save(vector<unsigned char>* data){
    FileUtility::saveInt(data, getEntityTypeId());


    FileUtility::saveInt(data, uniqueId);
    
    FileUtility::saveUnsignedChar(data, (unsigned char)defaultIcon);
    
    Point2::save(pos, data);
    Point2::save(lastPos, data);
    
    FileUtility::saveUnsignedChar(data, (unsigned char)fgColorCode);
    FileUtility::saveUnsignedChar(data, (unsigned char)bgColorCode);
    FileUtility::saveBool(data, solid);
}

int Entity::getEntityTypeId(){
    return ENTITY_TYPE_ENTITY;
}

void Entity::load(unsigned char* data, int* position){
    //Entity::loadNew() loads the entityId

    uniqueId = FileUtility::loadInt(data, position);

    defaultIcon = (char)FileUtility::loadUnsignedChar(data, position);

    pos = Point2::load(data, position);
    lastPos = Point2::load(data, position);

    fgColorCode = (char)FileUtility::loadUnsignedChar(data, position);
    bgColorCode = (char)FileUtility::loadUnsignedChar(data, position);
    solid = FileUtility::loadBool(data, position);

    updateIcon = true;
}

Entity* Entity::loadNew(unsigned char* data, int* position){
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
            throw FileUtility::FileExceptionLoad("Entity type unknown: "+to_string(type));
            return nullptr;
            break;
    }
    e->load(data, position);

    debug("Loaded Entity: "+e->getName()+"("+to_string(e->getEntityTypeId())+")"+", Pos: "+e->pos.toString());

    return e;
}



